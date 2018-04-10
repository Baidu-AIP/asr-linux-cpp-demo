//
// Created by fu on 3/17/18.
//

#ifndef ASRDEMO_SRT_SRT_FILE_LISTENER_HPP
#define ASRDEMO_SRT_SRT_FILE_LISTENER_HPP


#include <fstream>
#include <asrdemo/StatusListener.hpp>
#include <mutex>
#include <condition_variable>
#include <json/json.h>

namespace asr_srt {
    class SrtFileListener : public asrdemo::ResultListener {
    public:
        SrtFileListener(const std::string &srt_filename);

        virtual ~SrtFileListener();

        bool try_waiting_finish(std::string &error_msg);

    protected:
        virtual void on_start_working();

        virtual void on_finish(const std::string &json);

        virtual void on_error(int err_domain, int err_code, const std::string &err_desc, const std::string &sn);

        virtual void on_last_status(int status);

    private:
        const int TIMEOUT = 3; // 3s内结束
        FILE *_fp = nullptr;
        std::string _filename;
        std::atomic_bool _has_error = ATOMIC_VAR_INIT(false);
        std::string _error_msg;
        std::mutex _error_mutex;
        std::mutex _waiting_mutex;
        std::condition_variable _cond;
        std::atomic_bool _is_finished = ATOMIC_VAR_INIT(false);
        std::atomic_int _count = ATOMIC_VAR_INIT(0);

        void write_error(const std::string &error_msg);

        bool parse_json(const std::string &json, Json::Value &root, std::string &error_msg);

        void format_srt_time(std::string &time);
    };
}


#endif //ASRDEMO_SRT_SRT_FILE_LISTENER_HPP
