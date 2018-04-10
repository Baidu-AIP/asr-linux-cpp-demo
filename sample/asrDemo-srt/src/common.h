//
// Created by fu on 3/17/18.
//

#ifndef ASRDEMO_SRT_COMMON_H
#define ASRDEMO_SRT_COMMON_H

#include <iostream>
#include <asrdemo/Util.hpp>
#include <glog/logging.h>
// #define _LOG_COMMON "[" << asrdemo::Util::get_gmt_time() << "][" << __FILE__ << ":" << __LINE__ << "]  "
#define ALOG  LOG(INFO)
#define ELOG  LOG(ERROR)
namespace asr_srt {
    void init_log(int argc, char* argv[]);
    void release_log();
}

#endif //ASRDEMO_SRT_COMMON_H