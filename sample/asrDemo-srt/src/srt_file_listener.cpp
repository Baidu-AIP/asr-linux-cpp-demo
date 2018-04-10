//
// Created by fu on 3/17/18.
//

#include <sstream>
#include <memory>
#include "srt_file_listener.hpp"
#include "common.h"

namespace asr_srt {
    SrtFileListener::SrtFileListener(const std::string &srt_filename) : _filename(srt_filename) {

    }

    SrtFileListener::~SrtFileListener() {
        if (_fp) {
            fclose(_fp);
            _fp = NULL;
        }
    }

    bool SrtFileListener::try_waiting_finish(std::string &error_msg) {
        ALOG << _filename << " : try_waiting_finish call";
        if (_has_error) {
            std::lock_guard<std::mutex> lk(_error_mutex);
            error_msg = _error_msg;
            return false;
        }
        {
            std::unique_lock<std::mutex> lck(_waiting_mutex);
            if (_is_finished) {
                return true;
            }
            _cond.wait_for(lck, std::chrono::seconds(TIMEOUT), [this]() { return _is_finished.load(); });
            ALOG << _filename << " : try_waiting_finish free";
            if (_has_error) {
                error_msg = _error_msg;
                return false;
            }
        }

        if (!_is_finished) {
            error_msg = "sdk not finished";
            return false;
        } else {
            return true;
        }
    }

    void SrtFileListener::on_last_status(int status) {
        ALOG << _filename << " : on_last_status call" << std::endl;
        _is_finished = true;
        fclose(_fp);
        _fp = nullptr;
        _cond.notify_all();
    }


    void SrtFileListener::on_start_working() {
        _fp = fopen(_filename.c_str(), "w");
        if (_fp == NULL) {
            write_error("file does not exist");
        }
        if (ferror(_fp) != 0) {
            write_error("ferror(_fp)");
        }
        const char bom[] = "\xef\xbb\xbf";
        if (fwrite(bom, 1, strlen(bom), _fp)!= strlen(bom)) {
            write_error("write file error");
        }
    }

    void SrtFileListener::on_finish(const std::string &json) {
        if (_has_error) {
            return;
        }
        ALOG << _filename << " : " << json;
        Json::Value value;
        std::string error_msg = "";
        bool is_success;
        is_success = parse_json(json, value, error_msg);
        if (is_success) {
            if (!value.isMember("sn_start_time") || !value.isMember("sn_end_time") ||
                !value.isMember("results_recognition")) {
                is_success = false;
            } else {
                std::string start_time = value["sn_start_time"].asString();
                format_srt_time(start_time);
                std::string end_time = value["sn_end_time"].asString();
                format_srt_time(end_time);
                std::string word = "";
                if (value["results_recognition"].size() > 0) {
                    word = value["results_recognition"][0].asString();
                }
                int count = _count.fetch_add(1);
                count++;
                std::ostringstream oss;
                oss << count << "\r\n";
                oss << start_time << " --> " << end_time << "\r\n\r\n";
                oss << word << "\r\n";
                std::string str = oss.str();
                if (fwrite(str.c_str(), 1, str.size(), _fp) != str.size()) {
                    write_error("write file error");
                }
            }
        }
        if (!is_success){
            write_error("json parse error: " + json);
        }
    }

    void SrtFileListener::on_error(int err_domain, int err_code, const std::string &err_desc,
                                   const std::string &sn) {
        std::ostringstream os;
        os << err_domain << "," << err_code << "," << err_desc << "," << sn;
        write_error(os.str());
    }

    void SrtFileListener::write_error(const std::string &error_msg) {
        std::string msg = "[" + _filename + "]" + error_msg;
        _has_error = true;
        _is_finished = true;
        ELOG << _filename << " , " << msg;
        std::lock_guard<std::mutex> lk(_error_mutex);
        _error_msg = msg;
    }

    bool SrtFileListener::parse_json(const std::string &json, Json::Value &root, std::string &error_msg) {
        Json::CharReaderBuilder rbuilder;
        const std::unique_ptr<Json::CharReader> reader(rbuilder.newCharReader());
        const char *str = json.c_str();
        return reader->parse(str, str + json.size(), &root, &error_msg);
    }

    void SrtFileListener::format_srt_time(std::string &time) {
        size_t pos = time.find(".");
        time = time.replace(pos, 1, 1, ',');
        if (pos == 5) {
            time.insert(0, "00:");
        }
    }
}