# [Creating a Window](https://learnopengl.com/Getting-started/Creating-a-window)

We need a window to draw stuff in!
* Creating a window, defining an OpenGL context, and handing user input is OS-dependent
  * Must use a separate library that targets the specific OS
    * Example libraries: GLUT, GLFW, SDL, SFML

## [GLFW](https://github.com/glfw/glfw)

GLFW stands for *Graphics Library Framework* 
* (is this written anywhere other than Wikipedia? 笑)

GLFW is a C library that allows the developer to:
* Render to a window
* Define an OpenGL context
* Handle user input (mouse, joystick, keyboard)

[Download GLFW here!](https://www.glfw.org/download.html)

## CMake

Steps to build on Mac OS:
1. Download GLFW source from [here](https://www.glfw.org/download.html) and extract the zip
2. Create a `build/` directory in the root of the directory (ex. `glfw-3.3.8/build`)
3. In the CMake GUI, set `Where is the source code:` to the path of `glfw-3.3.8` and `Where to build the binaries:` to the path of `glfw-3.3.8/build`
4. Click `Configure`, then `Configure` again to set the settings
   * I chose `Unix Makefiles` for the generator
5. Click `Generate`
6. Go to `glfw-3.3.8/build` and run:
```commandline
make
```

Steps to build CMake on Linux:

Install the C++ compiler:
```
sudo apt-get install g++
```

Install `make`:
```
sudo apt-get install make
```

Install OpenSSL development headers:
```
sudo apt -y install libssl-dev
```

[Install Qt](https://askubuntu.com/questions/1335184/qt5-default-not-in-ubuntu-21-04):
```
sudo apt-get install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
```

Build CMake from source:
```
~/Downloads/cmake-3.24.2$ ./bootstrap --qt-gui && make && sudo make install
```

`--qt-gui` builds the CMake GUI.
