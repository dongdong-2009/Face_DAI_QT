####################################################
# Generic makefile - all Makefile
# for compiling and linking C++ projects on Linux 
# Author: George Foot  Modified:Jackie Lee
####################################################
### Customising
#
# Adjust the following if necessary; EXECUTABLE is the target
# executable's filename, and LIBS is a list of libraries to link in
# (e.g. alleg, stdcx, iostr, etc). You can override these on make's
# command line of course, if you prefer to do it that way.
#
#
 # 可执行文件名
EXECUTABLE := faceServer   
 # 静态库目录
LIBDIR:=  /usr/local/lib/ 
LIBDIR += ./newlibs/ 
 # 动态库文件名
LIBS := opencv_core opencv_features2d \
        opencv_highgui opencv_imgproc \
        opencv_ml opencv_video opencv_contrib \
        opencv_objdetect  
        
LIBS += sqlite3  
LIBS += seeta_facedet_lib seeta_fa_lib viplnet

 # 头文件目录
INCLUDES:=/usr/local/include/opencv/    
INCLUDES += /usr/local/include/opencv2/      
SRCDIR:=              # 除了当前目录外，其他的源代码文件目录
#
# # Now alter any implicit rules' variables if you like, e.g.:

CC := g++
CFLAGS := -g -w -O3 -std=c++11 -lsqlite3 -lpthread -msse4.2 -lrt
CPPFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD
#
# # The next bit checks to see whether rm is in your djgpp bin
# # directory; if not it uses del instead, but this can cause (harmless)
# # `File not found' error messages. If you are not using DOS at all,
# # set the variable to something which will unquestioningly remove
# # files.
#

RM-F := rm -f


# # You shouldn't need to change anything below this point.
#
SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(SRCDIR)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES:= $(wildcard $(patsubst %.d,%.cpp,$(MISSING_DEPS)))


.PHONY: all deps objs clean veryclean rebuild info

all: $(EXECUTABLE)

deps: $(DEPS)

objs: $(OBJS)

clean:
	@$(RM-F) *.o
	@$(RM-F) *.d
        
veryclean: clean
	@$(RM-F) $(EXECUTABLE)

rebuild: veryclean all
ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
	@$(RM-F) $(patsubst %.d,%.o,$@)
endif
-include $(DEPS)
$(EXECUTABLE) : $(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))

info:
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(DEPS)
	@echo $(MISSING_DEPS)
	@echo $(MISSING_DEPS_SOURCES)
