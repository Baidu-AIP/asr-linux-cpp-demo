/*
 * common_Util.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: fu
 */

#include "Util.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <chrono>

namespace asrdemo {
template<typename Type>
static void print_values(bds::BDSSDKMessage &params, std::vector<std::string> &keys,
                         Type tmp_value, std::stringstream &stream) {
    std::string k;
    for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); ++it) {
        k = (*it);
        params.get_parameter(k, tmp_value);

        stream << k << "=" << tmp_value << "; ";
    }
    stream << std::endl;
}

std::string Util::params_to_string(bds::BDSSDKMessage &params) {
    std::stringstream stream;
    if (!params.name.empty()) {
        stream << "name: " << params.name << std::endl;
    }
    std::vector<std::string> param_keys = params.string_param_keys();
    if (param_keys.size() > 0) {
        stream << "string: ";
        std::string tmp_value;
        print_values(params, param_keys, tmp_value, stream);
    }
    param_keys = params.int_param_keys();
    if (param_keys.size() > 0) {
        stream << "int: ";
        int tmp_value = 0;
        print_values(params, param_keys, tmp_value, stream);
    }

    param_keys = params.float_param_keys();
    if (param_keys.size() > 0) {
        stream << "float: ";
        float tmp_value = 0;
        print_values(params, param_keys, tmp_value, stream);
    }

    return stream.str();
}

uint64_t Util::current_timestamp() {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

std::string Util::get_gmt_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm current_time;
    localtime_r(&tv.tv_sec, &current_time);

    int year = (1900 + current_time.tm_year);
    int month = (1 + current_time.tm_mon);
    int day = current_time.tm_mday;
    int hour = current_time.tm_hour;
    int minute = current_time.tm_min;
    int sec = current_time.tm_sec;
    int msec = (int) (tv.tv_usec / 1000);

    char time_ch_buf[128] = {0};
    snprintf(time_ch_buf, sizeof(time_ch_buf) / sizeof(char),
             "%d-%02d-%02d %02d:%02d:%02d.%03d", year, month, day, hour, minute, sec, msec);
    return std::string(time_ch_buf);
}

uint64_t Util::cal_speech_duration_ms(int bytes, int sample_rate) {
    const int sample_point_bytes = 2;
    int duration_ms = bytes / (sample_point_bytes * sample_rate / 1000);
    return duration_ms;
}

uint64_t Util::cal_speech_8k_duration_ms(int bytes) {
    return Util::cal_speech_duration_ms(bytes, 8000);
}

uint64_t Util::cal_speech_16k_duration_ms(int bytes) {
    return Util::cal_speech_duration_ms(bytes, 16000);
}
} /* namespace asrdemo */
