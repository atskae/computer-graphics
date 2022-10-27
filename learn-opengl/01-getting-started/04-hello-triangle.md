# [Hello Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)

The **graphics pipeline** renders a 3D image on a 2D screen. The pipeline can be split into to two general steps:
1. Convert 3D coordinates to 2D coordinates
2. Convert 2D coordinates to colored pixels

Many sub-steps in the graphics pipeline can be parallelized.
A **shader** is a small program that is run on the GPU. We can run many shaders in parallel on a GPU.

TIL a shader is more general that I thought, it's not just related to the fine arts term of shading.

The **OpenGL Shader Language** (GLSL) is a programming language to write shaders.

Graphics pipeline in OpenGL:

Input: `Vertex Data` (points, triangles, etc.) We need to provide a hint to OpenGL on what type of data is in the Vertex Data.
1. Vertex shader: converts 3D coordinates to other forms of 3D coordinates
* Other basic processing on the vertices
* The vertex shader program takes in one vertex, then the program is applied to all vertices
2. Primitive shape assembly: create primitives/shapes from the vertices
3. Geometry shader: create additional shapes
4. Rasterization: maps primitives to the pixels on the screen, apply *clipping*: removing *fragments* outside of the OpenGL window
* **Fragments** are any data that OpenGL needs to render pixels to the screen
* This step creates fragments for the next step
5. Fragement shader: calculates the final colors of the pixels (lights, shadows, other visual effects)
6. Alpha test and blending: applies alpha values (opacity) and blends colors accordingly

Modern OpenGL requires the user to provide a *vertex shader* and *fragment shader*.

## Vertex input

* All vertices are 3D coordinates (x, y, z coordinates)
* Coordinates are in range [-1, 1] = **normalized device coordinates** (NDC)
    * Anything in this range are visible in the screen

We use OpenGL objects called **Vertex Buffer Objects** (VBO) to send vertices to the GPU.

## Shader Program
We have to compile each type of shader (vertex shader, fragment shader), then combine those shaders into one main shader program.
The shader program connects the outputs of each shader as inputs to the next shader. If the input/output types do not match, there will be a linkage error.

## Layout (GLSL)

In our vertex program we had to specify the vertex attribute's `location`, which describes the storage for the attribute 
```
#version 330 core
layout (location = 0) in vec3 aPos; // <-- raw-hur

void main()
...
```

If we want our C++ logic to pass anything to `vec3 aPos` in our vertex shader, we refer to that input variable with `location = 0`.

[More on layout qualifiers](https://www.khronos.org/opengl/wiki/Layout_Qualifier_(GLSL))
