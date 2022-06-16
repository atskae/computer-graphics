# [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)

## [Motion Blur](https://raytracing.github.io/books/RayTracingTheNextWeek.html)

The **motion blur effect** is created by averaging the images of what the camera sees over a time interval
* Method originally introduced by [Rob Cook in 1984](https://graphics.pixar.com/library/DistributedRayTracing/)
* Generate rays at random times within some time interval

Went on some rabbit hole and found someone who [rendered black holes](https://rantonels.github.io/starless/).

Diffuse spheres with motion blur:

![Motion blur](images/motion_blur.png)

The camera now has a start time `time0` and end time `time1` (shutter open/close times), and the rays that the camera shoots out has a timestamp: a random value between `time0` and `time1`.

Any moving object will also have a start time and end time, and needs to be able to determine where it is at any time between `time0` and `time1`.

## [3.3 Axis-Aligned Bounding Boxes (AABBs)](https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/axis-alignedboundingboxes(aabbs))

あああ they mentioned "parallelepiped" 😆 (3b1b moment).
In this ray tracer, we will call "axis-aligned bounding rectangular parallelepiped" -> "axis-aligned bounding boxes", or AABBs.

* **slab method**: a "slab" is an axis-aligned interval
  * An **interval** are points between two endpoints
    * For example, the interval `x_0` to `x_1` on the x-axis is a "slab"
  * n-dimensional AABB is just an intersection of `n` axis-aligned intervals
    * In 2D, two intervals overlapping creates a 2D AABB rectangle:

    ![AABB formed with two intervals](images/form-aabb.png)

    * In 3D, the boundaries are *planes* (while in 2D, the boundaries are *lines*)

Recall a ray is just a line, with linear equation `P(t) = A + tb`, where:
* `A` is the origin point (an n-dimensional point)
* `b` is the direction of the ray (an n-dimensional vector)
* `P(t)` is a location/point in space

We can apply the ray equation to all three axes: x, y, and z. For example:
* The x-coordinate of `P(t)` (the location on the ray) is:
  ```
    x_0 = A_x + t_0 * b_x
  ```
* The y-coordinate of `P(t)`:
  ```
    y_0 = A_y + t_0 * b_y
  ```
* We can use this to find where the ray intersects the intervals/slabs

![Ray intersecting a slab](images/ray-intersect-slab.png)

To see if a ray intersects an AABB, we find the points where the ray intersects each interval (for example, the `t` values of the ray `P(t)` that intersects the interval/slab `(x_0, x_1)` is `t_0` and `t_1` in the example above). Each set of intersections creates a range `(t_0, t_1)`. If *all* of these ranges (a range per axis) overlap, then the ray intersects the AABB.

For example, in the 2D space:

![AABB intersection](images/ray-interval-overlaps.png)

The ranges that go through the x-axis slab and the y-axis slab for the ray `r0` does not overlap because it does not go through the AABB.

The ray `r1` does have an overlap of where the ray enters the x-axis and y-axis slabs, and we can see that does go through the AABB

### Intersections

If we had a slab/interval `(x_0, x_1)`, we have to find `t_0` that intersects `x_0` and `t_1` that intersects `x_1`.

![t values that intersect the intervals](images/t-values-intersection.png)

Since it is possible that the ray can be coming from the *negative* x direction, `t_0` and `t_1` can be reversed.
* We just have to make sure that after our initial calculation, we ensure that `t_0 <= t_1`.

#### Numerical Considerations
* IEEE floating point, where `inf` means *infinity*, and `NaN` means "not a number"
  * `inf/0 = inf`
  * `0/0 = NaN`
* If the ray does not intersect the slab, both `t_0` and `t_1` will be +- infinity.

#### Overlaps
If there is an overlap between the intervals that are created (`(d, D)` and `(e, E)`) after calculating where the ray intersects the slabs (`(x_0, x_1)` and `(y_0,y_1)`), then we know the ray intersects the AABB:

![Intersect AABB calculation](images/intersects-aabb-calc.png)

## [4.2 Texture Coordinates for Spheres](https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures/texturecoordinatesforspheres)

Spherical coordinates
* Longitude and latitude, *spherical coordinates* theta and phi: `(θ, ϕ)`
* Recall that the ray tracer uses the *right-handed coordinate system*

![Spherical coordinates](images/spherical-coordinates.png)

* Angle θ goes from the bottom pole, upward (from -y axis to +y axis)
* Angle ϕ goes around the y-axis
  * (from -x axis to +z axis, to +x axis to -z axis, then back to -x axis)

Texture coordinates (`(u,v)` in the `hit_record` class)
* We want to map the coordinate on the sphere `(θ, ϕ)` to the *texture coordinates* `(u,v)`, where both `u` and `v` are in range `[0,1]`.
* `(u=0,v=0)` would map to the bottom-left corner of the texture

![Spherical to texture coordinate](images/spherical-to-texture-coordinate.png)

`(u,v)` is between 0 and 1, so to convert from the spherical coordinates to `(u,v)`. we can normalize the spherical coordinates:

![Normalize spherical coordinates](images/normalize-spherical-to-uv.png)

### Cartesian Coordinates and Spherical Coordinates Relationship

We should first see how we use angle theta and phi to get the Cartesian coordinate `(x,y,z)`:

```
y = -cos(θ)
x = -cos(ϕ)sin(θ)
z = sin(ϕ)sin(θ)
```

#### Derivation

Review from [a while ago!](https://github.com/atskae/computer-graphics/tree/master/scratch-a-pixel/notes/02-geometry/08-spherical-coordinates-trig#converting-cartesian-to-spherical-coordinates) (need to be modified since those notes use the left-handed coordinate system, and z is up....)

Deriving `y = -cos(θ)`:

![y-coordinate from spherical](images/y-cartesian.png)

We need the negative sign in front of `cos(θ)` because `cos(θ)` gives us a negative number when the angle is in the positive y-axis

Deriving x and z coordinates:

Tried to do the same thing as above with the other coordinates but didn't get to the equation. Luckily ran in to [this video](https://www.youtube.com/watch?v=Ca-Rz6rZ2Hw) which led me to decompose the vector to get the remaining coordinates (the video uses a different coordinate system though).

Most examples online show the angle `θ`  starting at +y and going down... so we can use the [relationship of vertical angles](https://www.mathsisfun.com/geometry/vertical-angles.html) to flip the view of the angle `θ`:

![Vertical angles are equal](images/vertical-angles-are-equal.png)

Then we can project the vector created by angle `θ` to the -x/+z plane to draw a right triangle; we can see angle `ϕ` in relation to this triangle.

Using [geometry of parallel lines](https://www.mathsisfun.com/geometry/parallel-lines.html) we can find the angles of the right trangle that equal `θ`:

![Theta equals other angles](images/theta-equals-other-angles.png)

If we rotated this image, it is easier to see that the angles are corresonding angles, which makes them equal:

![Corresponding angles are equal](images/corresponding-angles-are-equal.png)

Now we can use the angle `θ` inside the right triangle and trig to find the length of the hypotenuse, which is just `sin(θ)`. To find the x and z coordinates, we then break apart the hypotenuse into its two components.

The following image is the top view. We can form *another* right triangle, now using angle `ϕ`:

![Deriving x and z coordinates](images/derive-x-and-z-coordinates.png)

The x-coordinate is the adjacent side of `ϕ`, and the z-coordinate is the opposite side of `ϕ`. As shown previously, the hypotenuse is `sin(θ)`. We can then use the trig relationships SOH and CAH (from SOH CAH TOA) to find the lengths of the triangle, which correspond to each coordinate x and z.

For the x-coordinate, we need to add a negative sign to `cos(ϕ)` since positive `ϕ` is in the negative x-axis, so we flip the sign.

NOOOICE.

### Getting angles θ and ϕ 

To find angle ϕ we use the ratio of the z and x coordinate values. `tan()` relates these two coordinate values (TOA)

As stated before, the "opposite" side of `ϕ` is the z-coordinate = sin(ϕ)sin(θ) and "adjacent" side of `ϕ` is cos(ϕ)sin(θ).

Since we now have tan(ϕ), we can take the inverse of tan(), arctan(), to get the angle ϕ.

![Finding angle phi](images/finding-angle-phi.png)

#### arctan2

C++'s arctan2() function gives a range [0, pi], then switches to [-pi, 0] after the angle surpasses pi.

![arctan2 range](images/arctan2-range.png)

To map the angles to the textured coordinates `u,v`, we need to convert the angles returned by atan2() to range to [0, 2*pi]

To do this, we simply flip the coordinate signs `(a,b)`, and add pi:

```
arctan2(a,b) = arctan2(-a, -b) + pi
```

![arctan2 range conversion](images/convert-arctan2-range.png)

First we get the delta angle value to either add/subtract from angle pi. This is obtained by flipping the coordinate `(a,b)` -> `(-a,-b)`, which is the same as a pi rotation.

If we had a positive angle (angle is less than pi), then the formula starts at pi and subtracts the delta angle amount to get the original angle (positive angles are already in range [0, 2*pi]).

If we had a negative angle (angle is between -pi and 0 in C++), we start at pi and add the delta angle amount (this gives us a positive angle value greater than pi).

![arctan2 range conversion explain](images/arctan2-range-conversion-explanation.png)

#### pi rotation (180 degrees)

Just another instance where math *beautifully* makes sense and the joy that an idiot like me can actually understand any of it... 😭 

[Review from a while ago!](https://github.com/atskae/computer-graphics/tree/master/scratch-a-pixel/notes/02-geometry/04-matrices#the-rotation-matrix)

Why `(a,b)` -> `(-a, -b)` is a pi rotation:

![Review pi rotation](images/review-pi-rotation.png)

It's the transformation matrices from months ago!

### Angle θ

Angle `θ` is easy! Just take the inverse cosine:
```
y = -cos(θ)
-y = cos(θ)
arccos(-y) = θ
```

#### C++ Nonsense
I forgot to put `public` before texture:
```
class solid_color : texture {
```
Got a spam of errors from the compiler that were not helpful.
I'm starting to *actually* *really* see the love for Rust...

## [4.3 A Checker Texture](https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures/acheckertexture)

Wowwwwwwwwww gotta figure out why the sine function and how? ???

![Checker](images/checker.png)

## Links
* [Ray Tracing: the Next Week (blog post)](https://in1weekend.blogspot.com/2016/01/ray-tracing-second-weekend.html)
  * Extra links to supplemental reading and advice
* [Bounding volume hierarchy (BVH)](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy)
