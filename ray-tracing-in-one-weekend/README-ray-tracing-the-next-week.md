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

## Links
* [Ray Tracing: the Next Week (blog post)](https://in1weekend.blogspot.com/2016/01/ray-tracing-second-weekend.html)
  * Extra links to supplemental reading and advice
