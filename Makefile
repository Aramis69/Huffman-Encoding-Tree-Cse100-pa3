# A simple makefile for CSE 100 P3

CC=g++
CXXFLAGS=-std=c++11 -g
LDFLAGS=-g

all: compress uncompress

compress: HCNode.o HCTree.o BitInputStream.o BitOutputStream.o 

uncompress: HCNode.o HCTree.o BitInputStream.o BitOutputStream.o

HCTree.o: HCNode.hpp HCTree.hpp BitInputStream.hpp BitOutputStream.hpp 

HCNode.o: HCNode.hpp 

BitOutputStream.o: BitOutputStream.hpp

BitInputStream.o: BitInputStream.hpp

clean:
	rm -f compress uncompress *.o core*
