CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++11 -ggdb

OUT     := build
SRC     := src
INCLUDE := include
LIB     := lib

all: $(SRC)/spm.cpp
    $(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o spm

clean:
    -rm $(out)/*
