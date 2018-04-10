/*
 * BdSpeech.hpp
 *
 *  Created on: Dec 12, 2017
 *      Author: fu
 */

#ifndef ASRDEMO_ASRDEMO_CONTROLLER_HPP_
#define ASRDEMO_ASRDEMO_CONTROLLER_HPP_

#include <string>
#include <atomic>
#include <mutex>


#include "BDSpeechSDK.hpp"
#include "BDSSDKMessage.hpp"
#include "bds_ASRDefines.hpp"
#include "bds_asr_key_definitions.hpp"

#include "common.h"
#include "ResultListener.hpp"

namespace asrdemo {

class AsrdemoController {
public:
    /**
     * @param app_name IN 您自己的定义的程序名称，可能用于之后服务端的排查
     * @param resource_path IN 含有bds_easr_mfe_cmvn.dat和bds_easr_mfe_dnn.dat这两个文件的目录
     * @param listener IN ResultListener实例，含有您处理结果的业务逻辑
     */
    AsrdemoController(const std::string &app_name, const std::string &resource_path, ResultListener &listener);

    virtual ~AsrdemoController();

    /**
     * 配置参数。具体参数见在线文档 ，参数列表：输入参数列表：ASR_CMD_CONFIG一节
     *
     * @param config_params IN
     * @param error_msg OUT 错误消息
     * @return 是否成功 不成功时错误消息在error_msg输出
     */
    bool config(bds::BDSSDKMessage &config_params, std::string &error_msg);

    /**
     * 推送音频数据，建议320字节（audio_buf_len = 320）
     *
     * @param audio_buf IN 二进制音频数据
     * @param audio_buf_len
     */
    bool post_audio_data(const char *audio_buf, int audio_buf_len, std::string &error_msg);

    /**
     * 音频流结束时调用
     */
    bool post_data_finish_and_stop(std::string &error_msg);

    /**
     * 取消调用
     */
    bool cancel(std::string &error_msg);


    /**
     * 打开日志文件
     *
     * logFileName IN 日志文件路径
     * fileSize IN 实际日志文件大小为 512K*fileSize， 超过后自动重命名并覆盖之前的“logFileName".bak文件
     */
    static void open_log_file(const char *logFileName, int fileSize = 0) {
        bds::BDSpeechSDK::open_log_file(logFileName, fileSize);
    }

    /**
     * 关闭日志文件，与open_log_file配对使用
     */
    static void close_log_file() {
        bds::BDSpeechSDK::close_log_file();
    }

    /**
     * 不再需要SDK识别时，释放所有资源
     */
    static void do_cleanup() {
        bds::BDSpeechSDK::do_cleanup();
    }

protected:
    /**
     * 用于post_data_finish_and_stop
     */
    bool stop(std::string &error_msg);

    /**
     * 用于output_callback ，释放识别实例
     */
    void release();

    /**
     * 收到SDK的3个空闲状态返回，认为识别过程结束
     */
    static void output_callback(bds::BDSSDKMessage &message, void *user_arg);

private:
    const std::string _app_name;
    const std::string _resource_path;
    // 用户自己的回调逻辑
    ResultListener &_listener;

    bool _is_configed;
    std::atomic_bool _is_finished = ATOMIC_VAR_INIT(false);
    std::mutex _finish_mutex;

    // 调用SDK出错
    bool _has_error;
    std::string _error_msg;

    bds::BDSpeechSDK *_sdk;
    bds::BDSSDKMessage _push_params;

    // 设置配置参数
    void set_config_params(bds::BDSSDKMessage &config_params);

    // 设置启动参数
    void set_start_params();
    DISALLOW_COPY_AND_ASSIGN(AsrdemoController);
};
}/* namespace asrdemo */

#endif /* ASRDEMO_ASRDEMO_BDSPEECHCONTROL_HPP_ */
