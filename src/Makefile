#!/bin/make
#yuh/src

SRC=$(shell ls *.cpp | grep -v flymake)
OBJS=$(patsubst %.cpp,%.o,$(SRC))
DEPENDS=$(patsubst %.o,%.d,$(OBJS))

TARGET=../libs/libyuh.a

CC = clang
CFLAGS = -O2 -Wall
CXX = clang++
INCLUDES = 
CXXFLAGS = -O3 -Wall -std=c++11 -pthread $(INCLUDES)
LDFLAGS = -lm -lstdc++

.PHONY: all clean check-syntax distclean tar depend
# .SUFFIXES:
# .SUFFIXES: .o .c .cpp 

all: stdafx.pch $(TARGET)

#for auto-complete-clang
stdafx.pch:
	clang++ -std=c++11 -cc1 -x c++-header stdafx.h -emit-pch -o stdafx.pch $(INCLUDES)
#for flymake
check-syntax:
	$(CXX) -Wall -Wextra -pedantic -std=c++11 $(INCLUDES) -fsyntax-only -S ${CHK_SOURCES}

clean:
	$(RM) $(OBJS)
distclean:
	$(RM) $(OBJS) $(TARGET) $(DEPENDS)

$(TARGET): $(OBJS)
	ar rs $(TARGET) $(OBJS)

.cpp:
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
%.d: %.cpp
	$(CXX) -MM $(CXXFLAGS) $<  >  $@
depend: $(DEPENDS)
-include $(DEPENDS)
