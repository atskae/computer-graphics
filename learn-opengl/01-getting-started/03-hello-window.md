# [Hello Window](https://learnopengl.com/Getting-started/Hello-Window)

Install `pkg-config`, which provides all the linker flags needed to compile a program that uses that header:
```
sudo apt install pkg-config
```

We can see all the fun compiler flags we need to use GLFW :`)
```
$ pkg-config --cflags glfw3
-I/usr/local/include
$ pkg-config --static --libs glfw3
-L/usr/local/lib -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp
```

Test the compile again:
```
g++ $(pkg-config --cflags glfw3) -o test-compile main.cpp $(pkg-config --static --libs glfw3)
$ ./test-compile 
I hope glfw and glad compiles!
```
