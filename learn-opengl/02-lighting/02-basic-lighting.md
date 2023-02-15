# [Basic lighting](https://learnopengl.com/Lighting/Basic-Lighting)

Many models to approximate the effects of light on a computer.
* [Phong lighting model](https://en.wikipedia.org/wiki/Phong_reflection_model)
  * Published in 1975

Phong lighting model components:
* **Ambient**: in dark environments there is still some dim light source somewhere in the world (ex. moon, street lamp)
  * This gives color to an object in the dark
* **Diffuse**: the directional impact of the light source on the object
  * Parts of the object closer to the light = brighter, away = darker
  * Visually the most significant of the three
* **Specular**: the bright spot on shiny objects
  * Color more influenced by the light source's color than the object's color

When you combine the three components, you render objects under the Phong model.

## Ambient Lighting

There could be many light sources, some not even visible.
* Light can *reflect* off objects and illuminate other objects indirectly
* **Global illumination algorithms** model this effect, which can be expensive

**Ambient lighting** is a simplified global illumination model.
* Take a percentage of the light source's color
* Then you multiply this value with the objects color

In the fragment shader that renders the object (recall that object and light source have *different* shaders):
```glsl
float ambience_strength = 0.1;
vec3 ambience = light_color * ambience_strength;
vec3 final_object_color = object_color * ambience;
```

![Ambient lighting](images/ambient_lighting.png)

## Diffuse Lighting

Calculate the angle between the incoming light ray and the **normal vector** (a vector that is perpendicular to the object's surface) on the surface of an object:
* The *dot product* between two *unit* vectors, with an angle `θ` between the two vectors, is `cos(θ)`
* If the light is directly above the object, the angle between the object' normal vector and light is 0
  * `cos(0 degrees) = 1`
  * The object should be brightest when the angle between two vectors is closer to 0
* If the light and normal vector form a 90 degree angle, the result is `cos(90 degrees) = 0`
  * The object should be darkest

We use the scalar computed from the dot product to calculate the object's brightness on a fragment.
* The fragment's orientation toward the light gives us an angle

We need the two vectors:
* The normal vector off the object's surface
* The light's directional vector
  * Calculate by the difference between the position from the light source and the object
