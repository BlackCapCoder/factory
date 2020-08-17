build:
	clang++ -std=c++20 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -lOpenGL -O2 *.cpp -o main -Wno-address-of-temporary -Wno-c++11-narrowing

run: build
	./main
