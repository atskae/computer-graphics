# [Hidden Faces Removal (z-buffer)](https://github.com/ssloy/tinyrenderer/wiki/Lesson-3:-Hidden-faces-removal-(z-buffer))

The problem with our current renderer is that the triangles must already be in the correct depth order relative to a fixed position of the camera for them to be rendered correctly. Otherwise, we'd have triangles that are supposed to be behind other triangles drawn over previously-drawn triangles, even if the triangles are closer to the camera.

It's not efficient to always sort triangles by depth ([painter's algorithm](https://en.wikipedia.org/wiki/Painter%27s_algorithm)) - scenes can change dynamically, and re-sorting triangles at every scene change is expensive. Also it might not always be possible to sort triangles by depth if triangles intersect into each other in a certain way.

Let's say we have a 3D scene like this, with three triangles, and a square to represent the screen (images from tinyrenderer):

![Three triangles in a 3D scene](https://raw.githubusercontent.com/ssloy/tinyrenderer/gh-pages/img/03-zbuffer/d493c52da4cabe9a057c26f696784956.png)

If we look at this scene from the top (at some height `y` along the y-axis) we'd see:

![Top view of the 3D scene](https://raw.githubusercontent.com/ssloy/tinyrenderer/gh-pages/img/03-zbuffer/023668cb8ea97f59bf87d982c1e8b030.png)

We can't sort the triangles by distance to the camera - is the blue triangle closer or is the green triangle closer? Is the red triangle in front of the blue triangle? Etc.

## [Draw widthx1 image of the scene](https://raw.githubusercontent.com/ssloy/tinyrenderer/gh-pages/img/03-zbuffer/023668cb8ea97f59bf87d982c1e8b030.png)

Let's try to draw one line of pixels of the scene viewed from some height `y` along the y-axis (as above).

Essentially we are putting in a plane along the y-axis in the scene:

![Put a plane into the scene along the y-axis](https://raw.githubusercontent.com/ssloy/tinyrenderer/gh-pages/img/03-zbuffer/d673f40bcadbe53f4b3cb29bbbcfb461.png)

and we only draw the pixels that are within this plane and the scene viewed from the top:

![One line of pixels of the scene, as viewed from the y-axis](https://raw.githubusercontent.com/ssloy/tinyrenderer/gh-pages/img/03-zbuffer/3d4c4a1710b8e2558beb5c72ea52a61a.png)

How would we render just that one line of pixels?

We need to consider the height *of the position* of each part of the triangle. If we look at the 3D scene from the side, we can more easily see the height of the triangle's position:

![3D scene from the side](https://raw.githubusercontent.com/ssloy/tinyrenderer/gh-pages/img/03-zbuffer/20e9d8742d17979ec70e45cafacd63a5.png)

We need to keep track of the largest height position seen of each pixel along the x-axis (this is a sliding window across the x-axis of the widthx1 image). If a part of a triangle's position is shorter than another triangle's position, then we do not color that part of the triangle that is underneath that other triangle.

To keep track of the triangle position's height (y-values of each position in the triangle), we store them in a *y-buffer*, which is an array of `image_width` values, holding the position height seen at each x-value in the image. We are only trying to draw 1 line of the scene, so the image size is `image_width`x1.

Rendered the following lines:
```cpp
rasterize(Point(20, 34),   Point(744, 400), render, red,   ybuffer);
rasterize(Point(120, 434), Point(444, 400), render, green, ybuffer);
rasterize(Point(330, 463), Point(594, 200), render, blue,  ybuffer)
```

![Render a line with y-buffer](images/ybuffer_rendering.png)

In this scenario, since we are looking at the scene from the top, we set the y-coordinate to `0` when rendering.

## [Back to 3D](https://github.com/ssloy/tinyrenderer/wiki/Lesson-3:-Hidden-faces-removal-(z-buffer)#back-to-3d)

When rendering a 3D scene into a 2D image, we need a *z-buffer* that is 2D dimensional to keep track of the largest z-value for each pixel `(x,y)`.

**How do we compute the z-value?** Recall how we computed the y-value:
```cpp
float t = (x-p0.x)/(float)(p1.x-p0.x);
int y = p0.y*(1.0 - t) + p1.y*t;
```

(which I thought was just linear interpolation...) But turns out that `1-t` and `t` are the *barycentric coordinates* of the point `(x,y)` with respect to points `p0` and `p1`:
```
(x,y) = p0*(1-t) + p1*t
```

So to find the z-coordinate, we can apply the same technique, by multiplying the z-coordinates of each point in the triangle (`p0`, `p1`, `p2`) which it's respective barycentric coordinate.

We computed the y-coordinate by adding some percentage `1-t` of `p0.y` with some percentage `t` with `p1.y`.

We compute the z-coordinate by adding some percentage `1 - u - v` of `p0.z` with some percentage `u` of `p1.z` with some percentage `v` of `p2.z`.

First bug:

I did update the logic to compute the barycentric coordinates... looks like they don't even render triangles anymore...

![zbuffer bug 0](images/zbuffer_bug0.png)

The bug was holding the `cross_product` in an `int` when computing `[u, v, 1]` (which is a float), but storing `[u, v, 1]` in the cross_product vector.

Fixed but still not looking right:

![zbuffer bug 1](images/zbuffer_bug1.png)
