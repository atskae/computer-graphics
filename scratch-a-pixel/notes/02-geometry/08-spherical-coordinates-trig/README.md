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

[Spherical coordinate grapher](https://www.geogebra.org/m/FzkZPN3K) amazing resource to understand this!

## Conventions Again: Z is Up!

We were using the *right-hand coordinate system* until now, but spherical coordinates conventionally use the **left-hand coordinate system**.

![Z is defined as up](images/z-is-up.png)

This is the *physics* convention of labeing the angles... (as opposed to the *mathematics* convention, which flips the labels....)
* z-axis is defined as the **up vector**
  * The angle between the vector and the z-axis is `theta` (also called the **polar** angle)
* x-axis is defined as the **right vector**
  * After projecting the vector to the horizontal plane, the angle between the x-axis and the projection is angle `phi` (also called the **azimuth** angle)
* y-axis is defined as the **forward vector**

(Wikipedia and other sources switch `theta` and `phi` UGHHH, there is no convention -cries-)
* [A Redditor asked about flipping theta and phi](https://www.reddit.com/r/Physics/comments/14rb66/why_are_theta_and_phi_switched_between/) !!!
    * [A brief paper on this problem](https://bridge.math.oregonstate.edu/papers/spherical.pdf)

# [Creating an Orientation Matrix or a Local Coordinate System](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/creating-an-orientation-matrix-or-local-coordinate-system)

A **normal** is a vector that is perpendicular to a give object (ex. a plane) ([from Wikipedia](https://en.wikipedia.org/wiki/Normal_(geometry)))

This chapter is about building a local coordinate system using a *normal*.
* We do this in rendering pipelines, to convert points that are in one coordinate system to another coordinate system

* [Review from coordinate systems chapter](../02-coordinate-systems/README.md#left-handed-vs-right-handed-coordinate-systems) on the meaning of up vector, forward/down vector, the right vector
  * Note foward/down (interchangeable, ugh)

* [This OpenGL article explains it briefly](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/)️

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

## What do we do with this matrix?

If we had a vector `V` that is defined in the *world-coordinate system* (or any other system), multiplying `V` by the orientation matrix `M` that we constructed will give us a new vector `V_M` that is defined in terms of the local coordinate system that our matrix `M` describes (using the normal `N`, tangent `T`, and bi-tangent `B` vectors of that local coordinate system).
* There are no *translation* values in the orientation matrix (which are usually in the fourth column/row)
* This orientation matrix is only used with vectors, and typically used in *shading*, since expressing vectors in relation to some normal on a surface makes the shading computation easier

### Affine Space
* [Embree (a renderer from Intel)](https://www.intel.com/content/www/us/en/developer/articles/technical/embree-photo-realistic-ray-tracing-kernels.html) and other renderers may choose to matrices/transformations as *affine space*
  * An affine space uses a location in space and three axes (Vx, Vy, Vz) to describe its coordinate system


## (back to) Conventions Again: Z is up!

Recall the handedness of a coordinate system:

![World coordinate system handedness](images/world-coordinate-systems-handedness.png)

The coordinate values are the same for each axis, (ex. x-axis is labeled as `(1,0,0)` regardless of handedness).

In previous chapters, we've been using the right-hand coordinate system when we were describing things in the World Coordinate System.

But since the convention in shading uses the *left-hand coordinate system*, when we say we convert from the World Coordinate System (where the up vector is the y-axis) to the Local (target) coordinate system (where the up vector is the z-axis), the World Coordinate System is left-handed!!

The *local coordinate system* in the following sections will mean the *left-handed* coordinate system where:
* The right vector is the x-axis
* The forward/down vector is the y-axis
* The up vector is the z-axis

which is the convention used in shading.

### Orientation matrix of local coordinate system

To derive the orientation matrix that describes a local coordinate system where the up vector is the z-axis:
* Place the tangent vector `T` in the first row of the matrix (the x-axis)
* Place the bi-tangent vector `B` in the second row (the y-axis)
* Place the normal vector `N` in the third row (the z-axis)

![Derive matrix that converts to local](images/convert-to-local-matrix.png)

### Matrix that transforms from world space to local coordinate system

*World space* labels the up vector as the y-axis and the local coordinate system labels the up vector as the z-axis. To derive the matrix that converts vectors from world space to the local coordinate system:
* The x-axis is the same in both world space and local, so the first row is still `(1,0,0)`
* The local coordinate system has the y-axis where the world coordinate system has the z-axis, which is at `(0,0,1)`, so this becomes the second row of the matrix, which becomes the y-axis in the local coordinate system.
* The local coordinate system has the z-axis where the world coordinate sytem has the y-axis, whcih is at `(0,1,0)`. so this becomes the third row of the matrix, which becomes the z-axis in the local coordinate system.

![Deriving conversion matrix](images/deriving-conversion-matrix.png)

Let's apply a conversion of a vector in world space: `v = (0,1,0)`, which is aligned to the y-axis (the up vector) in world space:
```
v' = v * M
```

The matrix multiply (which applies the coordinate system conversion) results in `v' = (0,0,1)`, which is now parallel to the z-axis (the up vector in the local coordinate system).

![Applying the coordinate system conversion](images/apply-coordinate-system-conversion.png)

(Their explanation is confusing because `(0,0,1)` is clearly *not* aligned with the up vector in either coordinate system was zum Teufel)

(The way I see it, which makes sense, is the y-axis in world space is `v=(0,1,0)`, and `v'=(0,0,1)` is the y-axis in the local coordinate system. Makes sense! Why is this not stated anywhere? Is this not the way to see this conversion??? -sigh- -cry-

Basically the conversion switches the y and z coordinates. <--- is what they say

## Converting Cartesian to Spherical Coordinates

We will now be working in the coordinate system where the z-axis is the up vector.

![Local coordinate system](images/local-coordinate-system-z-is-up.png)

(it would be nice if they told you where the original angle was...)

### Finding angle theta

Let's say we have a vector `V` in this coordinate system with length 1 and angle `theta` from the z-axis. One of many side-views of this vector:

![A vector in local coordinate system](images/vector-in-local-coordinate-system.png)

(I'm assuming that the horizontal axis is the x-axis in this view but they don't tell you so, maybe that's not important... it would have been very helpful though)

To make computation visually easier to understand, let's rotate the figure above by 90 degrees clockwise:

![Rotate vector 90 degrees clockwise](images/rotate-vector-90-clockwise.png)

Now this looks a lot like the unit circle that we've seen before, and the trigonometric relationships are more clear.

We can see that `V_z = cos(theta)`, which also tells us that that the angle `theta = arccos(V_z)`.

### Finding angle phi

![Top view](images/top-view-phi.png)

(they don't even tell you if it is a projection of `V`, and not `V` itself...)

Recall that the `tangent` is the ratio of `V_y` to `V_x`. We can use this to find angle `phi` using the *inverse* of tangent: arctangent (`arctan`).

```
tan(phi) = V_y/V_x
```

```
phi = arctan(V_y/V_x)
```

In programming, you need to make sure to use an `arctan` function that takes into account negative `V_x` and `V_y` values.

## And Vice Versa: Spherical Coordinates to Cartesian Coordinates
Using angles `theta` and `phi`, we can get back the x, y, and z coordinates:
```
x = cos(phi) * sin(theta)
y = sin(phi) * sin(theta)
z = cos(theta)
```

The z-coordinate can be deduced from the [previous section](README.md#finding-angle-theta), where we derived a way to see how `V_z = cos(theta)`.

For the x and y coordinate, it is easier to see this relation with an example vector `V(1,0,0)`:

![Spherical to Cartesian relationship](images/spherical-cartesian-relation.png)

TODO maybe get a more intuitive understanding of why this works

## More Tricks with Trigonometric Functions
These functions are common in renderers:
(note we are using the left-handed coordinate system, where the z-axis is the up vector)

(upcoming Python-y pseudo-code)

### Getting angle `theta` from 3D vector `V`
```
def getSphericalTheta(V):
    return arccos(V.z)
```
* Should ensure that `V_z` is normalized (has length of 1)

### Getting angle `phi`
```
def getSphericalPhi(V):
    return arctan(V.y, V.x)
```
* Ensure that phi's range is between 0 and `2*pi`

### Getting cos(theta), sin(theta)
Sometimes we just want the value directly: `cos(theta)`, `sin(theta)`, `cos(phi)`, `sin(phi)`, instead of the angle values themselves.

Recall that we previously got `cos(theta)` from this relationship:

![cos theta](images/cos-theta.png)

```
def getCosTheta(V):
    return V.z
```

For `sin(theta)`, we can use the *Pythagorean theorem*, also knowing that the vector is unit length (length = 1):

![sin theta](images/sin-theta.png)

```
sin(theta)^2 = 1 - cos(theta)^2
```

Then we can simply take the square root of `sin(theta)^2`:

```
def getSinTheta(V):
  return sqrt(1 - getCosTheta(V))
```

### Getting cos(phi) and sin(phi)
Recall where angle `phi` is:

![Recall angle phi](images/recall-angle-phi.png)

The values of `cos(phi)` and `sin(phi)` (as seen from the top):

![cos phi and sin phi](images/top-view-angle-phi.png)

The vector `V` creates a shadow on the xy-plane:

![Projection](images/projection-vp.png)

This shadow is called the **projection** of `V`, which we will call `V_p`.

A few things to notice about the projection `V_p`:
* It's length is related to the angle `theta`
  * The closer `theta` is to `pi/2` (90 degrees), the longer the projection becomes
  * The closer `theta` is close to 0 or `pi` (180 degrees), the shorter the projection becomes
* `V_p` does not always have unit length (length of 1)
  * So we can't directly use `arctan()`

#### Normalizing the vector `V_p`

Recall the relationship of `sin(theta)` when we rotated the z-axis 90 degrees. If we rotate the view back, we can see that `sin(theta)` is the *length* of the projection of vector `V`:

![Sin theta is the projection](images/sin-theta-is-projection.png)

To normalize the projection vector `V_p`, we can divide each coordinate of `V_p` with it's length, which is equal to `sin(theta)`.

After we normalize `V_p`, the x and y coordinates of the normalized `V_p` can be used to find `sin(phi)` and `cos(phi)`:

![Vpx and Vpy](images/vpx-vpy.png)

```
def getCosPhi(V_p):
    length = getSinTheta(V_p) # length of projection
    return V_p.x / length
```

```
def getSinPhi(V_p):
    length = getSinTheta(V_p) # length of projection
    return V_p.y / length
```

(Ok soooo they don't tell you how you get the coordinates of the *projection* itself I guess....)
(I guess I better view [*dot products*](../03-math-operations-points-vectors/README.md#how-to-compute-the-dot-product))
* 3B1B explained how the dot product is related to projection, not this textbook...
* Or actually maybe projecting a vector onto a *plane*
* TODO: figure that out.

