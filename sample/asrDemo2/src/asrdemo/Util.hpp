/*
 * common_Util.hpp
 *
 *  Created on: Dec 13, 2017
 *      Author: fu
 */

#ifndef ASRDEMO_UTIL_HPP_
#define ASRDEMO_UTIL_HPP_

#include <stdint.h>
#include <sys/time.h>
#include <string>

#include "BDSSDKMessage.hpp"

namespace asrdemo {

class Util {
public:
    /**
     * 转为 BDSSDKMessage 为可见的字符串
     */
    static std::string params_to_string(bds::BDSSDKMessage &params);

    /**
     * 当前的timestamp 毫秒
     */
    static uint64_t current_timestamp();

    /**
     * 8k采样率的原始音频对应的时长 毫秒
     */
    static uint64_t cal_speech_8k_duration_ms(int bytes);

    /**
    * 16k采样率的原始音频对应的时长 毫秒
    */
    static uint64_t cal_speech_16k_duration_ms(int bytes);

    /**
     * 当前时间字符串
     */
    static std::string get_gmt_time();

private:
    /**
     * 原始音频对应的时长
     */
    static uint64_t cal_speech_duration_ms(int bytes, int sample_rate);
};

} /* namespace asrdemo */

#endif /* ASRDEMO_UTIL_HPP_ */
