# [Matrices](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/matrices)

Matrices are essential for moving objects, light, and cameras, for creating the images you want!

## Introduction to Matrices: they make transformation easy!
A matrix is a way to combine all these transformations (scale, rotation, translation) on the points/vectors of an image.
* Multiplying a point/vector by a matrix applies the transformation, and returns the transformed point/vector
* Can combine transformations: scale, rotation, translation
  * Ex. we can create a matrix that:
    * Rotates a point 90 degrees along the x-axis
    * Scale a point by 2 long the z-axis
    * Translate the point (-2,3,1
  * Without matrices, we would have to write more code:
    ```
    # Define a bunch of constants
    point = (1,1,1)
    translate_val = (-1,2,4)
    scale_val = (1,1,2)
    axis = (1,0,0)
    angle = 90
    
    # Apply each transformation
    new_point = translate(point, translate_val)
    new_point = scale(new_point, scale_val)
    transformed_point = rotate(new_point, axis, angle)
    ```
  * *With matrices*, we only need to construct the matrix that performs these transformations:
    ```
    matrix = M(...)
    point = (1,1,1)
    transformed_point = point * matrix  # einfach!
    ```
    すごい🤯


## Matrices, what are they?
A **matrix** is a 2D array of numbers.
* An `m x n` matrix has `m` rows and `n` columns
* Rows are the horizontal lines of numbers, columns are the vertical ones

    <img src="images/matrix-rows-columns.png" width=400px>

    <img src="images/matrix-example.png" width=300px>

In CG, the entries in a matrix are called **matrix coefficients** (is also called "entry" or "element" in other fields).
* Indicate a matrix coefficient in the `i`th row and `j`th column.
* The matrix itself is usually written as a capital letter (ex. `M`)
* `M_ij` = the coefficient in matrix `M` in the `i`th row and `j`th column

CG mostly uses **square** matricies: the number of rows `m` and number of columns `n` are the same.
* Interested in `3x3` and `4x4` matrices

## Matrix Multiplication
### Where is it used

Multiplying matrices together is a core part in applying transformations to points and vectors.

The result of a matrix multiplication (also called **matrix product**) is another matrix.
```
M_3 = M_1 * M_2
```

A matrix defines the linear transformations that can be applied to points/vectors (scale, rotation, translation).

*Matrix multiplication* is a way to combine, in one matrix, the effect of other matrices.
* If `M_1` applies translation, and `M_2` applies scale:
  * We can combine `M_1` and `M_2` to create a new matrix `M3`: `M_1 * M2 = M3`
  * `M3` is a *single* matrix that applies translation *and* scale (in that order).
* There is a unique matrix for each particular transformation

<img src="images/combining-matrices.png">

Two matrices `M1` and `M1` can only be multipled if the number of colums in `M1` is equal to the number of rows in `M2`
* `M1 = M(3 x 4)`
* `M2 = M(4 x 3)`
* `M1 * M2` is allowed.
  * (3x**4**)(**4**x3) match!
* `M(mxp) * M(pxn) = M(mxn)`
* CG mostly use *square* matrices so we ususally won't have to care about this 👏

### How to multiply matrices
* (Arrays start at zero) (hooray)

![Matrix multiply](images/matrix-multiply.png)

* Matrix multiply is *not* commutative!
  * `M1 * M2` does not equal `M2 * M1`
  * We need to pay attention to the order that the matrices are mutliplied
    * Common bug

# [How Does Matrix Work: Part 1](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/how-does-matrix-work-part-1)

## Point-Matrix Multiplication
* We can think of a 3D point/vector as a `1x3` matrix, a matrix with `1` row and `3` columns
  * Point `P = [x, y, z]`
* We can multiply this 3D point/vector `P = [x, y, z]` with a matrix `M(3xn)`

<img src="images/point-matrix-multiply.png">

* We want to transform a point, so a point multiplied by a matrix (applying the transformation) should also return a point
  * Given a 3D point `[x y z]` = `1x3` matrix, we can only multiply that point by a `3x3` matrix in order to get a new 3D point after the transformation 

## The Identity Matrix
The **identitiy matrix** (or **unit matrix**) is a *square* matrix whose coefficients/entries are all 0 except the coefficients along the diagonal, which have the value 1

<img src="images/zero-matrix.png">

* The result of point `P` multiplied by the Identity matrix `I` will give us point `P`:
  * `P * I = P`

![Identity matrix multiply](images/identity-matrix-multiply.png)

## The Scaling Matrix
* When we multiplied point `P = [x, y, z]` by the Identity matrix `I`:
  * `x` was multiplied by `I_00` (the 0th row and 0th column of `I`)
  * `y` was multiplied by `I_11`
  * `z` was multiplied by `I_22`
* Another way to think about it: the Identity matrix scales each dimension (`x`, `y`, and `z`) by a scale of `1`
* We can create a **scaling matrix** by starting with the identity matrix and changing the values of the diagonal for each respective dimension `x`, `y`, and `z`:

![Scaling matrix multiply](images/scaling-matrix-multiply.png)

* Example, to scale point `P(1,2,3)` by a scaling factor `S_x=1`, `S_y=2`, `S_z=3`

![Scaling matrix multiply example](images/scaling-matrix-multiply-example.png)

## The Rotation Matrix
The **rotation matrix** will rotate a point/vector around one axis in the Cartesian coordinate system, using trigonometry!
* We can technically use *translation* to achieve the same effect, but using rotation is easier

![Rotate P around the z-axis](images/rotate-around-z-90.png)
* Rotating point `P` **counterclockwise** by `90` degrees
* We want a matrix `R` such that `P_T = P * R`

The z-coordinate (depth) doesn't change when we rotate around the z-axis (`P.z == P_T.z`)

![The j=2 column of rotation matrix R](images/rotation-matrix-column-2.png)

The x-coordinate goes from `1` to `0`, and 

![x-coordinate after rotation](images/rotation-x-coordinate.png)

the y-coordinate goes from `0` to `1`
![y-coordinate after rotation](images/rotation-y-coordinate.png)

## Using Trigonometry
We can compute the x- and y- coordinates of `P_T` (the point after rotating by angle `theta`):
* `x = cos(theta)`
* `y = sin(theta)`

`theta = pi/2` is equivalent to a rotation of `90` degrees counterclockwise
* `x = cos(pi/2) = 0`
* `y = sin(pi/2) = 1`

![Using trig to define relationships in rotation](images/rotate-trig.png)

We can derive the general rotation matrix around the z-axis by any angle `theta`:

![Matrix for rotating around z-axis in general](images/rotation-matrix-z-general.png)

We can derive the matrix that rotates a point `90` degrees around the z-axis from the general rotation matrix:

![Matrix for rotating around the z-axis by 90 degrees](images/rotation-90-z-from-general.png)

### Making clockwise rotations work
Check if clockwise rotations work correctly.

First derive the matrix:
![Derive 90 degree clockwise rotation](images/try-derive-clockwise-90-matrix.png)

If this matrix is correct, it should take `P(0,1,0)` to `P_T(1,0,0)`:

![Try to compute clockwise 90 degree rotation](images/try-compute-clockwise-90-point.png)

It doesn't apply the clockwise 90 rotation correctly... The x-coordinate ends up being flipped (negative).
We know the first column of the matrix computes the x-coordinate of the transformed point `P_T`.  We need to make `sin(theta)` negative in the first column:

![Fix clockwise 90 matrix](images/fix-clockwise-90-matrix.png)

Let's check to make sure clockwise rotation works:
![Check clockwise rotation](images/check-fixed-clockwise-90-matrix.png)

It works!! 🕺

Let's make sure the new matrix works for the original 90-degree *counterclockwise* rotation:
![Check counterclockwise rotation](images/check-fixed-counterclockwise-90-matrix.png)

It works!! 💃

### Derive rotating around the x-axis

![Rotate around the x-axis](images/rotate-around-x-90.png)
Let's try to figure out matrix `R_x`,  a matrix that performs a 90 degree rotation around the x-axis.

(I guessed and checked a lot... It seems to help when we first focus on the coordinate that *transforms to* a non-zero value).

The y-coordinate transforms from 0 to 1 after the rotation:

![Figure out the y-column of x-axis rotation](images/rotate-around-x-90-y-column.png)
So `R_x` row 2, colum 1 is `-sin(theta)`.

The z-coordinate transforms from 0 to -1 after a **clockwise** rotation:

![Figure out the z-column of x-axis rotation](images/rotate-around-x-90-z-column.png)

So `R_x`, row 1, colum 2 is `sin(theta)`.

`R_x` matrix so far:

![x-axis rotation matrix so far](images/rotate-around-x-90-matrix-so-far.png)

#### Use trigonometry!

![Use trig to find matrix values](images/rotate-around-x-matrix-use-trig.png)

Let's to do a `pi/6` rotation from `pi/6 = 30 degrees` to `pi/3 = 60 degrees`. Then we can use the properties of 30-60-90 triangles.

![30-60-90 triangles](images/30-60-90-triangles.png)

Use the values of `cos(pi/6)` and `sin(pi/6)` to calculate the y- and z- coordinates of point `P` and `P_T`:

![Use 30-60-90 trangles to get coordinates](images/use-30-60-90-to-find-coordinates.png)

Now we know that rotating the point `P(0, 1/2, -1 * sqrt(3)/2)` with angle of `pi/6` in the counterclockwise direciton will result in point `P_T(0, sqrt(3)/2, -1/2)`. Note that the z-coordinate is negative.

We can now compute the missing coordinates in the `R_x` matrix in the same way that we did with the previous entries:

![Compute missing entries in matrix that rotates around x-axis](images/derive-rotate-around-x-axis.png)

The final matrix that rotates a point around the x-axis with angle `theta`:
![Matrix that rotates a point around the x-axis](images/rotate-around-x-axis-final-matrix.png)

