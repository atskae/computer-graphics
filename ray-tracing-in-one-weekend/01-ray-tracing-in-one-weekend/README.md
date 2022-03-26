# Ray-tracer
Following [Ray-tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) 🚀

Also learning CMake...

## How to run

1. Generate build files:
```
cmake -S . -B build
```

2. Build the project:
```
cmake --build build
```

3. Run the executable
```
./build/RayTracer
```


## Progress Log

### [2.2 Creating an Image File](https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage/creatinganimagefile)
I changed it up a bit so that the red -> row and green -> column (which makes more sense to me):

The function `print_ppm_file()` in `main.cpp` will generate an image.

Build the project (`cmake --build build`) then run the program while redirectiug the output to a file: `./build/RayTracer > image.ppm`. Then view the `image.ppm` in an image viewer.

![Hello World!](images/hello-world.png)

すごいいいいい 💃✨

### [5. Adding a Sphere](https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere)

![Funky Japanese flag](images/red_sphere.png)

This math makes sense... I'm crying 😭 schön... (I'm getting a サンダサンさん moment).
* [Math is fun - quadratic equation](https://www.mathsisfun.com/algebra/quadratic-equation.html)
  * Discriminant review
* [Line-sphere intersection](https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection)
  * Ok, the dot product part is kind of magic still...
  * I understand that you need the dot product to get a real number (as opposed to a 3D vector) but other than that... why does this work... Amazing...
* [Determining a vector given two points](http://mathonline.wikidot.com/determining-a-vector-given-two-points)


### [6. Surface Normals and Multiple Objects](https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects)

Made this by mistake... Time to debug 🔍

![Smash Ball!](images/smash_ball.png)

The bug was this line:
```c++
color ray_color(const ray& r) {
        // ...
        return 0.5 * color(normal.x(), normal.y(), normal.z());
```

I forgot the `+1` for each color channel... Without it, the color channels go out of range.

Fixed! 🛠

![Sphere shading map](images/sphere_shading_map.png)

#### Negative pixel values
The bug above was causing negative pixel values to be written to the PPM file. But I still couldn't explain why *those particular* image colors in the buggy image were rendered.

I generated an image where I had negative values in the blue channel at the top, and had the blue channel incrementally become more positive going down the image (keeping red and green at `0`). I drew white lines at certain points of interest (manually had to figure out. I drew an orange line at center height.

Running this:
```bash
python negative_pixel_values.py
```

Generates the following image:

![Negative pixel values](images/negative_pixel_values.png)

Totally weird. I'd think that negative pixel values would just do the reverse of the positive values, but there are random (?) places where the colors suddenly change (I drew white lines there). Also the most negative value `-255` isn't the full blue color as expected.


This is the red and blue channel values going in reverse (`red=-255` and `blue=255` at the top):

![Two channels](images/negative_pixel_values_two_channels.png)


🤷‍♀ 変なの。 

#### Sphere normal

If the sphere center = `C` and the closest point of intersection of the ray and the sphere is `P`, then the normal vector is:
```
P - C
```
This is the vector whose direction is from the center `C` to the surface `P`


## Resources
* [PPM image format](https://www.cs.swarthmore.edu/~soni/cs35/f13/Labs/extras/01/ppm_info.html)
* [CMake examples](https://github.com/ttroy50/cmake-examples/tree/master/01-basic)
