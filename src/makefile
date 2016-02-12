CC=g++
CFLAGS=-std=c++11 -ggdb `pkg-config --cflags --libs opencv`

all: crats drats test calibration

crats: main.cpp 
	$(CC) $(CFLAGS) main.cpp -o crats

drats: mainProc.cpp
	$(CC) $(CFLAGS) mainProc.cpp -o drats

test: test.cpp
	$(CC) $(CFLAGS) test.cpp -o test

calibration: camera_calibration.cpp
	$(CC) $(CFLAGS) camera_calibration.cpp -o calibration

clean: 
	rm crats drats test

