cmake_minimum_required(VERSION 3.1)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE DEBUG)
project(asrDemo_srt)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/log)

set(CMAKE_CXX_STANDARD 11)
include_directories(include/asr/ASR include/asr/  include include/extern)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O0 -fPIC  -Wno-unknown-pragmas -D_GLIBCXX_USE_CXX11_ABI=0")
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
file(GLOB SOURCE_FILES src/*.c* src/*.h*)
add_executable(asrDemo_srt ${SOURCE_FILES})
target_link_libraries(asrDemo_srt asrdemoall jsoncpp glog rt dl pthread)