CC=g++
CLAGS=-ggdb `pkg-config --cflags --libs opencv`

all: drats

drats: main.o liveSurf.o
	$(CC) main.o liveSurf.o -o drats

main.o: main.cpp
	$(CC) $(CLAGS) main.cpp

liveSurf.o: liveSurf.cpp
	$(CC) $(CFLAGS) liveSurf.cpp

clean: 
	rm *o drats

