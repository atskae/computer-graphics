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

The ranges that go through the x-axis slab and the y-axix slab for the ray `r0` does not overlap because it does not go through the AABB.

The ray `r1` does have an overlap of where the ray enters the x-axis and y-axis slabs, and we can see that does go through the AABB


* IEEE floating point, where `inf` means *infinity*, and `NaN` means "not a number"
  * `inf/0 = inf`
  * `0/0 = NaN`

## Links
* [Ray Tracing: the Next Week (blog post)](https://in1weekend.blogspot.com/2016/01/ray-tracing-second-weekend.html)
  * Extra links to supplemental reading and advice
