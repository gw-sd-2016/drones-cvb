CC=g++
CFLAGS=-ggdb `pkg-config --cflags --libs opencv`

all: drats

drats: main.cpp 
	$(CC) $(CFLAGS) main.cpp -o drats

clean: 
	rm *o drats

