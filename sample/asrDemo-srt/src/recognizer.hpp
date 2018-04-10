//
// Created by fu on 3/17/18.
//

#ifndef ASRDEMO_SRT_RECOGNIZER_HPP
#define ASRDEMO_SRT_RECOGNIZER_HPP


#include <BDSSDKMessage.hpp>
#include <asrdemo/AsrdemoController.hpp>
#include <iostream>
#include <fstream>
#include "srt_file_listener.hpp"

namespace asr_srt {

    enum RETURN_PROCESS_STATUS {
        RETURN_OK = 0,
        RETURN_SKIP = 1,
        RETURN_PROCESSING = 2,
        RETURN_FILE_END = 3,
        RETURN_WAITING_SDK_END = 4,
        RETURN_ERROR = -10

    };

    class Recognizer {
    public:
        Recognizer(const std::string &filename);
        virtual ~Recognizer();
        RETURN_PROCESS_STATUS process_one_frame(std::string &error_msg);

        RETURN_PROCESS_STATUS config(bds::BDSSDKMessage &config, std::string &error_msg);

        uint64_t get_next_process_time() const{
            return _next_process_time;
        }

        bool try_waiting_finish(std::string &message);

        std::string get_filename() const{
            return _filename;
        }
    private:
        const std::string APP_NAME = "asr_str";

        const int FRAME_SIZE = 320; // 10ms

        bool _has_error = false;
        std::string _error_msg = "";
        std::string _filename;
        std::ifstream _is;
        SrtFileListener _listener;
        asrdemo::AsrdemoController _controller;


        bool _is_file_end = false;
        uint64_t _next_process_time = 0;

        std::string get_srt_filename(const std::string &filename) const;

        RETURN_PROCESS_STATUS check_file_status();

        RETURN_PROCESS_STATUS read_one_frame(std::string &error_msg);

    };

}
#endif //ASRDEMO_SRT_RECOGNIZER_HPP
