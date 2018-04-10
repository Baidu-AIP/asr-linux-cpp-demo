
#include <string>
#include <BDSSDKMessage.hpp>
#include <bds_asr_key_definitions.hpp>
#include <bds_ASRDefines.hpp>
#include <bits/unique_ptr.h>
#include <thread>
#include <asrdemo/Util.hpp>
#include "recognizer.hpp"
#include "common.h"

using namespace asr_srt;

const int MAX_NUM_RECOG = 6; //并发数
const int MAX_NUM_FILES = 20; // 一共需要识别多少个文件

// %d 从1开始到 MAX_NUM_FILES
const char TEMPLATE[] = "../data/video/fasheng%02d.pcm";
// "../data/video/fasheng%02d.pcm"
// "../data/16k-%d.pcm"


std::atomic_int g_count = ATOMIC_VAR_INIT(0);


static void set_config(bds::BDSSDKMessage &cfg_params) {

    // app_id app_key app_secret 请测试成功后替换为您在网页上申请的appId appKey和appSecret
    cfg_params.set_parameter(bds::ASR_PARAM_KEY_APP_ID, "10455099");
    cfg_params.set_parameter(bds::ASR_PARAM_KEY_CHUNK_KEY, "rKCHBLmYiFPuCQTS0HttLbUD");
    cfg_params.set_parameter(bds::ASR_PARAM_KEY_SECRET_KEY, "037dc446820ec143d1628c20146b9d34");

    // 普通话搜索模型(没有逗号）)：1536，普通话搜索模型+语义理解 15361, 普通话输入法模型（有逗号） 1537
    cfg_params.set_parameter(bds::ASR_PARAM_KEY_PRODUCT_ID, "1537");

    cfg_params.set_parameter(bds::COMMON_PARAM_KEY_DEBUG_LOG_LEVEL, bds::EVRDebugLogLevelOff); //关闭debug日志 ，上线时请注释此行
    cfg_params.set_parameter(bds::ASR_PARAM_KEY_COMPRESSION_TYPE, bds::EVR_AUDIO_COMPRESSION_PCM);
    // float vad_pause_time_ms = 700.0;  //设置vad语句静音切分门限, ms。 即原始语音静音 700ms后，SDK认为一句话结束
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_MAX_SPEECH_PAUSE, vad_pause_time_ms);

    //以下是不常用的参数
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_COMPRESSION_TYPE, bds::EVR_AUDIO_COMPRESSION_BV32); // 有损压缩
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_SAVE_AUDIO_ENABLE, 1);    //是否存识别的音频
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_SAVE_AUDIO_PATH, "sdk_save_audio.d");  //存音频的路径
    // cfg_params.set_parameter(bds::ASR_PARAM_KEY_DISABLE_PUNCTUATION, 0); // 输出时没有逗号 搜索模型有效

}

static std::unique_ptr<asr_srt::Recognizer> create_recognizer(int num);

static void run();

static int run_one(int num);

int main(int argc, char *argv[]) {
    init_log(argc, argv);
    asrdemo::AsrdemoController::open_log_file("asrdemo-srt.log", 25);
    std::thread threads[MAX_NUM_RECOG];
    for(int i=0; i<MAX_NUM_RECOG; i++){
        threads[i] = std::thread(run);
    }
    for(int i=0; i<MAX_NUM_RECOG; i++){
        if  (threads[i].joinable()){
            threads[i].join();
        }
    }
    asrdemo::AsrdemoController::close_log_file(); // 程序退出时或者不需要再写日志 固定用法， 引擎空闲时调用
    asrdemo::AsrdemoController::do_cleanup(); // 程序退出时或者不需要asr服务时 固定用法， 引擎空闲时调用
    release_log();
    return 0;
}

void run() {
    bool will_stop = false;
    while (!will_stop) {
        int count = g_count.fetch_add(1) + 1;
        if (count > MAX_NUM_FILES) {
            will_stop = true;
        } else {
            run_one(count);
        }
    }
}

static int run_one(int num) {
    std::string error_msg;
    std::unique_ptr<asr_srt::Recognizer> recognizer = create_recognizer(num);
    bool will_read_file = true;
    bool has_error = false;
    while (will_read_file) {
        RETURN_PROCESS_STATUS status = recognizer->process_one_frame(error_msg);
        //ALOG << "status : " << status;
        switch (status) {
            case RETURN_ERROR:
                fprintf(stderr, "%s, ERROR %s\n",recognizer->get_filename().c_str(), error_msg.c_str());
                has_error = true;
                break;
            case RETURN_SKIP:
            case RETURN_PROCESSING: {
                uint64_t time = recognizer->get_next_process_time();
                uint64_t sleepMs = time - asrdemo::Util::current_timestamp();
                // ALOG << recognizer->get_filename() << " : begin to sleep ms :" << sleepMs;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
                break;
            }
            case RETURN_FILE_END:
                will_read_file = false;
                break;
            default:
                ALOG << "other status : " << status << std::endl;
                break;
        }
        if (has_error){
            break;
        }
    }
    if (!has_error) {
        has_error = !recognizer->try_waiting_finish(error_msg);
        if (has_error) {
            fprintf(stderr, "ERROR END %s\n", error_msg.c_str());
        }
    }
    return 0;
}


static std::unique_ptr<asr_srt::Recognizer> create_recognizer(int num) {

    char filename[100];
    snprintf(filename, sizeof(filename), TEMPLATE, num);

    bds::BDSSDKMessage cfg_params;
    set_config(cfg_params);
    asr_srt::Recognizer *recog = new asr_srt::Recognizer(filename);
    std::string error_msg;
    recog->config(cfg_params,error_msg ); // 此步不处理错误
    return std::unique_ptr<asr_srt::Recognizer>(recog);

}