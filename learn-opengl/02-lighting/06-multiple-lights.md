# [Multiple Lights](https://learnopengl.com/Lighting/Multiple-lights)

In GLSL, we will move the logic of each type of light source (directional, point lights, flashlight) into its own GLSL function.
* Each type of light's effect is added to the fragment's color

```glsl
vec3 output_color(0.0);
output_color += calculate_directional_light();
output_color += calculate_point_lights();
output_color += calculate_flashlight();

// The final color of the fragment
FragColor = vec4(output_color, 1.0);
```

Review of types of light sources:
* Directional light - are approximated to be infinitely far away, has no position, just one direction of the rays (ex. the sun)
* Point lights - has a position and shines rays in all directions, light dims as the rays travels further away (ex. lightbulb)
* Spotlight - defines a radius of where only the fragments inside the radius are lit.
