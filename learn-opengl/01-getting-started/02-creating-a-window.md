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
This will create a header and library file in the `build/src` directory:
```
➜  build ls src/*.(a|h)
src/glfw_config.h src/libglfw3.a
```

7. Create a new directory in the home directory called `OpenGL` and subdirectories `include` and `lib`. We will put all headers and libraries for OpenGL projects in there
```commandline
mkdir ~/OpenGL
mkdir ~/OpenGL/include
mkdir ~/OpenGL/lib
```
8. Copy the library file `src/libglfw3.a` to `~/OpenGL/lib` and the header file `glfw_config.h` to `~/OpenGL/include`
```commandline
cp src/src/libglfw3.a ~/OpenGL/lib
cp src/glfw_config.h ~/OpenGL/include
```

### Setting up VS Code
1. Create an empty project folder `hello-window` and open the folder in VS Code
2. Create a `c_cpp_properties.json` file in the project root
```commandline
touch c_cpp_properties.json
```
3. Open the Command Palette (Command + Shift + p) and run `C/C++: Edit Configurations (UI)`
4. Scroll down in the C/C++ Configurations window and add `~/OpenGL/**` to the Inlude Path


## GLAD

Initializing the OpenGL driver is also OS-specific, so we need another library to do this.

The OpenGL driver needs to know where the device-specific functions are at runtime. The developer needs to store these function pointers at runtime.

The library [GLAD](https://github.com/Dav1dde/glad) (Loader-Generator) will do these tasks under the hood for us.

GLAD provides a [web-service](https://glad.dav1d.de/) that generates the required files for the requested version of OpenGL (...that's different).

Used [this chart by Apple](https://support.apple.com/en-us/HT202823) to find the correct OpenGL version.

Then follow the [Setting up GLAD Steps](https://learnopengl.com/Getting-started/Creating-a-window)

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

## Building GLFW in Linux

Need to install the RandR, Xinerama, Xcursor, XInput headers:
```
sudo apt-get install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

Then open the CMake GUI, set the source/destination directories, then click `Configure`.

We can now go into the build folder and run `make` to build and install the package:
```
glfw-3.3.8/build$ make && sudo make install
```

(from here on out it is assumed that we're running on Linux!)

## GLAD
OpenGL is only a specification. The implementations will differ by graphics cards and operating system. *Function pointers* need to be calculated at run-time to point to the correct OpenGL implementation.

GLAD is a library that finds the correct function pointers for you.

First get the OpenGL version installed, you'd need to install `mesa-utils` first:
```
sudo apt install mesa-utils
```

[Mesa](https://www.mesa3d.org/) is an open-source implementation of OpenGL. `mesa-utils` are misc utility functions for Mesa.

Get the OpenGL version:
```
glxinfo | grep "OpenGL version"
```

After download the GLAD zip, move `include/glad`:
```
sudo mv KHR/ /usr/include/
```

The version 1 glad webserver provides `glad.h` but the version 2 webserver gives `gl.h`... Using version 1...


I already had the `KHR` directory in `usr/include`... The `diff` seems minor so I will leave it as is.

Test compilation (for some reason `glad` needs to be imported before `glfw` OTL)

```cpp
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]) {
    std::cout << "I hope glfw and glad compiles!" << std::endl;
    return 0;
}

```
```
g++ -o test-compile main.cpp
```
