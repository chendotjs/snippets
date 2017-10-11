all : MatchTemplate_Demo MatchTemplate_Demo_FITS

OPENCVLIBS=$(shell pkg-config --libs opencv)

MatchTemplate_Demo : MatchTemplate_Demo.cpp
	g++ $^ -o $@ $(OPENCVLIBS) -g

MatchTemplate_Demo_FITS : MatchTemplate_Demo_FITS.cpp
	g++ $^ -o $@ $(OPENCVLIBS) -lcfitsio -g

clean :
	rm MatchTemplate_Demo MatchTemplate_Demo_FITS
