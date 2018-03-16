## asr Makefile ##
CC=g++
#AR=ar

#FILE_NAME=$(src)
FILE_NAME=src/main.cpp
INC_PATH= -I../../include\
          -I../../include/ASR\

SRC_PATH=./src
OBJ_PATH=.
TARGET=$(basename $(FILE_NAME))
LIB_PATH=../../lib
EXTERN_PATH=../../extern/lib
TARGET_PATH=./

CPPFLAGS1=-Wall -O0 -fPIC -g -D__LINUX__ -Wno-unknown-pragmas -D_GLIBCXX_USE_CXX11_ABI=0
CPPFLAGS= $(CPPFLAGS1) $(LIB_PATH)/libBDSpeechSDK.a $(EXTERN_PATH)/libcurl.a $(EXTERN_PATH)/libiconv.a $(EXTERN_PATH)/libz.a $(EXTERN_PATH)/libssl.a $(EXTERN_PATH)/libcrypto.a $(EXTERN_PATH)/libuuid.a -lrt -ldl -lpthread
SRC_FILE=$(shell cd $(SRC_PATH)&&echo *.cpp)
SRC:=$(foreach n,$(SRC_FILE),$(SRC_PATH)/$(n))

OBJ_FILE=$(SRC_FILE:.cpp=.o)
OBJ:=$(foreach n,$(OBJ_FILE),$(OBJ_PATH)/$(n))
	          
DEP_FILE=$(SRC_FILE:.cpp=.d)
DEP:=$(foreach n,$(DEP_FILE),$(OBJ_PATH)/$(n))

$(TARGET):$(SRC)
	$(CC) -o $(TARGET)  ./$(FILE_NAME) $(INC_PATH) $(CPPFLAGS) 
	-mv $@ $(TARGET_PATH)
clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)

cleanall:
	-rm -f $(OBJ)
	-rm -f $(TARGET)


