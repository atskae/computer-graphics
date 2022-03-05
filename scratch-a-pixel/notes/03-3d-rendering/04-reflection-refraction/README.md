# [Adding Reflection and Refraction](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/adding-reflection-and-refraction)

We can add other effects to the ray-tracing algorithm: **reflection** and **refraction**
* Simulate glass materials or mirror surfaces

["An Improved Illumination Model for Shaded Display" (Turner Whitted, 1979)](https://www.cs.drexel.edu/~david/Classes/Papers/p343-whitted.pdf) extends Appel's ray-tracing algorithm by adding reflection and refraction

We can see the effects of reflection and refraction on a glass ball
* If we know the direction of the ray that intersects the glass ball, we can compute what happens
* The effect is based on the *normal* at the point of intersection and the direction of the incoming ray (the *primary ray*)
* Need to calculate the **index of refraction** of the material

Rays usually travel in a straight line. **Refraction** causes these rays to *bend*
* Depending on the material's *index of refraction* the photon that hits the object changes its direction
