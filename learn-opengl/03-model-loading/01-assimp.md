# [Assimp](https://learnopengl.com/Model-Loading/Assimp)

There are many file formats that store model information (vertices, textures, normals, etc). These model files are exported by 3D artists tools like Maya and Blender.
Example file formats: Wavefront.obj, Collada file format.

The library [Assimp](http://assimp.org/) is able to import many file formats and converts them to a common internal representation.
* All the data is imported into a **Scene Object** that has references to all the objects in the scene
* The Scene Object is a graph - it has references to child nodes of other objects
* Objects hold mesh data and references to child nodes
* **Mesh** data hold vertices, normals, texture, material, anything we need to render the object
    * A Mesh contains several *faces*
* A **Face** holds information on how to render a *primitive* (a triangle, square, or a point)
* A Mesh also holds a **Material** object, which holds information such as color, materials, textures, specular/diffuse maps

* A Mesh is thought to be complete separable objects, such as head, limbs, clothes, weapons
    * A **Model** is a combination of Meshes, for example, a complete human character with a head, limbs, clothes, weapon

General steps of model loading:
1. Read in a Scene object
1. Traverse the Scene object nodes to obtain all the Meshes (collect all the vertex, indices, texture information)
1. Collect all Mesh objects into a single Model object

## Building Assimp

Building and installing [5.3.1](https://github.com/assimp/assimp/releases/tag/v5.3.1)

I generated the Makefile using CMake, then ran `sudo make install`

Then I added `-lassimp` to my Makefile:
```Makefile
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3` -lassimp
```
