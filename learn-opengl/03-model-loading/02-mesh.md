# [Mesh](https://learnopengl.com/Model-Loading/Mesh)

"mesh strainer...." 🐩

We need to transform what Assimp loads into data that OpenGL understands. Here we define our own internal representation that we can send to OpenGL.

A Mesh class minimally contains:
* A set of vertices, where each vertex contains:
    * Position vector
    * Normal vector
    * A texture coordinate
* Indices for indexed drawing
* Material data in the form of textures (specular/diffuse maps)
