CC=g++
#gcc -shared -Wl,--allow-multiple-definition -Wl,--whole-archive libBDSpeechSDK.a -Wl,--no-whole-archive -o libBDSpeechSDK.so
INC_PATH = -I../../../include\
           -I../../../include/ASR\
           -I./include
# 程序名称
TARGET = asrDemo 
# cpp hpp源代码路径
SRC_PATH = ../src

# .o 临时目录
OBJ_PATH = obj

# 编译 libasrdemo.a
CFLAGS_LIB = $(INC_PATH) -std=c++11 -fPIC -g  -O0 -Wall -Wno-unknown-pragmas  -D__LINUX__ -D_GLIBCXX_USE_CXX11_ABI=0
LIB_SOURCE = $(wildcard $(SRC_PATH)/asrdemo/*.cpp)  
LIB_OBJECT = $(patsubst %.cpp,$(OBJ_PATH)/asrdemo/%.o,$(notdir $(LIB_SOURCE)))

# 编译 demo
CFLAGS_DEMO = $(INC_PATH) -std=c++11  -fPIC  -g  -O0  -Wall -Wno-unknown-pragmas -Wno-unused-function -D__LINUX__ -D_GLIBCXX_USE_CXX11_ABI=0
DEMO_SOURCE = $(wildcard $(SRC_PATH)/yours_*.cpp)
DEMO_OBJECT = $(patsubst %.cpp,$(OBJ_PATH)/%.o,$(notdir $(DEMO_SOURCE)))

# .a 库
COMMON_LIBS =  -lrt -ldl -lpthread
LIBS = -lBDSpeechSDK -lcurl -liconv -lz -lssl -lcrypto -luuid $(COMMON_LIBS)
OPT_LIBS_EXT=-L../../../lib -L../../../extern/lib
LIB_P=lib


$(TARGET):init lib/libasrdemo.a lib/libasrdemoall.so $(DEMO_OBJECT)
#使用lib/libasrdemo.a
#$(CC) $(DEMO_OBJECT) -o  $(TARGET)  $(OPT_LIBS_EXT) -L$(LIB_P) $(LIBS) -lasrdemo
#使用lib/libasrdemo.so
	$(CC) $(DEMO_OBJECT) -o $(TARGET) -L$(LIB_P) -Wl,-rpath,$(LIB_P) -lasrdemoall -lrt -ldl -lpthread 
	@echo
	@echo "Build Success"
	@echo "Type : ./$(TARGET)"

init:
	mkdir -p $(OBJ_PATH)/asrdemo
# lib 目录 libasrdemoall.so
	mkdir -p lib 
#  头文件 目录
	mkdir -p include/asrdemo  
	cp $(SRC_PATH)/asrdemo/*.h* include/asrdemo 

lib/libasrdemo.a:init $(LIB_OBJECT)
	ar cr lib/libasrdemo.a $(LIB_OBJECT) 
lib/libasrdemoall.so:init $(LIB_OBJECT) 
	$(CC) -shared -fPIC -g  $(LIB_OBJECT) $(OPT_LIBS_EXT) $(LIBS)  -o  lib/libasrdemoall.so

$(OBJ_PATH)/asrdemo/%.o:$(SRC_PATH)/asrdemo/%.cpp
	$(CC) $(CFLAGS_LIB) -c  $< -o $@
$(OBJ_PATH)/yours_%.o:$(SRC_PATH)/yours_%.cpp
	$(CC) $(CFLAGS_DEMO) -c  $< -o $@
clean:
	-rm -rf lib include $(OBJ_PATH) $(TARGET) BDS_Ident.dat asrdemo.log *.so
