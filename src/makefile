CC=g++
CFLAGS=-std=c++11 -ggdb `pkg-config --cflags --libs opencv`

all: drats

drats: main.cpp 
	$(CC) $(CFLAGS) main.cpp -o drats

clean: 
	rm drats

