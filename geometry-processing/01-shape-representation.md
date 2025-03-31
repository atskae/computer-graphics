# 01 Shape Representation

## Object source

Depending on the domain, objects are obtained:
* From the real world (ex. scanning physical objects)
* Create in the digital world (modeling digtally)
* Procedurally generated

## Shape Representation/Data Considerations

Needs:
* Stored digitally on a computer
* Shape data easily opened, modified, and saved
* Modification operations: smoothing, repairing, etc.
* Can be rendered
  * Rasterization
  * Ray tracing

Shapes are represented by:
* Points
* Polygon meshes
* Parametric surfaces
* Implicit functions
* Subdivision surfaces

### Points

A **point** is a tuple of 3 values.

* Points are the typical representation of objects obtained from 3D scanners
  * Later converted to other shape representation (ex. meshes)

### Spatial Data Structures
* Regular uniform 3D lattice
