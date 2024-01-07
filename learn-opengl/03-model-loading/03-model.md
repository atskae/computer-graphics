# [Model](https://learnopengl.com/Model-Loading/Model)

A **Model** contains multiple *meshes*.

For example a house Model may contain meshes that represent:
* A patio
* Backyard
* Chimney
* ...etc.

General steps to loading a model:
* Read in a file object using Assimp, which gives us an Assimp Scene object describing the model file
    * The Assimp Scene object is the root of the Assimp data structure
* Traverse the Scene graph and generate Mesh objects out of the Scene's nodes
    * [Assimp documentation on the import process and data structures](https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html#data-structures)
    * Assimp's [Mesh object](https://assimp.sourceforge.net/lib_html/structai_mesh.html#details)

All of Assimp's [post-processing options](https://assimp.sourceforge.net/lib_html/postprocess_8h.html)
