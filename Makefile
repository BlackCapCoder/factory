build:
	g++ -fconcepts -fpermissive -mavx -std=c++20 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -O2 *.cpp -o main

run: build
	./main
