# [Textures](https://learnopengl.com/Getting-started/Textures)

We could use an image as a texture to apply to a shape.
* For each vertex in the shape (ex. triangle), we need to map that vertex to the coordinate in the texture image
  * Each vertex must map to a **texture coordinate** to know where to sample from in the texture image
  * *Fragment interpolation* fills in the rest of the shape

Texture coordinates `(x,y)` are in range [0, 1]
* Texture coordinate `(0,0)` is the lower-left corner of the texture image
* Texture coordinate `(1,1)` is the upper-right corner
* **Sampling**: getting a color from a texture image using texture coordinates


## Texture Wrapping

Can tell OpenGL how to handle texture coordinates that fall outside the range [0, 1]
* Repeat, mirrored-repeat, clamp, ...
* Can configure for each *axis* of the texture coordinate with `glTexParameter()` family of functions
  * Texture coordinates `(s,t)`
    * `GL_TEXTURE_WRAP_S` applies the effect to the first axis
    * `GL_TEXTURE_WRAP_T` applies the effect to the second axis


## Texture Filtering

A **texel** is a pixel in the texture image.
* OpenGL has to figure out which texture coordinate maps to which texel
  * This is called **texture filtering**
  * Can configure OpenGL to do this in different ways

Texture filtering options:
* `GL_NEAREST`: nearest-neighbor or point filtering
  * Choose the texel with the closest distance from the texel's center to the texture coordinate
  * Blockier result
* `GL_LINEAR`: bilinear filtering
  * Interpolate between the texels near the texture coordinate to compute a new color
  * Smoother resutl

We can also configure what happens to the texture when it is scaled up or down in size
* Magnifying or minifying
* Can use either `GL_NEAREST` or `GL_LINEAR` here too

### Mipmaps

[**Mipmaps**](https://en.wikipedia.org/wiki/Mipmap) are a collection of the same texture pattern of different sizes
* Each texture image is twice as small as the image larger than it
* Use a smaller texture image for objects that are far away, and higher-resolution texture images for objects that are closer in view
  * More efficient - we don't load in a giant texture image for a small object
* OpenGL can generate mipmaps for us: `glGenerateMipmap()`
  * Can do a combination of nearest-neighbor and linear
* Configuring mipmaps for magnification will result in error, since mipmaps are used for downscaling
  * (doesn't this depend on your perspective? 🤓)

## Loading and Creating Textures

[`std_image.h`](https://github.com/nothings/stb/blob/master/stb_image.h) library loads images of various formats.
* (C++ programming: download a header file from the internetz, shove it into an include folder, compile, wahoo!)

A **sampler** object in GLSL (ex. `sampler1D`, `sampler2D`, etc) holds the texture object (from `glGenTextures()` that we created on the CPU-side.
* Define a sampler in the fragment shader
* Apply the texture in the fragment shader using GLSL's built-in function `texture()`
