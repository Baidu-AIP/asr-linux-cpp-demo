# asr-linux-cpp-demo
## 简介

Linux C++ SDK 仅有在线长语音功能，包含在线语义。没有任何离线功能。



- 仅支持x64 linux 操作系统，　g++ 4.8 以上版本

在centos 4 6 7 及ubuntu 14.04，　g++ 4.8 版本测试通过

**SDK内部限制１０个线程并发，即最多支持１０个音频的实时识别。 **



## 运行

sample目录下有三个实例工程

sample/asr 运行 sh build_and_run.sh
sample/asrDemo2 下 run目录中运行 sh build_and_run.sh 。同时也是为Clion工程
sample/asrDemo-srt 阅读该目录下的readme文件，再运行

## 完整文档

http://ai.baidu.com/docs#/ASR-Linux-SDK/top

## 项目地址

https://github.com/Baidu-AIP/asr-linux-cpp-demo
