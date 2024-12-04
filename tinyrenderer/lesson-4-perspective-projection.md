# [Lesson 4: Perspective Projection](https://github.com/ssloy/tinyrenderer/wiki/Lesson-4:-Perspective-projection)

## [Linear Transformations](https://github.com/ssloy/tinyrenderer/wiki/Lesson-4:-Perspective-projection#linear-transformations)

A *shear* distorts a shape in a particular direction. To skew along the x-axis, we add a constant that is proportional to a y-value of a point `(x,y)`:
```
x' = x + s*y
y' = y
```

Any rotation can be represented as three shears.

Paper on this algorithm: [A fast algorithm for general raster rotation](http://graphicsinterface.org/wp-content/uploads/gi1986-15.pdf)

(according to ChatGPT) the derivation:

A rotation matrix can be represented as a horizontal shear, vertical shear, and another horizontal shear:

![Rotation as shears](images/rotation-as-shears.png)

We then solve for the three *shear factors*, `k1`, `k2`, and `k3`:

![3 shears matrix](images/three-shears-matrix.png)

![Equate shear entries](images/equate-shears-to-rotation-entries.png)

The shear factors:

![Shear factors](images/shear-factors.png)
