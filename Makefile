all:
	g++ -std=c++11 -I. -o engine -O3 main.cpp shader.cpp mesh.cpp model.cpp modtexmesh.cpp stb_image.c texture.cpp -lGLEW -lGL -lsfml-window -lsfml-system -lassimp
