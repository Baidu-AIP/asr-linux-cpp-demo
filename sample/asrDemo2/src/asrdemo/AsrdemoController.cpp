/*
 * BdSpeech.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: fu
 */
#include "AsrdemoController.hpp"
#include <iostream>

namespace asrdemo {

#define CHECK_ERROR_THEN_RETURN(MSG) do{\
    if (_has_error) {\
        error_msg = MSG;\
        error_msg += ": ";\
        error_msg += _error_msg;\
        release();\
        return false;\
    }\
}while(0);

AsrdemoController::AsrdemoController(const std::string &app_name, const std::string &resource_path,
                                     ResultListener &listener) :
        _app_name(app_name), _resource_path(resource_path), _listener(listener), _is_configed(false),
        _has_error(false) {
    _sdk = bds::BDSpeechSDK::get_instance(bds::SDK_TYPE_ASR, _error_msg);
    if (_sdk == nullptr) {
        _has_error = true;
        _error_msg = "BDSpeechSDK is NULL;";
    }
    _push_params.name = bds::ASR_CMD_PUSH_AUDIO;
}

AsrdemoController::~AsrdemoController() {
    release();
}

bool AsrdemoController::config(bds::BDSSDKMessage &config_params, std::string &error_msg) {
    if (_is_configed) {
        _has_error = true;
        _error_msg = "bd_speech_control_impl has already initialized";
    }
    if (_is_finished || _sdk == nullptr){
        _has_error = true;
        _error_msg = "bd_speech_control_impl has already initialized";
    }
    CHECK_ERROR_THEN_RETURN("sdk is finished");
    _sdk->set_event_listener(&AsrdemoController::output_callback, (void *)this);
    CHECK_ERROR_THEN_RETURN("BDSpeechSDK init ERROR");
    set_config_params(config_params);
    CHECK_ERROR_THEN_RETURN("set_config_params ERROR");
    set_start_params();
    CHECK_ERROR_THEN_RETURN("set_start_params ERROR");

    _is_configed = true;
    return true;
}

bool AsrdemoController::post_audio_data(const char *audio_buf, int audio_buf_len, std::string &error_msg) {
    if (_is_finished){
        error_msg = " sdk is finished and released";
        return false;
    }
    _push_params.set_parameter(bds::DATA_CHUNK, audio_buf, audio_buf_len);
    _has_error = !_sdk->post(_push_params, _error_msg);
    CHECK_ERROR_THEN_RETURN("post_audio_data ERROR");
    return true;
}

bool AsrdemoController::post_data_finish_and_stop(std::string &error_msg) {
    if (!post_audio_data(NULL, 0, error_msg)) {
        return false;
    }
    return stop(error_msg);
}

bool AsrdemoController::cancel(std::string &error_msg) {
    if (_is_finished){
        error_msg = " sdk is finished and released";
        return false;
    }
    bds::BDSSDKMessage cancel_params;
    cancel_params.name = bds::ASR_CMD_CANCEL;
    _has_error = !_sdk->post(cancel_params, _error_msg);
    CHECK_ERROR_THEN_RETURN("post_audio_data ERROR");
    return true;
}

bool AsrdemoController::stop(std::string &error_msg) {
    if (_is_finished){
        error_msg = " sdk is finished and released";
        return false;
    }
    bds::BDSSDKMessage stop_params;
    stop_params.name = bds::ASR_CMD_STOP;
    _has_error = !_sdk->post(stop_params, _error_msg);
    CHECK_ERROR_THEN_RETURN("post_audio_data ERROR");
    return true;
}

void AsrdemoController::output_callback(bds::BDSSDKMessage &message, void *user_arg) {
    if (message.name != bds::asr_callback_name) {
        std::cerr << " message.name not correct:" + message.name << std::endl;
        return;
    }
    int status = 0;

    if (!message.get_parameter(bds::CALLBACK_ASR_STATUS, status)) {
        std::cerr << " message get status failed:" << std::endl;
        return;
    }

    AsrdemoController *controller = (AsrdemoController *) user_arg;
    switch (status){
        case bds::EVoiceRecognitionClientWorkStatusCancel:
        case bds::EVoiceRecognitionClientWorkStatusError:
        case bds::EVoiceRecognitionClientWorkStatusLongSpeechEnd:
            controller->release();
            break;
    }
    controller->_listener.output_callback(message, status);

}

void AsrdemoController::set_config_params(bds::BDSSDKMessage &config_params) {
    config_params.name = bds::ASR_CMD_CONFIG;
    std::string temp_str;

    // 校验必填字段
    const std::string required_string_fields[] = {bds::ASR_PARAM_KEY_APP_ID,
                                                  bds::ASR_PARAM_KEY_CHUNK_KEY,
                                                  bds::ASR_PARAM_KEY_SECRET_KEY,
                                                  bds::ASR_PARAM_KEY_PRODUCT_ID};
    std::string key;
    for (int i = 0; i != sizeof(required_string_fields) / sizeof(std::string); ++i) {
        key = required_string_fields[i];
        if (!config_params.get_parameter(key, temp_str)) {
            _has_error = true;
            _error_msg = key + " is not set ";
            return;
        }
    }

    // 强制设置字段
    config_params.set_parameter(bds::ASR_PARAM_KEY_CHUNK_ENABLE, 1);
    config_params.set_parameter(bds::ASR_PARAM_KEY_ENABLE_LONG_SPEECH, 1);

    // 补填字段
    if (!config_params.get_parameter(bds::ASR_PARAM_KEY_MFE_DNN_DAT_FILE, temp_str)) {
        config_params.set_parameter(bds::ASR_PARAM_KEY_MFE_DNN_DAT_FILE,
                                    _resource_path + "bds_easr_mfe_dnn.dat");
    }
    if (!config_params.get_parameter(bds::ASR_PARAM_KEY_MFE_CMVN_DAT_FILE, temp_str)) {
        config_params.set_parameter(bds::ASR_PARAM_KEY_MFE_CMVN_DAT_FILE,
                                    _resource_path + "bds_easr_mfe_cmvn.dat");
    }
    if (!config_params.get_parameter(bds::ASR_PARAM_KEY_COMPRESSION_TYPE, temp_str)) {
        config_params.set_parameter(bds::ASR_PARAM_KEY_COMPRESSION_TYPE,
                                    bds::EVR_AUDIO_COMPRESSION_PCM);
    }
    int tmp_int;
    if (!config_params.get_parameter(bds::COMMON_PARAM_KEY_DEBUG_LOG_LEVEL, tmp_int)) {
        config_params.set_parameter(bds::COMMON_PARAM_KEY_DEBUG_LOG_LEVEL,
                                    bds::EVRDebugLogLevelTrace);
    }
    _has_error = !_sdk->post(config_params, _error_msg);
}

void AsrdemoController::set_start_params() {
    bds::BDSSDKMessage start_params;
    start_params.name = bds::ASR_CMD_START;
    start_params.set_parameter(bds::ASR_PARAM_KEY_APP, _app_name);
    start_params.set_parameter(bds::ASR_PARAM_KEY_PLATFORM, "linux");
    start_params.set_parameter(bds::ASR_PARAM_KEY_SDK_VERSION, "LINUX TEST");
    _has_error = !_sdk->post(start_params, _error_msg);
}

void AsrdemoController::release() {
    std::lock_guard<std::mutex> lk(_finish_mutex);
    _is_finished = true;
    if (_sdk != nullptr) {
        bds::BDSpeechSDK::release_instance(_sdk);
        _sdk = nullptr; // 注意 如果多次调用release方法，注意 _sdk的读取需要线程同步
    }
}


#undef CHECK_ERROR_THEN_RETURN

} /* namespace asrdemo */
