#########################################################################
## Function: makefile for YUVProcessKit project
## Platform: linux, macOS
## Author: 	 lipeng
## Modified: 2021.7.2 created
#########################################################################

platform ?= x86_64
PUREC ?= 1
DEBUG ?= 0

#包含编译配置(系统或架构相关宏以及编译链接选项等配置)
include config.mk

#指定后缀名和伪目标
.SUFFIXES: .c,.o,.cpp,.S,.s  
.PHONY: all,clean

#设置相关路径
vpath %.h ./../../src
ROOTSRC  = ./../Projects
INCLUDES = ./../Projects

#设置工具的相关参数
BIN_TARGET = $(BIN_COMBINER) $(BIN_COMPARE) $(BIN_CONNECT) $(BIN_EXTRACT) $(BIN_SCALE) $(BIN_SPLIT)
BIN_COMBINER = ./../bin/$(OS)/CombinerYUV
BIN_COMPARE  = ./../bin/$(OS)/CompareYUV
BIN_CONNECT  = ./../bin/$(OS)/ConnectYUV
BIN_EXTRACT  = ./../bin/$(OS)/ExtractYUV
BIN_SCALE	 = ./../bin/$(OS)/ScaleYUV
BIN_SPLIT    = ./../bin/$(OS)/SplitYUV

#可执行文件目录
BIN_DIR = ./../bin/$(OS)

#设置编译的相关参数
CFLAGS := -Wall -std=c99 -fno-stack-protector  $(DEBUG_FLAGS) -I$(INCLUDES)
CFLAGS += $(EXTRA_CFLAGS) 

CXXFLAGS := -Wall  $(DEBUG_FLAGS) -I$(INCLUDES)
CXXFLAGS += $(EXTRA_CFLAGS)

LDFLAGS := -Wall
LDFLAGS += $(EXTRA_LFLAGS)
 
ARFLAGS := -crus

ASMFLAGS += $(EXTRA_AFLAGS)

#以下获取OBJS文件的方式适用于含有少量文件的编译
SRCS1 = $(ROOTSRC)/CombinerYUV/CombinerYUV/combinerYUV.cpp
SRCS2 = $(ROOTSRC)/CompareYUV/CompareYUV/compare.cpp
SRCS3 = $(ROOTSRC)/connectYUV/connectYUV/connectYUV.cpp
SRCS4 = $(ROOTSRC)/extractYUV/extractYUV/extractYUVdemo.cpp
SRCS5 = $(ROOTSRC)/ScaleYUV/ScaleYUV/scale.cpp
SRCS6 = $(ROOTSRC)/splitYUV/splitYUV/splitYUV.cpp
	   
#OBJS = $(patsubst %.S,%.o, $(patsubst %.asm,%.o, $(patsubst %.cpp,%.o, $(patsubst %.c,%.o,$(SRCS)))))
OBJS1 = $(ROOTSRC)/CombinerYUV/CombinerYUV/combinerYUV.o
OBJS2 = $(ROOTSRC)/CompareYUV/CompareYUV/compare.o
OBJS3 = $(ROOTSRC)/connectYUV/connectYUV/connectYUV.o
OBJS4 = $(ROOTSRC)/extractYUV/extractYUV/extractYUVdemo.o
OBJS5 = $(ROOTSRC)/ScaleYUV/ScaleYUV/scale.o
OBJS6 = $(ROOTSRC)/splitYUV/splitYUV/splitYUV.o
#编译规则
all: clean mkdr $(BIN_TARGET)
mkdr: #自动创建库目录LIB_DIR
	if [ ! -d $(BIN_DIR) ];then mkdir -p $(BIN_DIR); echo "$(BIN_DIR) create successfully!"; else echo "$(BIN_DIR) already exists!"; fi

$(BIN_COMBINER): $(OBJS1)
#	$(error $(SRCS))
	$(CC) -o $@ $(OBJS1) $(LDFLAGS)
$(BIN_COMPARE): $(OBJS2)
	$(CC) -o $@ $(OBJS2) $(LDFLAGS)
$(BIN_CONNECT): $(OBJS3)
	$(CC) -o $@ $(OBJS3) $(LDFLAGS)
$(BIN_EXTRACT): $(OBJS4)
	$(CC) -o $@ $(OBJS4) $(LDFLAGS)
$(BIN_SCALE): $(OBJS5)
	$(CC) -o $@ $(OBJS5) $(LDFLAGS)
$(BIN_SPLIT): $(OBJS6)
	$(CC) -o $@ $(OBJS6) $(LDFLAGS)
	
%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@
		
%.o:%.asm
	$(ASM) $< $(ASMFLAGS) -o $@	
%.o:%.S
	$(ASM) $< $(ASMFLAGS) -c -o $@
%.o:%.s
	$(ASM) $< $(ASMFLAGS) -c -o $@	
		
clean:
	-rm	$(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6)
	-rm $(BIN_TARGET)
