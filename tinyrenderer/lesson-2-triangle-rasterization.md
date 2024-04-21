# [Lesson 2: Triangle Rasterization and Back Face Culling](https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling)

## Filling Triangles

### Old-school Method: Line sweeping
We can use our line implementation to draw a triangle:

```cpp
void triangle(std::vector<Point> t, TGAImage& image, TGAColor color) {
    line(t[0], t[1], image, color);
    line(t[1], t[2], image, color);
    line(t[2], t[0], image, color);
}
```

![3 triangles](images/3-triangles.png)

Einfach! Now how to draw a *filled* triangle? ...

**Attempt 1**

Pick a corner of a triangle, and from that corner, draw a point to every point in the base of the triangle.

I modified the line drawing algorithm to return all the points that were used to draw the line.

There are still holes in the triangle... (looks funky-ly cool though)

![Filled triangle first attempt](images/filled_triangle_attempt_1.png)

**Attempt 2**

Choose the longest side, and draw a line from the other two sides

Still full of holes...

One side:

![Half filled 1](images/filled_triangle_attempt_2_half1.png)

The other side:

![Half filled 2](images/filled_triangle_attempt_2_half2.png)

Both halves:

![Full filled](images/filled_triangle_attempt_2_full.png)
