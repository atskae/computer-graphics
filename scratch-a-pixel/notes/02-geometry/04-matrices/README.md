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

