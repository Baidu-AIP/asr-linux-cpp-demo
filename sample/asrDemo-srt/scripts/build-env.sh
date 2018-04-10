#!/bin/sh



cd .. && \
SO_DIR=`pwd`/../asrDemo2/run && \
rm -rf lib include asr_resource && \
mkdir -p lib include/extern include/asr include/asrdemo asr_resource && \
\
echo "begin to build libasrdemoall.so" && echo ""  && \
sleep 3 && \
cd $SO_DIR && \
make clean && make lib/libasrdemoall.so && \
cd - && \
ln -s $SO_DIR/lib/libasrdemoall.so lib/libasrdemoall.so && \
cp -r $SO_DIR/include/asrdemo/* include/asrdemo && \
cp -r ../../include/* include/asr && \
cp -r ../../extern/include/* include/extern && \
cp -r ../../resources/asr_resource/* asr_resource && \
\
echo "begin to build glog" && \
sleep 3 && \
cd lib-src && rm -rf glog-0.3.5 && tar xzf glog-0.3.5.tar.gz && \
cp glogCMakeLists.txt glog-0.3.5/CMakeLists.txt &&\
cd glog-0.3.5 &&  cmake . && make -j4 && \
pwd && cp -d -f *.so*  ../../lib && \
cp -r -d -f ./glog ../../include/extern && \
cp -r -d -f ./src/glog/*.h ../../include/extern/glog && \
cd ../.. && \
echo "" && \
\
echo "begin to build jsoncpp" && \
sleep 3 && \
cd lib-src && rm -rf jsoncpp-1.8.4 && tar xzf jsoncpp-1.8.4.tar.gz && \
cp jsoncppCMakeLists.txt jsoncpp-1.8.4/CMakeLists.txt && cd jsoncpp-1.8.4 && \
cmake . && make -j4 && \
cp -d src/lib_json/*.so* ../../lib && \
cp -r -d include/json ../../include/extern && \
cd ../.. && \
echo "" && \
echo "build env finished"

 
