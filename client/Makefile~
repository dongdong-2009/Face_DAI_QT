#Makefile
#author:jiajunjie

CROSS_COMPILE := /home/libceb/1234/sdk/toolchain/sysroots/x86_64-tanxunsdk-linux/usr/bin/arm-tanxun-linux-gnueabi/arm-tanxun-linux-gnueabi-
CPP := $(CROSS_COMPILE)g++
OBJS:= main.o global.o facedetect.o network.o config.o

INCLUDES:=  -I /usr/local/include/opencv/ 
INCLUDES += -I /usr/local/include/opencv2/ 
INCLUDES += -I /home/libceb/1234/sdk/toolchain/sysroots/cortexa9hf-vfp-neon-tanxun-linux-gnueabi/usr/include 

LIBS:=  -rdynamic ./lib/libopencv_stitching.so.2.4.11 ./lib/libopencv_superres.so.2.4.11 ./lib/libopencv_videostab.so.2.4.11 ./lib/libopencv_features2d.so.2.4.11 ./lib/libopencv_flann.so.2.4.11 ./lib/libopencv_objdetect.so.2.4.11 ./lib/libopencv_ml.so.2.4.11 ./lib/libopencv_highgui.so.2.4.11 ./lib/libopencv_photo.so.2.4.11 ./lib/libopencv_imgproc.so.2.4.11 ./lib/libopencv_core.so.2.4.11 -Wl,-rpath=./lib

CFLAGS:= -w -std=c++11 -O2 
CFLAGS += -march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=/home/libceb/1234/sdk/toolchain/sysroots/cortexa9hf-vfp-neon-tanxun-linux-gnueabi 

all:faceClient

faceClient:$(OBJS)
	$(CPP) $(OBJS) -o $@ $(INCLUDES)$(CFLAGS)$(LIBS) 

main.o:main.cpp
	$(CPP) $(INCLUDES)$(CFLAGS) -c main.cpp

global.o:global.cpp
	$(CPP) $(INCLUDES)$(CFLAGS) -c global.cpp

facedetect.o:facedetect.cpp
	$(CPP) $(INCLUDES)$(CFLAGS) -c facedetect.cpp

network.o:network.cpp
	$(CPP) $(INCLUDES)$(CFLAGS) -c network.cpp

config.o:config.cpp
	$(CPP) $(INCLUDES)$(CFLAGS) -c config.cpp

clean:
	rm -f *.o  faceClient

distclean:
	rm -f *.o *.d faceClient
	
help:
	@echo "Generic Makefile for C Programs (gcmakefile) version 1.0"
	@echo 'Copyright (C) 2012, 2012 blog.jia0511.net <jia0511@163.com>'
	@echo 'Author: jiajunjie'
	@echo 'Usage: make [TARGET]'
	@echo 'TARGETS:'
	@echo 'all       (=make) compile and link.'
	@echo 'clean     clean objects and the executable file.'
	@echo 'distclean clean objects, the executable and dependencies.'
