# Learn OpenGL

Notes from [Learn OpenGL](https://learnopengl.com/)

## How to build

Summary of [these steps here](https://learnopengl.com/Getting-started/Creating-a-window) for Ubuntu:
1. Download GLFW: https://www.glfw.org/download.html
1. Install packages for X11:
    ```
    sudo apt install xorg-dev
    ```
1. Download CMake: https://cmake.org/download/
    * Download script, run script (need to `chmod +x`), add the `cmake-x.y.z-linux-x86_64/bin` to `$PATH`
1. Run the Cmake GUI:
    ```
    cmake-gui
    ```
1. I used Makefile when generating
1. In the build folder, I then ran `make install`
1. Get glad, move the `include/glad` and `include/KHR` directory to `/usr/include`
1. Download [GLM](https://github.com/g-truc/glm/releases/tag/0.9.9.8) and moved the `glm/glm` to `/usr/include/`
1. In the `learn-opengl/src`, run:
    ```
    make
    ```
    Then run:
    ```
    ./learn-opengl
    ```
