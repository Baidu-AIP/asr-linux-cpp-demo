/*
 * asrdemo_PrintResultListener.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: fu
 */

#include "yours_PrintResultListener.hpp"
#include <iostream>
#include <sstream>
#include "asrdemo/Util.hpp"

namespace yours {

PrintResultListener::PrintResultListener(const std::string &prefix) :
        asrdemo::StatusListener(), _prefix(prefix) {
    write_log("PrintResultListener constructor");

}

PrintResultListener::~PrintResultListener() {
    write_log("PrintResultListener DeConstructor");
}

/**
 * 识别开始
 */
void PrintResultListener::on_start_working() {
    write_log("开始识别");
}

/**
 * 检查到一句话开始
 */
void PrintResultListener::on_start() {
    write_log("检测到一句话开始");
}

/**
 * 检查到一句话结束
 */
void PrintResultListener::on_end() {
    write_log("检测到一句话结束");
}

/**
 * 临时识别结果
 *
 * json IN 含有临时结果的json
 */
void PrintResultListener::on_flush_data(const std::string &json) {
    write_log("临时结果：" + json);
}

/**
 * 一句话识别结束。
 * 未开启长语音时，表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
 *
 * json IN 含有最终结果的json
 */
void PrintResultListener::on_finish(const std::string &json) {
    write_log("识别结果：" + json);
}

void PrintResultListener::on_nlu(const std::string &json) {
    write_log("语义理解结果：" + json);
}

/**
 * 长语音识别结束，（长语音 = 多个“一句话”的识别过程）
 * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
 */
void PrintResultListener::on_long_speech_end() {
    write_log("长语音结束");
}

/**
 * 识别出错
 * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
 *
 * err_domain IN 错误领域
 * err_code IN 具体错误码
 * err_desc IN 错误描述
 * sn IN 一句话的识别过程中的logId
 */
void PrintResultListener::on_error(int err_domain, int err_code, const std::string &err_desc, const std::string &sn) {
    std::ostringstream oss;
    oss << "识别错误， err_domain=" << err_domain << " ;err_code=" << err_code << " ;err_desc=" << err_desc << " ;sn= "
        << sn;
    write_log(oss.str());
}

/**
 * 识别被用户取消
 * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
 */
void PrintResultListener::on_cancel() {
    write_log("用户取消");
}

void PrintResultListener::write_log(const std::string str) const {
    std::cout.flush();
    std::ostringstream os;
    os << "[" << asrdemo::Util::get_gmt_time() << "][" << _prefix << "] :" << str << std::endl;
    std::cout << os.str();
}

} /* namespace asrdemo */
