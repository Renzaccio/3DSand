all:	main.cpp
	g++ -std=c++20 -L./lib main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o 3DSand