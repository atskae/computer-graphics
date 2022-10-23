CC=g++
CFLAGS=$(shell pkg-config --cflags glfw3) $(shell pkg-config --static --libs glfw3)

hello-window: main.cpp glad.c
	$(CC) -o hello-window main.cpp glad.c $(CFLAGS)
