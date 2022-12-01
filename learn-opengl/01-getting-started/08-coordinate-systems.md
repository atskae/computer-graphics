# [Coordinate Systems](https://learnopengl.com/Getting-started/Coordinate-Systems)

OpenGL expects **normalized device coordinates** (NDC), where the x, y, and z components of a coordinate are in range `[-1.0 , 1.0]`.
* Coordinates outside of this range will not be visible

Workflow of coordinate conversion:
* On CPU-side, work with coordinates in our own defined ranges
* In the vertex shader, convert coordinates to normalized device coordinates (NDC)
* Send coordinates from vertex shader to rasterizer, which transforms the coordinates to 2D pixels on the screen

Coordinates are transformed to various *intermediate* coordinate systems; each coordinate system makes certain transformations much easier.

The most important coordinate systems that are used:
* Local space (object space): where the vertex coordinates start off as
* World space: all objects in the scene are relative to some global origin
* View space (eye space): gets the coordinates from the perspective of the camera/viewer
* Clip space: get coordinates to range [-1.0, 1.0] and determine what is visible on screen. Perspective projection is also applied
* Screen space: applies **viewport transform** which transforms coordinates from [-1.0, 1.0] to the pixels on the screen, defined by the `glViewport` size.
  * The coordinates are then sent to the rasterizer which turns the coordinates into fragments

## The Global Picture

We use transformation matrices to convert coordinates to different coordinate systems. The most important types are:
* Model matrix
* View matrix
* Projection matrix

Coordinates transform: local coordinates -> world coordinates -> view coordinates -> clip coordinates -> screen coordinates.

### Local Space
When you import a model (ex. from Blender), the object's origin is at `(0,0,0)`, even if in the final scene (with other objects) the object will be placed at a different coordinaate.
* Local space are the coordinates of the object as if it was the only object in the scene

### World Space
Where the object is in relation to other objects in the same scene of a larger world.
* The **model matrix** transforms local coordinates to world coordinates
  * The matrix translates/scales/rotates the object to place the object in the world
  * Ex. shrink a house model and rotating it before placing it in a global town scene with other objects

### View Space
Also referred to as *the camera of OpenGL*. The view space is also called **camera space** or **eye space**.

The view space is taking the world coordinates and viewing the scene from the perspective of a viewer/camera that can be placed anywhere in the scene.

The **view matrix** transforms world coordinates to view space coordinates.

### Clip Space

At the end of the vertex shader run, OpenGL expects all coordinates to be in range `[-1.0, 1.0]` (normalized device coordinates, NDC). If outside this range, the coordinate is **clipped**/removed from view.

The **projection matrix** converts view space coordinates to clip-space coordinates.
* The matrix has to know range in each dimension of the view space coordinates
* Ex. view space range could be `[-1000, 1000]`, and this needs to be converted to NDC range: `[-1.0, 1.0]`
* Perspective Division occurs in the conversion process

**Frustum**: the viewbox that the projection matrix creates
* All coordinates in the frustum will be drawn on the user's screen
* **Projection** is the process to convert 3D coordinates (of a scene) to 2D coordinates (to the screen)

After converting to clip space, **perspective division** is applied, where each component (x, y, z) of a coordinate is divided by the `w` component of the coordinate.
* This converts a 4D clip space homogenous coordinate to a 3D normalized device coordinate
* This occurs automatically at the end of the vertex shader

Two types of projection matrices:
* Orthographic
* Perspective
