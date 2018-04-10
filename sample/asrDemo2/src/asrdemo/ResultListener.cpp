/*
 * asrdemo_ResultListener.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: fu
 */

#include "ResultListener.hpp"
#include <iostream>

namespace asrdemo {

ResultListener::ResultListener() {
    //std::cout<< "result_listener constructor"<<std::endl;
}

ResultListener::~ResultListener() {
}

void ResultListener::output_callback(bds::BDSSDKMessage &message, int status) {
    switch (status) {
        case bds::EVoiceRecognitionClientWorkStatusStartWorkIng: {
            // 识别工作开始
            on_start_working();
            break;
        }

        case bds::EVoiceRecognitionClientWorkStatusStart: {
            // 检测到开始说话, 一句话开始
            on_start();
            break;
        }

        case bds::EVoiceRecognitionClientWorkStatusEnd: {
            // 检测到说话结束，一句话结束
            on_end();
            break;
        }

        case bds::EVoiceRecognitionClientWorkStatusFlushData: {
            // 连续上屏, 临时识别结果
            std::string json_result;
            message.get_parameter(bds::CALLBACK_ASR_RESULT, json_result);
            on_flush_data(json_result);
            break;
        }

        case bds::EVoiceRecognitionClientWorkStatusFinish: {
            //最终结果
            std::string json_result;
            message.get_parameter(bds::CALLBACK_ASR_RESULT, json_result);
            on_finish(json_result);
            break;
        }

        case bds::EVoiceRecognitionClientWorkStatusChunkNlu: {
            //语义解析
            const char *buf;
            int len = 0;
            message.get_parameter(bds::DATA_CHUNK, buf, len);
            std::string result(buf, len);
            on_nlu(result);
            break;
        }
        //case bds::EVoiceRecognitionClientWorkStatusChunkThirdData: {    //第三方结果
        //    const char* buf;
        //    int len = 0;
        //    message.get_parameter(bds::DATA_CHUNK, buf, len);
        //    //第三方结果未必是文本字符串，所以以%s打印未必有意义
        //    fprintf(result_output_file, "third final result len[%d]\n", len);
        //    //for (int i = 0; i < len; ++i) fprintf(result_output_file, "%c", buf[i]);
        //    fprintf(result_output_file, "\n");
        //    fflush(result_output_file);
        //    break;
        //}

        case bds::EVoiceRecognitionClientWorkStatusLongSpeechEnd: { // 长语音结束状态
            on_long_speech_end();
            on_last_status(status);
            break;
        }

        case bds::EVoiceRecognitionClientWorkStatusError: {
            // 识别出错
            int err_code = 0;
            int err_domain = 0;
            std::string err_desc;
            message.get_parameter(bds::CALLBACK_ERROR_CODE, err_code);
            message.get_parameter(bds::CALLBACK_ERROR_DOMAIN, err_domain);
            message.get_parameter(bds::CALLBACK_ERROR_DESC, err_desc);

            std::string sn;
            message.get_parameter(bds::CALLBACK_ERROR_SERIAL_NUM, sn);

            on_error(err_domain, err_code, err_desc, sn);
            on_last_status(status);
            break;
        }

        case bds::EVoiceRecognitionClientWorkStatusCancel: {
            //用户取消
            on_cancel();
            on_last_status(status);
            break;
        }

        case bds::EVoiceRecognitionClientWorkStatusNewRecordData:
        case bds::EVoiceRecognitionClientWorkStatusMeterLevel:
            break;
        default: {
            //fprintf(result_output_file, "new status： %d\n",status);
            break;
        }
    }
}


/**
 * 识别开始
 */
void ResultListener::on_start_working() {

}

/**
 * 检查到一句话开始
 */
void ResultListener::on_start() {

}

/**
 * 检查到一句话结束
 */
void ResultListener::on_end() {

}

/**
 * 临时识别结果
 *
 * json IN 含有临时结果的json
 */
void ResultListener::on_flush_data(const std::string &json) {

}

/**
 * 语义理解结果
 */
void ResultListener::on_nlu(const std::string &json) {

}

/**
 * 识别被用户取消
 * 表示整个识别过程结束，BDSpeechSDK实例处于空闲状态
 */
void ResultListener::on_cancel() {

}


void ResultListener::on_long_speech_end() {

}

void ResultListener:: on_last_status(int status){

}


} /* namespace asrdemo */
