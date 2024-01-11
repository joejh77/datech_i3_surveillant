#soft-fp version
CXX=/home/${USER}/workspace/project/oasis/buildroot-oasis/output/oasis_nor/host/bin/arm-linux-gnueabi-g++
STRIP=/home/${USER}/workspace/project/oasis/buildroot-oasis/output/oasis_nor/host/bin/arm-linux-gnueabi-strip
#CXX=/home/${USER}/workspace/project/Oasis/tools/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-g++
#STRIP=/home/${USER}/workspace/Oasis/tools/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-strip
CXXFLAGS=-g -Wall -std=c++11 -march=armv7-a -mfloat-abi=softfp -mfpu=neon -DNDEBUG -fPIC

#hard-fp version
#CXX=/home/${USER}/workspace/Oasis/tools/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
#STRIP=CXX=/home/${USER}/workspace/Oasis/tools/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip
#CXXFLAGS=-g -Wall -std=c++11 -march=armv7-a -mfloat-abi=hard -mfpu=neon -DNDEBUG -fPIC

CXXFLAGS+=-O0 -fstack-protector-strong
#CXXFLAGS+=-O3

APP_SRC = ../datech_i3_app/

INCLUDES=-I./include -I$(APP_SRC)include -I$(APP_SRC)include/iio
LDFLAGS=-L$(APP_SRC)arm/lib

LDLIBS=-lpthread -lfreetype -lz -levdev

#gprof
#LDLIBS+=-loasis-static -loffs-direct
#CXXFLAGS+=-pg

#LDLIBS += -lAdas
#LDLIBS += -Wl,--no-as-needed -ltcmalloc_debug

#CXXFLAGS+=-fno-omit-frame-pointer -fsanitize=address
#CXXFLAGS+=-static-libasan  -static-libstdc++
#LDLIBS += -lasan

#CXXFLAGS+=-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
#CXXFLAGS+=-fno-omit-frame-pointer
#LDLIBS += -Wl,--no-as-needed -ltcmalloc

#LDLIBS += libtcmalloc.a

#LDLIBS += -lprofiler


.PHONY: clean
all: clean surveillant

stripped: surveillant
	$(STRIP) --strip-unneeded surveillant

surveillant: surveillant.cpp Makefile
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c surveillant.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(APP_SRC)bb_micom.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(APP_SRC)system_log.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(APP_SRC)sysfs.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(APP_SRC)datools.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(APP_SRC)SB_System.cpp
	
	$(CXX) $(CXXFLAGS) $(INCLUDES) *.o $(LDFLAGS) $(LDLIBS) -o $@
	
clean:
	$(RM) *.o surveillant
 
