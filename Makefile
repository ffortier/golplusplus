.PHONY: all clean run

all: clean run

clean:
	rm -rf build

run: build/gol++

build/gol++:
	mkdir -p build
	g++ -Wall -std=c++11 src/*.cpp -lSDL2 -o build/gol++

run:
	./build/gol++
