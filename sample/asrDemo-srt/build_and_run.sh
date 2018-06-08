#!/bin/sh


#依赖：
#ffmpeg
#cmake 3.1 以上


cd scripts
#编译依赖库。运行成功一次后，该脚本可以删除。
sh build-env.sh && \
\
#提取视频内的音频文件。运行成功一次后，该脚本可以删除。
sh convert-audio.sh wmv && \
\
cd ..
\
echo " begin to build " && \
rm -rf run/* && \
mkdir -p run && \
sleep 2  && \
cd run && \
cmake .. && make -j4 && \
echo "build success, begin to run " && \
sleep 2  && \
./asrDemo_srt
