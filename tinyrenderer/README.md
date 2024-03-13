# Tinyrenderer

Following the [tinyrenderer tutorial](https://github.com/ssloy/tinyrenderer/wiki/Lesson-0:-getting-started): "How OpenGL works".

## [Lesson 0: Getting Started](https://github.com/ssloy/tinyrenderer/wiki/Lesson-0:-getting-started)

Download the files from [this commit](https://github.com/ssloy/tinyrenderer/tree/909fe20934ba5334144d2c748805690a1fa4c89f).

Make sure the code builds with `make`. Running the generated binary will create an image:

![Initial output image](images/getting_started.png)

The top-left corner's coordinates is `(0,0)`. For an 100x100 pixel image, the bottom-right corner's coordinates is `(100, 100)`. So we call `image.flip_vertically()` so that the bottom-left corner becomes the origin.

I created a script `run.sh` that builds and runs the renderer, and saves the images as a PNG.

## [Lesson 1: Bresenham's Line Drawing Algorithm](https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm)


### First Attempt with Step
`line_with_step`: Drawing a line from `(0,0)` and `(40,40)` with various step sizes between colored pixels.

(Note that the images in this section did not apply `image.flip_vertically()`, so the origin is at the top-left corner)

With a step size of `0.01`:

![Line with step 0.01](images/line_with_step_0.01.png)

With a step size of `0.1`:

![Line with step 0.1](images/line_with_step_0.1.png)

Step size of `0.01` between `(0, 70)` and `(70,30)`:

![Another line](images/line_with_step_0.01_2.png)

### [Second Attempt with Calculated Step](https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm#second-attempt)

(`image.flip_vertically()` is applied going forward, so the origin is now at the bottom-left corner).

This attempt is doing [linear interpolation](https://en.wikipedia.org/wiki/Linear_interpolation) between the two points.

Three lines were drawn:
* White line: `(13,20)` -> `(80,40)`
* Red line: `(20,13)` -> `(40,80)`
* Red line: `(80,40` -> `(13,20)` (reversed coordinates of the first line, but should render exactly the same)

Only the first two lines were actually drawn, and the second line is spaced out:

![Linear interpolation](images/line_linear_interpolation.png)

The third line was not drawn because `x0` is greater than `x1`, so it never goes into the loop:
```cpp
for (int x=p0.x; x<=p1.x; x++) {
    //
}
```

The steeper the line, the more spaced out the points are. This is because we sample whole number `x` values between `x0` and `x1`. If they are close to each other, the range of `x` values to sample is smaller, so we sample less points.

Drawing some lines to compare the spacing:

```cpp
// Steep lines
line_with_calculated_step(Point(20, 13), Point(23, 80), image, red);
for (int i=0; i<10; i++) {
	int diff = 15* i;
	TGAColor color = red;
	color.b = red.b + (diff + 50);
	color.r = red.r - (diff + 30);
	line_with_calculated_step(Point(20, 13), Point(20+diff, 80), image, color);
}
```

![Steep lines with calculated step](images/steep_lines_with_calculated_step.png)


### [Third Attempt with Swapping](https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm#third-attempt)


First we choose which axis has a large range (`x1-x0` or `y1-y0`) so that we sample enough points to reduce the spacing. Then we interpolate over that axis.

![Steep lines with swap](images/steep_lines_with_swap.png)

Now the three lines render correctly, with the third line drawn over the first white line:

```cpp
line_with_swap(Point(13, 20), Point(80, 40), image, white);
	line_with_swap(Point(20, 13), Point(40, 80), image, red);
	line_with_swap(Point(80, 40), Point(13, 20), image, red);
```

![Line with swap](images/line_with_swap.png)

### [Timings: Fourth Attempt](https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm#timings-fourth-attempt)

We profile our code by running the line drawing steps `1,000,000` times:
```cpp
for (int i=0; i<1000000; i++) {	
    line_with_swap(Point(13, 20), Point(80, 40), image, white);
    line_with_swap(Point(20, 13), Point(40, 80), image, red);
    line_with_swap(Point(80, 40), Point(13, 20), image, red);
}
```

We use `gprof` to profile our code.

First I updated the Makefile to use the profiling flags:
```make
CPPFLAGS     = -ggdb -g -pg -O0
LDFLAGS      = -pg
```

Then we run the executable as normal, which then generates a `gmon.out` file.

Then we pass in the executable and `gmon.out` to `grof`:
```
gprof tinyrenderer gmon.out > profile.out
```

We can then view the profiling analysis:

```
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 57.93      1.90     1.90  3000000     0.00     0.00  line_with_swap(Point, Point, TGAImage&, TGAColor)
 29.27      2.86     0.96 204000000     0.00     0.00  TGAImage::set(int, int, TGAColor)
  7.93      3.12     0.26 207000000     0.00     0.00  TGAColor::TGAColor(TGAColor const&)
  2.74      3.21     0.09                             _init
  1.68      3.27     0.06  6000000     0.00     0.00  Point::Point(int, int)
  0.30      3.27     0.01                             main
  0.15      3.28     0.01        2     2.50     2.50  TGAColor::TGAColor(unsigned char, unsigned char, unsigned char, unsigned char)
```
