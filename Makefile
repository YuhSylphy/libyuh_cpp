#!/bin/make
#yuh

.PHONY: all clean distclean test

all: lib unittest

lib: 
	make -C src
unittest: 
	make -C test

test:
	make -C test test
clean: 
	make -C src clean
	make -C test clean
distclean: 
	make -C src distclean
	make -C test distclean


# SRC=$(shell ls problem*.cpp)
# HED=$(shell ls *.h *.hpp)
# OBJS=$(patsubst %.cpp,%.o,$(SRC))
# DEPENDS=$(patsubst %.o,%.d,$(OBJS))
# TARGET=$(patsubst %.o,%,$(OBJS))

# CC = clang
# CFLAGS = -O2 -Wall
# CXX = clang++
# INCLUDES = 
# CXXFLAGS = -O3 -Wall -std=c++11 -pthread $(INCLUDES)
# LDFLAGS = -lm -lstdc++

# .PHONY: all clean check-syntax distclean tar depend
# # .SUFFIXES:
# # .SUFFIXES: .o .c .cpp 

# all: stdafx.pch $(TARGET)

# #for auto-complete-clang
# stdafx.pch:
# 	clang++ -std=c++11 -cc1 -x c++-header stdafx.h -emit-pch -o stdafx.pch $(INCLUDES)
# #for flymake
# check-syntax:
# 	$(CXX) -Wall -Wextra -pedantic -std=c++11 $(INCLUDES) -fsyntax-only -S ${CHK_SOURCES}

# clean:
# 	$(RM) $(OBJS)
# distclean:
# 	$(RM) $(OBJS) $(DEPENDS) $(TARGET)
# tar:;
# 	tar cvf pe.tar.lzma --lzma $(SRC) $(HED) Makefile

# .cpp:
# 	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
# %.d: %.cpp
# 	$(CXX) -MM $(CXXFLAGS) $<  >  $@
# depend: $(DEPENDS)
# -include $(DEPENDS)
