/*
 * asrdemo_PrintResultListener.h
 *
 *  Created on: Dec 15, 2017
 *      Author: fu
 */

#ifndef YOURS_PRINTRESULTLISTENER_HPP_
#define YOURS_PRINTRESULTLISTENER_HPP_

#include <string>
#include "asrdemo/common.h"
#include "asrdemo/StatusListener.hpp"

namespace yours {

class PrintResultListener : public asrdemo::StatusListener {
public:
    /**
     * prefix IN 打印日志使用
     */
    PrintResultListener(const std::string &prefix);

    virtual ~PrintResultListener();

protected:
    /**
     * 识别开始
     */
    virtual void on_start_working();

    /**
     * 检查到一句话开始
     */
    virtual void on_start();

    /**
     * 检查到一句话结束
     */
    virtual void on_end();

    /**
     * 临时识别结果
     *
     * json IN 含有临时结果的json
     */
    virtual void on_flush_data(const std::string &json);

    /**
     * 一句话识别结束。
     * 未开启长语音时，表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
     *
     * json IN 含有最终结果的json
     */
    virtual void on_finish(const std::string &json);

    /**
     * 语义理解结果
     *
     * json IN 含有最终结果的json
     */
    virtual void on_nlu(const std::string &json);

    /**
     * 长语音识别结束，（长语音 = 多个“一句话”的识别过程）
     * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
     */
    virtual void on_long_speech_end();

    /**
     * 识别出错
     * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
     *
     * err_domain IN 错误领域
     * err_code IN 具体错误码
     * err_desc IN 错误描述
     * sn IN 一句话的识别过程中的logId
     */
    virtual void on_error(int err_domain, int err_code, const std::string &err_desc, const std::string &sn);

    /**
     * 识别被用户取消
     * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
     */
    virtual void on_cancel();

private:
    const std::string _prefix;

    void write_log(const std::string str) const;
    DISALLOW_COPY_AND_ASSIGN(PrintResultListener);
};

} /* namespace asrdemo */

#endif /* YOURS_PRINTRESULTLISTENER_HPP_ */
