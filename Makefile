main: main.o mcTranslator.o speechAssist.o
	g++ -std=c++11 main.o mcTranslator.o speechAssist.o -o main

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

mcTranslator.o: mcTranslator.cpp mcTranslator.h
	g++ -std=c++11 -c mcTranslator.cpp

speechAssist.o: speechAssist.cpp speechAssist.h 
	g++ -std=c++11 -c speechAssist.cpp

clean:
	rm  *.o main
