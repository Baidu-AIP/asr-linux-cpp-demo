//
// Created by fu on 3/17/18.
//

#include "recognizer.hpp"
#include "common.h"

#define CHECK_HAS_ERROR() if (_has_error){\
    error_msg = _error_msg;\
    ELOG << _error_msg;\
    return RETURN_ERROR;\
}

namespace asr_srt {
    Recognizer::Recognizer(
            const std::string &filename
    ) : _filename(filename), _is(filename, std::ios_base::in | std::ios_base::binary),
        _listener(get_srt_filename(filename)), _controller(APP_NAME, "../asr_resource/", _listener) {
        ALOG << _filename << " :Recognizer inited";
        _error_msg = "";
        if (!_is || !_is.is_open()) {
            _has_error = true;
            _error_msg = "file open failed :" + filename;
            ELOG << _error_msg;
        }
    }

    Recognizer::~Recognizer() {
        if (_is.is_open()) {
            _is.close();
        }
    }

    RETURN_PROCESS_STATUS Recognizer::config(bds::BDSSDKMessage &config, std::string &error_msg){
        CHECK_HAS_ERROR();
        if (!_controller.config(config,_error_msg)){
            _has_error = true;
        }
        CHECK_HAS_ERROR();
        return RETURN_OK;
    }

    bool Recognizer::try_waiting_finish(std::string &error_msg) {
        return _listener.try_waiting_finish(error_msg);
    }

    RETURN_PROCESS_STATUS Recognizer::process_one_frame(std::string &error_msg) {
        CHECK_HAS_ERROR();
        if (_next_process_time > 0 && _next_process_time > asrdemo::Util::current_timestamp()) {
            return RETURN_SKIP;
        }

        if (_is_file_end) {
            return RETURN_WAITING_SDK_END;
        }
        RETURN_PROCESS_STATUS status = check_file_status();
        if (status == RETURN_FILE_END) {
            _has_error = !_controller.post_data_finish_and_stop(error_msg);
        }
        CHECK_HAS_ERROR();
        if (status != RETURN_OK) {
            return status;
        }
        return read_one_frame(error_msg);

    }

    RETURN_PROCESS_STATUS Recognizer::read_one_frame(std::string &error_msg) {
        char audio_buf[FRAME_SIZE];
        int readed_len = 0;

        _is.read(audio_buf, FRAME_SIZE);
        readed_len = _is.gcount();
        if (readed_len == 0) {
            // ELOG << "read file with length = 0 : " << _filename << std::endl;
            return RETURN_SKIP;
        }
        _has_error = !_controller.post_audio_data(audio_buf, readed_len, _error_msg);
        CHECK_HAS_ERROR();
        int duration = asrdemo::Util::cal_speech_16k_duration_ms(readed_len); //下次调用需要sleep的时间
        _next_process_time = asrdemo::Util::current_timestamp() + duration - 1; // 忽略运行1ms
        return RETURN_PROCESSING;
    }


    RETURN_PROCESS_STATUS Recognizer::check_file_status() {
        if (_has_error) {
            return RETURN_ERROR;
        }
        if (_is.bad()) {
            _has_error = true;
            _error_msg = "file meets error, _is.fail(): " + _filename;
            ELOG << _error_msg << ":" << _is.bad() << _is.fail();
            return RETURN_ERROR;
        }
        if (_is.eof()) {
            _is.close();
            _is_file_end = true;
            _next_process_time = 0;
            return RETURN_FILE_END;
        }
        return RETURN_OK;
    }

    std::string Recognizer::get_srt_filename(const std::string &filename) const {
        return filename.substr(0, filename.size() - 3) + "srt";
    }
}