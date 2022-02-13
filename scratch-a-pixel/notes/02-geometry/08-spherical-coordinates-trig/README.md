# [Spherical Coordinates and Trigonometric Functions](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/spherical-coordinates-and-trigonometric-functions)

To render images for easily, we need to express vectors in terms of *spherical coordinates*.
* Makes shading easier

## Trigonometric Functions

### The Unit Circle

![Unit circle](images/unit-circle.png)

* **unit circle** has a radius of 1
  * A point `P` on the unit circle with angle `theta` can be expressed as `P(x,y)`, where
    * The x-coordinate `x` = `cos(theta)`
    * The y-coordinate `y` = `sin(theta)`
* The angle `theta` is defined in **radians**
    * In code you have to convert angles to radians from *degrees*:
    ```
    A_radians = pi/180 * A_degrees
    ```
    * A complete lap around the unit circle = 360 degrees = `2*pi` radians

### Sine, Cosine, and Tangent and the right triangle

Sine, cosine, and tangent are defined from a relationship in a *right triangle*.

![Rigth triangle](images/right-triangle.png)

* `sin(theta) = opposite/hypotenuse = P_y`
* `cos(theta) = adjacent/hypotenuse = P_x`
* `tan(theta) = opposite/adjacent = P_y/P_x`
  * This is also the ratio of the y-coordinate over the x-coordinate

The inverses. If we pass in the result of the original function (`sin`, `cos`, `tan`), the inverse returns the angle `theta`:
* `theta = arccos(P_x)`
* `theta = arcsin(P_y)`
* `theta = arctan(P_y, P_x)`
    * Some arctan implementations have a separate version that takes sign (positive or negative) into account (ex. C++ has `atan()` and `atan2()`)

**The Pythagorean Theorem**:
```
hypotenuse^2 = adjacent^2 + opposite^2
```

## Representing Vectors with Spherical Coordinates
In a cartesian coordinate system, we represented a vector with three values `P(x, y, z)` (a value per axis).
* We can represent the same vector using only two values with spherical coordinates

(got completely lost here)
* Is this a typo? "vertical angle (in red)"
  * The red angle is clearly not vertical...

(sigh)
(just skip this part and go to the next one since the explanation is trashed later anyway)

## Conventions Again: Z is Up!

We were using the *right-hand coordinate system* until now, but spherical coordinates conventionally use the **left-hand coordinate system**.

![Z is defined as up](images/z-is-up.png)

This is the *physics* convention of labeing the angles... (as opposed to the *mathematics* convention, which flips the labels....)
* z-axis is defined as the **up vector**
  * The angle between the vector and the z-axis is `theta` (also called the **polar** angle)
* x-axis is defined as the **right vector**
  * After projecting the vector to the horizontal plane, the angle between the x-axis and the projection is angle `phi` (also called the **azimuth** angle)
* y-axis is defined as the **forward vector**

(I hate it when they tell you to "see the next chapter" in the middle of a chapter, like, why is the next chapter the prerequisite of this chapter... 😑)
* And with broken links 😡

(Wikipedia and other sources switch `theta` and `phi` UGHHH, there is no convention -cries-)
* [A Redditor asked the same damn question](https://www.reddit.com/r/Physics/comments/14rb66/why_are_theta_and_phi_switched_between/) !!!
    * [A brief paper on this problem](https://bridge.math.oregonstate.edu/papers/spherical.pdf)

# [Creating an Orientation Matrix or a Local Coordinate System](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/creating-an-orientation-matrix-or-local-coordinate-system)

(doesn't *formally* tell you what a normal is. It's been mentioned randomly but yeah, never defined. Thanks.)

A **normal** is a vector that is perpendicular to a give object (ex. a plane) ([from Wikipedia](https://en.wikipedia.org/wiki/Normal_(geometry)))

This chapter is about building a local coordinate system using a *normal*.
* We do this in rendering pipelines, to convert points that are in one coordinate system to another coordinate system

* [Review from coordinate systems chapter](../02-coordinate-systems/README.md#left-handed-vs-right-handed-coordinate-systems) on the meaning of up vector, forward/down vector, the right vector
  * Note foward/down (interchangeable, ugh)

(does not explain what a *tangent* or *bitangent* is 😑)
* [This OpenGL article explains it briefly](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/)️
  * Is bitangent the same as binormal???? Another stupid lack of consensus?😵

To create a local coordinate system, we take a point `P`:
* The vector that is perpendicular to the point `P` is the **normal vector**. Let the *normal* at the point `P` to become an axis in the coordinate system
  * The normal is often aligned with the "up" vector

  ![Normal vector on sphere](images/normal-on-sphere.png)

* The **tangent vector** is any vector on the plane that is tangent to point `P`:
  
  ![Tangent plane](images/tangent-plane-on-sphere.png)

  There are an infinite number of tangent vectors to choose from, from this tangent plane.

  We choose a vector of unit length on this tangent plane to serve as our tangent vector (choose any unit vector??? I guess)

* The **bi-tangent** vector is the tangent vector that is *orthogonal* to the tangent vector we chose earlier (the tangent and the bi-tangent form a right-angle)

  ![Bi-tangent vector](images/bi-tangent-on-sphere.png)

* The *tangent* and *bi-tangent* vectors are sometimes referred to as **derivatives**
  * **dPdu** (tangent) and **dPdv** (bi-tangent)
    * Does not explain why these names....

The three vectors (the normal, tangent, and bi-tangent) should all be *orthogonal* and of *unit length* (length of 1).
* The cross product of the tangent vector and the bi-tangent vector should return the normal vector (review [previous section on cross-products](../03-math-operations-points-vectors/README.md#cross-product)).
  * Need to be careful here, because the cross product can return a vector that is pointing *inwards* into the object, not outward (which is what we want)

  ![Cross product right hand rule](images/cross-product-right-hand-rule.png)

  [cross product review](../03-math-operations-points-vectors/README.md#how-to-compute-the-cross-product)


## Create the Orientation Matrix from the three vectors
Using the *normal*, *tangent*, and *bi-tangent*, we can create the (right-handed) coordinate system:
* Align the normal vector `N` to the *up vector*
* Align the tangent vector `T` to the *right vector*
* Align the bi-tangent vector `B` to the *down/forward* vector 

![Orientation matrix 1](images/orientation-matrix-right-hand-1.png)

Recall that the label of "y-axis" and "z-axis" for the up vector may be flipped in different (coding) contexts. For example, shading tasks typically set the up vector to the z-axis:

![Orientation matrix 2](images/orientation-matrix-right-hand-2.png)

The orientation matrices shown are in *row-major* format. If using the *column-major* format, each axis is defined in each column of the matrix.
* For example, if the z-axis was the up vector:
  * First column of the matrix = tangent vector `T` = "x-axis"
  * Second column of matrix = bi-tangent vector `B` = "y-axis" 
  * Third column of matrix = normal vector `N` = "z-axis"

