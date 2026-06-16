CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++11 -ggdb

OUT     := build
SRC     := src
INCLUDE := include
LIB     := lib

all: $(SRC)/spm.cpp
    cd lib/aria2
    autoreconf -i
    ./configure ARIA2_STATIC=yes
    make
    cd -
    mv lib/aria2/build/libaria2.so lib
    $(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o spm

clean:
    -rm $(out)/*
