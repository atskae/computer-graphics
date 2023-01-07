# [Part 1](https://fgiesen.wordpress.com/2011/07/01/a-trip-through-the-graphics-pipeline-2011-part-1/)

* DX11 (DirectX version 11) and D3D9 (Direct3D version 9) are versions of the DirectX/Direct3D graphics APIs
* "Running D3D9 on DX11 hardware" means running an older version of Direct3D (D3D9) on hardware that can support the newer version (DX11)
* DirectX is the API package, with Direct3D a subset that specifically provides functionality for 3D graphics
  * Other utilities in DirectX: audio, input, networking, etc.
* Direct3D can only run on Windows and Xbox, whereas OpenGL can run on multiple platforms

## The user-mode Graphics Driver (UMD)
* DDI = device driver interface (lower-level API)
  * D3D calls DDI functions to communicate with the graphics driver
* HLSL (high level shading language): programming language used to write shaders
  * Analogous to GLSL for OpenGL
* Resources, like memory, are allocated but not instantiated until needed
  * So there is a delay on the first use of the resource
  * Typical practice: "Dummy draw calls" at the beginning of an application runtime are used to "warm things up"
* KMD (kernel-mode driver)
  * The UMD requests memory, the KMD actually allocates it

[Swizzling textures](https://fgiesen.wordpress.com/2011/01/17/texture-tiling-and-swizzling/), [Morton order/Z-order curve](https://en.wikipedia.org/wiki/Z-order_curve)
  * This is different from swizzling in programming (ex. `coordinate.xyzw` syntax)
  * You store information in the data's address
    * Ex) The bits that make up the (x,y) coordinate and color channels stored in the memory address
  * Maps multidimensional data to one dimension while perserving localilty/spatial relationships between points
    * Points that are close to each other originally will also be close to each other in the Z-order curve

 ## Enter the Scheduler 
* The GPU is a shared resource, shared across multiple apps on the system
* The **graphics scheduler** (as opposed to the OS scheduler)
* The graphics scheduler timeslices access to the 3D pipeline of multiple apps
  * Only one process can access the 3D pipeline at a time

## The kernel-mode driver (KMD)
* The KMD actually interacts with the hardware
* Multiple UMDs (user-mode drivers processes) run on a system but only one KMD process runs
* Manages resources shared by multiple processes
  * Mouse cursor controls, GPU memory
* Handles interrupts with ~~watchtimer dog~~ watchdog timer
* Content protection, Digital Rights Management (DRM)
  * Tries to protect digital media from unauthorized access and copying
  * KMD manages this

Manages the **command buffers**: commands to the hardware
* Direct memory access (DMA): allow transfers of memory without using the CPU
  * (Oh yeah... 😅)

UMD (user-mode driver) submits commands to the scheduler, then:
1. UMD command is passed to the KMD
2. KMD writes the command to the *main command buffer* (a ring buffer)
  * The GPU reads the commands from the ring buffer, the KMD writes commands to it

The bus! PCI Express
* DMA transfers take this route

The command processor is the frontend of the GPU.
* This is where the GPU read the commands that the KMD wrote

## OpenGL comparisons
* API and UMD split not as explicit in OpenGL
* GLSL is compiled on the driver side, whereas HLSL is compiled on the API-side
  * (side note) HLSL is compiled down to D3D bytecode, a compiled version of the shader program
    * This bytecode is what actually gets run on the GPU
  * Since HLSL is compiled on the API-side, there is only one compiler
    * Whereas there is a GLSL compiler for each hardware vendor/driver
