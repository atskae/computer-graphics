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

## 103_Events

Loads two mesh files, and configures the viewer to execute a function when a key is pressed.
The callback loads the mesh corresponding to the key (1 == bunny mesh, 2 == fertility mesh).

Register the method `key_down` when any key is pressed:

```cpp
igl::opengl::glfw::Viewer viewer;
viewer.callback_key_down = &key_down;
```

## 104_Colors

All the vertices are normalized to a range [0, 1] (since this is the range of the RGB values of the viewer), and the the normalized vertex values are used as colors.

```cpp
C =
  (V.rowwise()            - V.colwise().minCoeff()).array().rowwise()/
  (V.colwise().maxCoeff() - V.colwise().minCoeff()).array();
```

`V.colwise().minCoeff()` finds the minimum value across all vertex data in that dimension (columns are dimensions: x, y, z).

`V.colwise().maxCoeff()` finds the maximum value in that dimension across all vertex data.

For each vertex, each dimension (x, y, z) is normalized according to the minimum and maximum values in that dimension across at vertex data.

![Normalize data](images/104_Colors_normalizing-vertex-data.png)

![Screwdriver](images/104_Colors.png)

? Why do we use the minimum and maximum values within a dimension to normalize instead of taking the minium and maximum values across all dimensions?
