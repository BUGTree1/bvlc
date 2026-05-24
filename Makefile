CC = g++
CXXFLAGS = -Wall -Wextra
LIBS = -lSDL3 -lavcodec

.PHONY: run
run: ./bin/bvlc
	./bin/bvlc

./bin/bvlc: ./src/main.cpp
	$(CC) $(CXXFLAGS) $(LIBS) -o $@ $<
