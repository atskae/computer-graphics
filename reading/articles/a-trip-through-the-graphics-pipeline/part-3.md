# [Part 3](https://fgiesen.wordpress.com/2011/07/03/a-trip-through-the-graphics-pipeline-2011-part-3/)

## 3D Pipeline Stages
Pipeline names from D3D10/11
* Rougly 12 stages

## Input Assembler Stage
**Input assembler (IA) stage**: load the indices of the vertex buffer
* [*Indexed vertices*](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/) means we can re-use an vertex by referring to its *index*
  * Ex) If we draw two triangles, without indexing, we'd have to duplicate in memory the shared vertex
    * With indexing, the triangles can simply use the same index that refers to the shard vertex
  * [Indexed drawing in LearnOpenGL](https://learnopengl.com/Getting-started/Hello-Triangle)
    * In OpenGL, we bind an index buffer `GL_ELEMENT_ARRAY_BUFFER` and use `glDrawElements()` for indexed drawing
* Checks that indices are in bounds

## Vertex Caching and Shading
Vertex cache needs to be large enough to actually get cache hits before the vertices are replaced by newer vertex entries

One sequential FIFO with many shader cores
* [Amdahl's Law (what a throwback)](https://en.wikipedia.org/wiki/Amdahl%27s_law): defines the upper bound of the speedup gained from optimizing a single part of the system
* The sequential part will be a bottleneck

I asked ChatGPT: `"Explain Amdahl's Law to a five year old"`:
```
Amdahl's Law says that if you want to make a job go faster, you can only make it a little faster by fixing a small part of it. The bigger part of the job still takes just as long.

Think of it like cooking a big yummy cake. Even if you have a super fast mixer, if you still have to wait for the oven to bake the cake, the whole job of making the cake will not be super fast.
```
🎂

(for example) 32 vertices per batch. As you read the vertices, cache them
* Many shader units available

## Shader Unit Internals
* Can read about shader units from presentations by AMD, Nvidia or their documentation on CUDA/Stream SDKs
* Look at disassembled HLSL/GLSL code

A shader unit summary
* It's a fast ALU built around [FMAC (**F**loating **M**ultiply **AC**cumulate)](https://en.wikipedia.org/wiki/Multiply%E2%80%93accumulate_operation)
  * Specialized HW support for selected mathematical operations
    * Ex) squared root, reciprocal, sin, cos
* Optimized for high throughout, high density, not low latency
  * To make up for latency, uses many threads
    * A small number of registers per thread
* Good at straight-line code, bad at branches (especially if not coherent/consistent) 🌿
  * [CUDA: branching and divergence (what a throwback)](https://docs.nvidia.com/cuda/cuda-c-best-practices-guide/index.html#branching-and-divergence)
    * Each branch has to be executed separately
 