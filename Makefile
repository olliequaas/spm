CXX       := g++
CXX_FLAGS := -std=c++11

OUT     := build
SRC     := src
INCLUDE := include
LIB     := lib
libraries    := -laria2

all: spm libaria2

libaria2:
	cd lib/aria2 && autoreconf -i && ./configure ARIA2_STATIC=yes && make
	mv lib/aria2/build/libaria2.so lib

spm: $(SRC)/spm.cpp
	mkdir build && $(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o spm $(libraries) && mv spm build/

clean:
	-rm $(OUT)/*
