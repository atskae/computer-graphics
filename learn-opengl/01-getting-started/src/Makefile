CC=g++
CFLAGS=$(shell pkg-config --cflags glfw3) $(shell pkg-config --static --libs glfw3) -std=c++17 

hello-window: main.cpp glad.c stb_image.cpp
	$(CC) -o hello-window main.cpp glad.c stb_image.cpp $(CFLAGS)

clean:
	rm hello-window
