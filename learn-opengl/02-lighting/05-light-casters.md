# [Light Casters](https://learnopengl.com/Lighting/Light-casters)

## Directional Light

**Directional light** are light sources that are percieved to be *infinitely* far away. The rays from these light sources are all parallel and are always in the same direction, regardless of where the light source is (therefore, it's position is irrelevant). An example is the sun. The directional light's direction vector is the same for every object in the scene.

I set the diffuse setting to max to exaggerate the shadows more.

Directional light vector pointing down:

![Directional vector down](images/directional-light-down.png)

Directional light vector pointing up (simply make the y value positive):

![Directional vector up](images/directional-light-up.png)

Cool!! ☀️

## Point Lights

**Point lights** are light sources that have a defined position and scatters light rays in all directions. The strength of the light decreases as the the ray travels further away from the light source. Example: light bulbs in a room.

### Attenuation

**Attenuation** defines how much the strength of the light ray decreases as it travels further away from the light source.

The light is strong closer to the light source, then decreases more quickly near the light source, then slowly decreases again when it is far away. This is more realistic than linearly decreasing the lighting strength with distance.

Unless directional lights, the *position* is important in point lights.

Hmm... it's incorrect since the brightness changes based on the viewer's position, not light:

![Incorrect point light](images/incorrect-point-light.gif)

I updated the the light position to view-space in the vertex shader and used that in the fragment shader instead:
```glsl
LightPos = vec3(view * model * vec4(lightPos, 1.0f));
```

I had a bug here where it used `light.position` instead in the distance calculation:
```glsl
float dist = length(light.position - FragPos);
```

Though now everything seems lit:
Er, it seems light the light is comming from the bottom...

![Incorrect point light 2](images/incorrect-point-light-2.png)
