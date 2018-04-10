//============================================================================
// Name        : asrdemo2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "yours_PrintResultListener.hpp"
#include "asrdemo/AsrdemoController.hpp"
#include "asrdemo/Util.hpp"

using namespace std;

/**
 * 0. demo不用修改任何代码，可以直接运行。测试成功后请进行以下步骤
 * 1. 请修改set_config 函数，配置您的参数。
 * 2. 在main函数中选择recog_one_file进行单文件识别，或者recog_multi_files同时识别多个文件。
 *
 * 8k 采样率不再支持
 */


static void set_config(bds::BDSSDKMessage &cfg_params) {
    // app_id app_key app_secret 请测试成功后替换为您在网页上申请的appId appKey和appSecret
    const std::string app_id = "10455099";
    const std::string app_key = "rKCHBLmYiFPuCQTS0HttLbUD";
    const std::string app_secret = "037dc446820ec143d1628c20146b9d34";

    const std::string product_id = "1536"; // 普通话搜索模型(没有逗号）)：1536，普通话搜索模型+语义理解 15361, 普通话输入法模型（有逗号） 1537

    cfg_params.set_parameter(bds::ASR_PARAM_KEY_APP_ID, app_id);
    cfg_params.set_parameter(bds::ASR_PARAM_KEY_CHUNK_KEY, app_key);
    cfg_params.set_parameter(bds::ASR_PARAM_KEY_SECRET_KEY, app_secret);
    cfg_params.set_parameter(bds::ASR_PARAM_KEY_PRODUCT_ID, product_id);
    cfg_params.set_parameter(bds::COMMON_PARAM_KEY_DEBUG_LOG_LEVEL, bds::EVRDebugLogLevelOff); //关闭debug日志 ，上线时请注释此行
    // float vad_pause_time_ms = 700.0;  //设置vad语句静音切分门限, ms。 即原始语音静音 700ms后，SDK认为一句话结束
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_MAX_SPEECH_PAUSE, vad_pause_time_ms);

    //以下是不常用的参数
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_COMPRESSION_TYPE, bds::EVR_AUDIO_COMPRESSION_BV32); // 有损压缩
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_SAVE_AUDIO_ENABLE, 1);    //是否存识别的音频
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_SAVE_AUDIO_PATH, "sdk_save_audio.d");  //存音频的路径
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_DISABLE_PUNCTUATION, 0); // 输出时没有逗号 搜索模型有效

}

/**识别单个文件*/
static void recog_one_file();

/**识别多个文件*/
static void recog_multi_files();

// 调用set_config函数，写入sdk的配置参数
static void _config(asrdemo::AsrdemoController &controller);

// 按320字节大小，将数据post到sdk中。
static bool _post_data(istream &io, asrdemo::AsrdemoController &controller, int &sleep_ms, string &error_msg);

// 判断流是否结束或异常(io.eof())，结束的话，调用post_data_finish_and_stop，告知sdk音频结束
static int _push_data_eof(istream &io, asrdemo::AsrdemoController &controller, string &error_msg);

// flags 是否都是true
static bool _is_all_true(bool flags[], int size);

int main(int argc, char **argv) {
    cout << "BEGIN!" << endl;
    //此处25 表示日志文件为 25*512k大小，asrdemo.log超过大小后，自动重命名为asrdemo.log.bak。asrdemo.log继续生成
    asrdemo::AsrdemoController::open_log_file("asrdemo.log", 25);
    recog_one_file(); // 识别一个文件
    // recog_multi_files(); // 一个线程里识别多个文件
    asrdemo::AsrdemoController::close_log_file(); // 程序退出时或者不需要再写日志 固定用法， 引擎空闲时调用
    asrdemo::AsrdemoController::do_cleanup(); // 程序退出时或者不需要asr服务时 固定用法， 引擎空闲时调用
    return 0;
}

/**
 * 识别一个文件
 */
static void recog_one_file() {
    string filename = "../pcm/16k-0.pcm";
    ifstream io(filename.c_str());

    if (!io || !io.is_open()) {
        cerr << "read file error:" << filename << endl;
        exit(1);
    }

    // _audio_file_rate_is_8k = true； // 8k采样率音频设为true
    yours::PrintResultListener listener(filename);
    asrdemo::AsrdemoController controller("testYourApp", "../../../resources/asr_resource/", listener);
    _config(controller);

    bool will_continue = true;
    int sleep_ms = 0;
    while (will_continue) {
        string error_msg;
        int status = _push_data_eof(io, controller, error_msg); // 判断是否是文件结尾，是的话，结束这个音频文件的SDK调用
        if (status < 0) { // 出错
            cerr << "stop audio data error:" << error_msg << " : " << status << endl;
            will_continue = false;
        } else if (status == 1) { // 文件读完了
            will_continue = false;
            io.close();
        } else { // 正常
            if (sleep_ms > 0) { //上次_post_data欠的sleep
                //cout << "sleep ："<<sleep_ms<<endl;
                usleep(sleep_ms * 1000); // 一个音频的需要sleep的总长约等于音频时间，如果不遵守，服务器将可能会返回异常结果。
            }
            will_continue = _post_data(io, controller, sleep_ms, error_msg); // 继续从io读取一个包传入
            // cout << "push audio data success  "<<readed_len<<endl;
            if (!will_continue) { //  _post_data() 调用发生异常
                cerr << "stop audio data error:" << error_msg << " : " << status << endl;
            }
        }
    }

    /* 取消识别
     * if (!controller.cancel(error_msg)) {
     cerr << "push audio data error:" << error_msg << endl;
     exit(3);
     }*/

    cout << "begin to sleep" << endl; // 直到controller.is_finished()
    while (!listener.is_finished()) {
        sleep(1);
    }
    cout << "ASR FINISHED" << endl;
}

/**
 * 识别多个文件
 */
static void recog_multi_files() {
    const int num_files = 2; // 不超过10， [1-10]
    const char *filename_template = "../pcm/%d.pcm";
    // _audio_file_rate_is_8k = true； // 8k采样率音频设为true

    asrdemo::AsrdemoController *controllers[num_files]; // bd_speech_control_impl 控制类
    yours::PrintResultListener *listeners[num_files]; // PrintResultListener为示例，用户可以参照实现自己的实现类
    ifstream *ifs[num_files]; // 打开的音频文件
    uint64_t next_sleep_timestamps[num_files]; // 下次sleep需要达到的的时间点
    bool files_eof[num_files]; // 文件是否已经读完
    bool sdk_finished[num_files]; // SDK识别完成

    char tmp_filename[256];
    for (int i = 0; i < num_files; i++) {
        snprintf(tmp_filename, 256, filename_template, i);
        string filename(tmp_filename);
        // 3个new，对应结尾3个delete
        ifs[i] = new ifstream(filename.c_str());
        listeners[i] = new yours::PrintResultListener(filename);
        controllers[i] = new asrdemo::AsrdemoController("testAppM", "../../../resources/asr_resource/", *listeners[i]);
        if (!(*ifs[i]) || !ifs[i]->is_open()) {
            cerr << "read file error:" << filename << endl;
            exit(1);
        }
        next_sleep_timestamps[i] = 0;
        files_eof[i] = false;
        sdk_finished[i] = false;
    }

    for (int i = 0; i < num_files; i++) {
        _config(*controllers[i]);
    }

    bool is_success = true;
    int status = 0;
    int sleep_ms = 0;
    std::string error_msg;
    while (!_is_all_true(files_eof, num_files)) {
        for (int i = 0; i < num_files; i++) {
            if (files_eof[i]) {
                // cout << "files finished:" << i  << endl;
                continue;// 文件已经结束
            }
            status = _push_data_eof(*ifs[i], *controllers[i], error_msg);
            if (status < 0) { // 出错
                cerr << "stop audio data error:" << error_msg << " : " << status << endl;
                exit(3);
            } else if (status == 1) { //文件结束
                // cout << "file ended ：" << i << endl;
                files_eof[i] = true;
                ifs[i]->close();
            } else {
                if (next_sleep_timestamps[i] > 0) {
                    sleep_ms = next_sleep_timestamps[i] - asrdemo::Util::current_timestamp(); // 需要sleep的耗时
                    if (sleep_ms > 0) {
                        // cout << "sleep ：" << sleep_ms << endl;
                        usleep(sleep_ms * 1000);
                    }
                }

                is_success = _post_data(*ifs[i], *controllers[i], sleep_ms, error_msg); // 是否post成功，一般都是成功的。
                if (!is_success) {
                    cerr << "stop audio data error:" << error_msg << " : " << status << endl;
                    exit(3);
                }
                next_sleep_timestamps[i] = asrdemo::Util::current_timestamp() + sleep_ms;
            }
        }
    }
    cout << "ASR MULTIFILES BEGIN TO SLEEP" << endl;
    while (!_is_all_true(sdk_finished, num_files)) {  // 直到所有controllers都结束
        for (int i = 0; i < num_files; i++) {
            if (sdk_finished[i]) {
                continue;
            }
            if (listeners[i]->is_finished()) {
                sdk_finished[i] = true;
            } else {
                //cout << "SLEEP 1s to wait" << endl;
                sleep(1);
                break;
            }
        }
    }
    for (int i = 0; i < num_files; i++) {
        delete ifs[i];
        delete listeners[i];
        delete controllers[i];

    }

    cout << "ASR MULTIFILES FINISHED" << endl;

}

static void _config(asrdemo::AsrdemoController &controller) {
    bds::BDSSDKMessage cfg_params;
    std::string error_msg;
    set_config(cfg_params);
    // cout << "Config is :\n  " << asrdemo::Util::params_to_string(cfg_params);
    if (controller.config(cfg_params, error_msg)) {
        cout << "FOR Feedback : Config is filled :\n" << asrdemo::Util::params_to_string(cfg_params) << endl;
    } else {
        cerr << error_msg << " , END!" << endl;
        exit(2);
    }
}

/**
 * 判断流是否结束或异常(io.eof())，结束的话，调用post_data_finish_and_stop，告知sdk音频结束
 */
static int _push_data_eof(istream &io, asrdemo::AsrdemoController &controller, string &error_msg) {
    if (io.eof()) {
        if (!controller.post_data_finish_and_stop(error_msg)) {
            return -1; // 出错
        }
        return 1; // 文件结束，并且告知了sdk音频结束
    }
    return 0; // 文件未结束
}

/**
 * 按320字节大小从io读取，将数据post到sdk中。
 */
static bool _post_data(istream &io, asrdemo::AsrdemoController &controller, int &sleep_ms, string &error_msg) {
    const int audio_buf_len = 320;
    char audio_buf[audio_buf_len];
    int readed_len = 0;

    io.read(audio_buf, audio_buf_len);
    readed_len = io.gcount();
    if (readed_len < 0) {
        error_msg = " readed_len is <0";
        return false;
    }
    if (readed_len == 0) { // 读到0字节
        sleep_ms = 0;
        return true;
    }
    if (!controller.post_audio_data(audio_buf, readed_len, error_msg)) {
        return false;
    }

    sleep_ms = asrdemo::Util::cal_speech_16k_duration_ms(readed_len); // 根据audio_buf_len 计算音频长度 16k采样率

    return true;
}

static bool _is_all_true(bool flags[], int size) {
    for (int i = 0; i < size; i++) {
        if (!flags[i]) {
            return false;
        }
    }
    return true;
}
