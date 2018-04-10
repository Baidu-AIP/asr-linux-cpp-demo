//
// Created by fu on 3/28/18.
//
#include "common.h"


namespace asr_srt {
    void init_log(int argc,char* argv[]) {

        google::InitGoogleLogging(argv[0]);

        FLAGS_log_dir = "../log";
        FLAGS_max_log_size = 4; // 4 MB

        google::SetStderrLogging(google::INFO); // 同时输出到屏幕
        FLAGS_colorlogtostderr=true;
        FLAGS_stop_logging_if_full_disk = true;
        FLAGS_logbufsecs =0;        //缓冲日志输出，默认为30秒，此处改为立即输出
        google::InstallFailureSignalHandler();
    }

    void release_log(){
        google::ShutdownGoogleLogging();
    }

}