CC=g++
CFLAGS=-std=c++11 -ggdb `pkg-config --cflags --libs opencv`

all: crats drats

crats: main.cpp 
	$(CC) $(CFLAGS) main.cpp -o crats

drats: mainProc.cpp
	$(CC) $(CFLAGS) mainProc.cpp -o drats

clean: 
	rm crats drats

