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

## Loading Model

Download the backpack model from [here](https://learnopengl.com/Model-Loading/Model) (under **No more containers!**), and extract the zip to `src/textures/backpack`.

I commented out the backpack's texture loading logic and got the container texture onto it (commit: 5b35a1408dbde1dd003fc954f54f004070a5223b):

![Container backpack](images/container_backpack.png)

With some weird hacks off of commit `5b35a1408dbde1dd003fc954f54f004070a5223b` I was able to get the backpack... but not through the normal `Mesh.draw()` OTL

Things I forgot:
* Setting the  model_matrix in the shader for the backpack (otherwise nothing renders)

Hacks applied to commit:
* Change the texture files to the backpack textures

![Backpack hack](images/weird_hack_backpack.png)

The textures are not applied through Mesh.draw() though...
