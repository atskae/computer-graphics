# [Hello Window](https://learnopengl.com/Getting-started/Hello-Window)

The reason we need to include `glad` before `glfw` is because `glad` includes the OpenGL header. For some reason `glfw` will error out if OpenGl is already included instead of, um, skipping the extra include...

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

## GLFW and OpenGL Configurations

All possible options to `glfwWindowHint()`: https://www.glfw.org/docs/latest/window.html#window_hints

[Syntax explanation](https://computergraphics.stackexchange.com/questions/8639/this-function-syntax-is-confusing-me) of this call that initializes GLAD
```
gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)
```
We are casting the type from a pointer to a function to a pointer to anything `(void*)`.

### Error Handling and Exiting
We need to call `glfwTerminate()` when we run into errors so resources are properly deallocated.

## GLFW Window vs OpenGL Viewport
You can specify an OpenGL Viewport that is smaller in dimension than GLFW Window's dimensions, and choose to display other elements outside of the OpenGL Viewport.

All OpenGL rendering only occurs in the OpenGL Viewport.

### OpenGL Viewport Coordinates
The viewport's responsibility is to convert processed coordinate values (which range from [-1, 1]) to the coordinates on your screen (ex. convert (-0.5, 0.5) to screen coordinates (200, 450)).

In a 800x600 pixel window, the OpenGL viewport is converting points from (-1, 1) to (0, 800) and (0, 600).

### Window resize events, function callbacks

If the user resizes the window, then we'd want the OpenGL viewport to adjust its size. We can register a callback function that is called whenever the window is resized.

We attach the window resize callback with `glfwSetFramebufferSizeCallback(...)`

There are many other types of callbacks we can set for various events: ex. error handling, mouse input, etc.

## glfwSwapBuffers()
There are two buffers: the front and back buffers.
The front buffer contains the final image, and rendering commands draw on the back buffer.
This removes the flickering effect the would occur since rendering commands will not modify the image in the front buffer.

Images are rendered top to bottom, left to right.

## Compile with glad.c
We can now compile the program that uses glad and glfw:
```
g++ $(pkg-config --cflags glfw3) -o hello-window main.cpp glad.c $(pkg-config --static --libs glfw3)
```

We can now run the program!
```
./hello-window
```
