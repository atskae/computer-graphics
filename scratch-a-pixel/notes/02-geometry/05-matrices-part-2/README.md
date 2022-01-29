# [Matrices Part 2](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/how-does-matrix-work-part-2)

## Relationship Between Matrices and Coordinate Systems
Each row in a rotation matrix defines the *vector* that describes axis itself, with its direction in the positive direction). The vector in the first row corresponds to the x-axis, the second row corresponds to the y-axis, and the third row corresponds to the z-axis.

If we plug in `theta=0` into the matrix that rotates around the z-axis, `R_z`, we can see that each row describe a point in the coordinate system which indicates the positive direction of the axis:

![Each row describes the axis direction](images/rotate-around-z-0.png)

When we apply a rotation to `R_z`, the axes (which are vectors) also rotate around the z-axis, and the matrix entries are redefined as such.

For example, if we rotated 90 degrees (`pi/2`) counterclockwise, the axes also rotate 90 degrees:

![The axes definitions of a 90 degree counterclockwise rotation](images/rotate-around-z-90.png)

Some observations:
* We are rotating around the z-axis, so the direction of the z-axis is unchanged
* The x-axis vector (which points in the positive direction) rotated from `P_x(1,0,0)` to `P_x'(0,1,0)` after the 90-degree rotation.
* The y-axis vector (which points in the positive direction) rotated from `P_y(0,1,0)` to `P_y'(-1,0,0)` after the 90-degree rotation


This works for any rotation matrix. Let's try doing something similar to the `R_y` matrix (the matrix that rotates points around the y-axis).

![Each row describes the axis direction](images/rotate-around-y-0.png)

The `R_y(0)` looks exactly like `R_z(0)` above 🤯.

Let's apply a `-pi` rotation (`-180` degrees = `180` *clockwise* rotation).

![Applying a 180 clockwise rotation around the y-axis](images/rotate-around-y-180-clockwise.png)

Some observations:
* We are rotating around the y-axis, so the direction of the y-axis is unchanged
* The x-axis vector rotated from `P_x(1,0,0)` to `P_x'(-1,0,0)` after the 180-degree *clockwise* rotation.
* The z-axis vector rotated from `P_z(0,0,1)` to `P_z'(0,0,-1)` after the 180-degree clockwise rotation

Key Ideas
* Each row in the matrix contains a *vector* which defines an axis (or the bases) of a coordinate system
  * This is important when we want to convert to different coordinate systems (change of basis)
    * To do this we replace each row with the axes (vectors) of the target coordinate system
* Matrices are just a way to store the coordinate system
  * The matrix is sometimes called the **orientation matrix**

### Unimportant Rant
It took me three days to understand what the f they were talking about this part. Why no use pictures?!

![CG education needs work](images/table-flip.png)

They (scratchapixel) actually don't say anything about the row describing the positive direction of the axis, that was just an observation I had. Maybe it's an incorrect fact, who knows...

I guess the benefit of poor explanations is that they force me to actually *try* to understand the concept, draw things out and play around with the theory myself by plugging and chugging in numbers... and figure it out myself.

I think the concept described above is called *passive rotation*? They could have mentioned that at least...

A doodle between frustrations:

![A doodle in betwween trying to understand the damn thing](images/frustration-doodle.png)

(edit) So they have a relevant figure *at the end* 😑 Would have been nice to see this earlier.

## Orthogonal Matrices
The matrices that we described so far (scale, rotation) are **orthogonal matrices**, which have the following properties:
* Is a square matrix
* Has *real* entries (as opposed to *imaginary* ?)
* Its columns and rows are **orthogonal unit vectors**
  * **Orthogonal** means perpendicular (90 degrees)
  * **Unit** means the vector has length 1 

### How do we know that these matrices are orthogonal matrices?

Recall, each row in the matrix represents an axis in the Cartesian coordinate system.
* *Unit vector*: we know the length of each row (each row defines a vector) is of length 1 because we use trigonometric functions `sin()` and `cosine()` to compute them, and these vectors are all on the *unit circle* (which has a radius of 1)
* *Orthogonal*: (welp, they don't explain this) but something to do with *linearly independent* I think... (so each row/axis/vector in the matrix cannot be described using the other rows/axes/vectors)

The rows of the *Identity matrix* defines the axes in the *World coordinate system*. 

![Identity matrix represents the World coordinate system](images/identity-is-world.png)

The rotation matrix is a Cartesian coordinate system, which is initially aligned in the *World coordinate system*, and is rotated around one particular axis.

### An important property
The **transpose** of an orthogonal matrix is equal to its **inverse**.

If `Q` is an orthogonal matrix, then:
```
Q^T = Q^-1
```
```
Q * Q^T = I
```

where `Q^T` is `Q` transpose, `Q^-1` is the inverse of matrix `Q`, and `I` is the Identity matrix.

The significance of this is (hopefully) explained later...

## Affine Transformation
The term *affine transformations* may be used in place of matrix transformations.

**Affine transformations** is a transformation that preserves straight lines. The rotation/scaling matrices that we have seen so far are affine transformations.

Some affine transformations:
* Translation
* Rotation
* Shearing ("the shear" was in 3B1B!)

The above and their *combinations* are affine transformations.

The other type of transformation in CG is **projective transformation**.
* The **perspective transformation** is a type of *projective transformation*
* *Projective transformations* do not necessarily preserve parallel straight lines

## Summmary
* Each row in a matrix represent an axis in the Cartesian coordinate system
* "Orientation (rotation), size (scale), and position (translation) of the coordinate system represents the transformation that will be applied to the points when they are multiplied by the matrix"

Each row in the rotation matrix represents the axes in the new coordinate system `B`. The coordinates in point `P` "changes" because `P` is still defined in terms of the World coordinate system `A` (or any starting world coordinate system). But in the rotation matrix's coordinate system `B`, the point `P` is "still in the same place" relative to `B`'s axes.
* This is because point `P` is expressed in terms of coordinate system `A`
* Multiplying `P` (expressed in terms of `A`) with a matrix (whose axes represent coordinate system `B`) gives us a new point expressed in terms of coordinate system `A`

![Rotation matrices rotate the axes](images/rotate-axes-summary.png)

Order matters when multiplying these matrices together!

The matrices that define the axes in the coordinate system are also called **orientation matrices**.
