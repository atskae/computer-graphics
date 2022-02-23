## [Raytracing Algorithm in a Nutshell](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/raytracing-algorithm-in-a-nutshell)

Ibn al-Haytham explains why we see objects, and two interesting remarks:
* Without light we cannot see objects
* Without objects in our environment, we cannot see light 🔦
  * Traveling through intergalactic space
  * If there is no matter around us, we cannot see photons (only darkness), even if photons are moving through space

## Forward Tracing

Out of all the rays reflected by an object, only a select few of them ever reach our eyes.

Imagine a light source that emits a single photon at a time:
* The photon travels in a straight line until it hits that object (ignore photon absorption for now)
* The photon is then reflected in a random direction
* If that photon hits out eye, then we "see" that point on the object where that photon was reflected off of the object

![We only see objects if the reflected photon hits our eye](images/photon-hits-eye.png)

Photons being reflected in "every possible direction" ~= "random"
* If we look at the *micro-structure" of an object that looks smooth to our eyes, it appears complex and not smooth at all
  * Photons are so small that they are affected by the micro-structure of an object

The *macro-structure* of an object can be organized into patterns, causing photons to be reflected in a certain  way. Interesting new terms to be visited later:
* anisotropic reflection
* iridescence
  * Rainbow-like play of color (bubbles, oil)

![Bubble doodlez](images/crap-i-have-to-go-to-work.png)

