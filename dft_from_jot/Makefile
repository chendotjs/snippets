.PHONY : all

FFTWLIBS=$(shell pkg-config --libs fftw3)

all : simplest_dft fftw1 fftw2

simplest_dft : simplest_dft.cpp
	g++ $^ -o $@

fftw1 : fftw1.c
	gcc $^ -o $@ $(FFTWLIBS) -lm

fftw2 : fftw2.c
	gcc $^ -o $@ $(FFTWLIBS) -lm
