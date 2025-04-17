# Tutorials

Notes on the libigl tutorials.

After configuring CMake and building, the tutorial data files are at:
```
libigl/build/_deps/libigl_tutorial_data-src
```

Each tutorial can be compiled with:
```
libigl/build/tutorial$ cmake --build . --target 101_FileIO
```

and run with:
```
libigl/build/tutorial$ ../bin/101_FileIO
```

## 101_FileIO

Reads in an `.off` file of a cube, prints the vertices and faces data to the console, and saves the file as a `.obj`.

There is a separate function to read/write in each file format, example:
* `igl::readOFF`: `.off`
* `igl::writeOBJ`: `.obj`

## 102_DrawMesh

Reads in and loads the Stanford Bunny `.off` file, sets the model data into the mesh viewer, and launches the mesh viewer.

The viewer class in libigl is:
```cpp
igl::opengl::glfw::Viewer viewer;
```

There are also controls on toggling the wireframes, lighting, etc.

![Stanford bunny](images/102_DrawMesh.png)
