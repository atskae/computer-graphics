# [Part 4](https://fgiesen.wordpress.com/2011/07/04/a-trip-through-the-graphics-pipeline-2011-part-4/)

Vertex shaders have access to *texture samplers*.

## Texture State
(using D3D11 as example)

1) **Sampler state**: general settings on how to sample textures 
* Ex) filter mode, addressing mode, max [anisotropy](https://en.wikipedia.org/wiki/Anisotropy#Computer_graphics) (differences of the surface based on orientation, ex velvet)
  * As opposed to [*Isotropy*](https://en.wikipedia.org/wiki/Isotropy) is uniformity in all orientations
2) The underlying **texture resource**: a pointer to the raw texture bits
* Defines the physical layout of the texture in memory, whether it's a single texture or texture array
3) The **shader resource view** (SRV): defines how the texture bits should be interpreted by the texture sampler

