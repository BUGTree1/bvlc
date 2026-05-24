CC = g++
CXXFLAGS = -O3 -Wall -Wextra
LIBS = -lSDL3 -lavformat -lavcodec -lavutil -lavdevice -lswscale

.PHONY: run
run: ./bin/bvlc
	./bin/bvlc video.mp4 frame.png

./bin/bvlc: ./src/main.cpp
	$(CC) $(CXXFLAGS) -o $@ $< $(LIBS)
