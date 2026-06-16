CXX       := g++
CXX_FLAGS := -std=c++11

OUT     := build
SRC     := src
INCLUDE := include
LIB     := lib
libs    := -laria2

all: $(SRC)/spm.cpp
	cd lib/aria2 && autoreconf -i && ./configure ARIA2_STATIC=yes && make
	mv lib/aria2/build/libaria2.so lib
	mkdir build && $(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o spm $(libs) && mv spm build/


clean:
	-rm $(OUT)/*
