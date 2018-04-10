cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE DEBUG)
project(asrDemo2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O0 -fPIC -D__LINUX__ -Wno-unknown-pragmas -D_GLIBCXX_USE_CXX11_ABI=0 -Wno-unused-function")
file(GLOB SOURCE_FILES "src/*.c*" "src/*/*.cpp")

include_directories(../../extern/include ../../include ../../include/ASR)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../lib ${CMAKE_CURRENT_SOURCE_DIR}/../../extern/lib)
add_executable(asrDemo2 ${SOURCE_FILES})
target_link_libraries(asrDemo2 BDSpeechSDK curl iconv z ssl crypto uuid rt dl pthread)