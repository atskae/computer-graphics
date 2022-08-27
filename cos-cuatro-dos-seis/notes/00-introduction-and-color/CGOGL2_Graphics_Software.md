# Computer Graphics Software
Two types of graphics software:
* Special-purpose packages
  * Ex. Painting programs, applications for architecture, engineering, CAD systems
  * Providies a GUI for users, don't need to worry about the graphics procedures under the hood
* General-purpose
  * A library of software functions for communicating with graphics hardware
  * Graphics API
    * Ex. OpenGL, VRML (Virtual Reality Modeling Language)

## Coordinate Representations
* To generate a picture with a programming language, we need geometric descriptions of objects
  * Ex. coordinates that represent a square, a radius and center to represent a sphere
* Typically graphics API use right-handed Cartesian coordinate systems
  * Other coordinate systems are usually converted to the standard

**Modeling coordinates**/**local coordinates**/**master coordinates**
* Separate Cartesian reference frames are used to describe each individual object in the scene
  * Ex. Trees, furniture
* Then we construct a *model* by placing those objects into the scene reference frame called the **world coordinates**

**Viewing pipeline** the process to display the world coordinate description of the entire scene (individual objects all placed into the same scene)
* Convert world coordinates to *viewing coordinates*: how a hypothetically placed camera views the scene
  * We can a 2-dimensional projection of the 3D scene
  * The 2D projection is what is displayed on our display
  * Then the coordinates are *normalized*
    * Ranges from [-1, 1] or [0, 1] (depending on the system)
    * Graphics package is independent of the input coordinate system
* Then we convert to the coordinate system used by the frame/refresh buffer: **device/screen coordinates**
  * Typically used the *left-handed coordinate system*

Overall process:
1. Define individual objects in their own reference frames
   * 3D **Modeling coordinates**
2. Place objects into the same scene
   * Convert modeling coordinates to **world coordinates**

Viewing pipeline starts:
3. Convert world coordinates to **viewing/projection coordinates**
4. Then normalize the coordinates (to be able to use them in a graphics library)
   * Typically left-handed coordinate system
5. Convert to coordinate system of the frame/refresh buffer for display
    * 2D coordinates

## Graphics Functions
General-purpose graphics packages organize functions depending on use case:
* Graphics output
* Graphics input
* Attributes
* Transformation
* Viewing
* Subdividing pictures
* General control


**Graphics output primitives**: basic building blocks for pictures
* Contains character strings and geometric entities: lines, curves, filled-color areas (polygons)
  * Can also provide routines for generating 3D shapes: spheres, cubes, cones, cylinders

**Attributes**: properties of the output primitives
  * Ex. colors of shapes, line styles, area-filling patterns, textures

**Transformations**: change the orientation, size, and position of the object within a scene
* **Modeling transformations** help construct a scene with objects that have their own reference frames

**Viewing transformations**
* How to project the view the scene
* Ex. Determine visible objects, define lighting

**Input functions**: ways to work with the input devices (ex. mouse, drawing tablet, joystick)

**General control** functions, ex. clearing the screen, initalizing parameters
* Basic chores 🧹

## Software Standards
Goal of standardization of graphics software is *portability*
* Want to be able to run the same software on different systems with different hardware

**Graphical Kernel System (GKS)** is an organization that develops graphics standards (formed in 1984)
* Extension of GKS: **Programmer's Hierchical Interactive Graphics System** (PHIGS)
  * Standardizes hierchical object modeling, color specifications, surface rendering, picture manipulation
  * PHIGS+ is an extension for 3D

Graphics Library (GL), Silicon Graphics Inc.
* OpenGL became hardware independent
  * OpenGL Architectural Review Board: representatives from many graphics companies

**Language bindings** are developed to make use of the particular programming language's capabilities and also implement the standard functionality

## Other Graphics Packages
OpenInventor, VRML, Renderman Interface (Pixar)

## Introduction to OpenGL
Hardware-independent
* Hardware dependent logic is provided as auxiliary libraries

### Basic OpenGL Syntax
**OpenGL Basic Library**/**OpenGL Core Library**
* Functions prefixed with `gl`
* **OpenGL Utility** (GLU) for special operations

* Displaying a window on the screen is system-dependent, so it is not in the core library
  * OpenGL Utility Toolkit (GLUT) gives us device-independent ways to create a window

(p.34+ for reference)

