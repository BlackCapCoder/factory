CC=clang++
LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -lOpenGL
FLAGS=-std=c++20 -Wno-address-of-temporary -Wno-c++11-narrowing -O2

OBJS=$(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))


build/%.o: src/%.cpp $(wildcard src/%.h)
	$(CC) -c -o $@ $< $(FLAGS) $(LIBS)

main: $(OBJS)
	$(CC) $(FLAGS) $(LIBS) -o $@ build/*.o

run: main
	./main


