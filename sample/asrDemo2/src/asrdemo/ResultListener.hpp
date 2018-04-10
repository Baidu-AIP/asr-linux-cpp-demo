/*
 * asrdemo_ResultListener.hpp
 *
 *  Created on: Dec 13, 2017
 *      Author: fu
 */

#ifndef ASRDEMO_RESULTLISTENER_HPP_
#define ASRDEMO_RESULTLISTENER_HPP_

#include <string>
#include "BDSSDKMessage.hpp"
#include "bds_ASRDefines.hpp"
#include "bds_asr_key_definitions.hpp"

namespace asrdemo {

class ResultListener {
public:
    ResultListener();

    virtual ~ResultListener();

    virtual void output_callback(bds::BDSSDKMessage &message, int status);

protected:

    /**
     * 识别开始 EVoiceRecognitionClientWorkStatusStartWorkIng
     */
    virtual void on_start_working();

    /**
     * 检查到一句话开始 EVoiceRecognitionClientWorkStatusStart
     */
    virtual void on_start();

    /**
     * 检查到一句话结束 EVoiceRecognitionClientWorkStatusEnd
     */
    virtual void on_end();

    /**
     * 临时识别结果
     *
     * json IN 含有临时结果的json EVoiceRecognitionClientWorkStatusFlushData
     */
    virtual void on_flush_data(const std::string &json);

    /**
     * 一句话识别结束。 EVoiceRecognitionClientWorkStatusFinish
     * 未开启长语音时，表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
     *
     * json IN 含有最终结果的json
     */
    virtual void on_finish(const std::string &json) = 0;

    /**
     * 语义理解结果
     *
     * json IN 含有最终结果的json EVoiceRecognitionClientWorkStatusChunkNlu
     */
    virtual void on_nlu(const std::string &json);

    /**
     * 长语音识别结束，（长语音 = 多个“一句话”的识别过程） EVoiceRecognitionClientWorkStatusLongSpeechEnd
     * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
     */
    virtual void on_long_speech_end();

    /**
     * 识别出错 EVoiceRecognitionClientWorkStatusError
     * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
     *
     * err_domain IN 错误领域
     * err_code IN 具体错误码
     * err_desc IN 错误描述
     * sn IN 一句话的识别过程中的logId
     */
    virtual void on_error(int err_domain, int err_code, const std::string &err_desc, const std::string &sn) = 0;

    /**
     * 识别被用户取消 EVoiceRecognitionClientWorkStatusCancel
     * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
     */
    virtual void on_cancel();

    virtual void on_last_status(int status);
};

} /* namespace asrdemo */

#endif /* ASRDEMO_RESULTLISTENER_HPP_ */
