windows: main.cpp include/*.hpp
	g++ -std=c++20 -I./include -L./raylib/raylib-6.0_windows64/lib main.cpp -o main.exe -lraylib -lopengl32 -lgdi32 -lwinmm -O2

linux: main.cpp include/*.hpp
	g++ -std=c++20 -I./include -L./raylib/raylib-6.2_linux_amd64/lib main.cpp -o main.out -lraylib -lGL -lm