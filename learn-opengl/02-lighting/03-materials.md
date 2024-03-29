# [Materials](https://learnopengl.com/Lighting/Materials)

Different materials react to light differently
* ex) steel vs. wood
* How much light is reflected, scattered, highlights

We need to define **material properties** specific to each surface, containing:
* The three components of Phong shading: ambient, diffuse, and specular
  * As colors `vec3`
* Shininess `float`
* These get manipulated in the fragment shader
 
The properties:
* **Ambient**: the color under ambient lighting, usually the color of the object
  * What color the surface reflects under ambient lighting
* **Diffuse**: the color under diffuse lighting
* **Specular**: the color the highlight
* **Shininess**: the scattering/radius of the specular highlight

Some material properties defined here: http://devernay.free.fr/cours/opengl/materials.html
(ex. emerald, gold, black plastic)

## Setting materials
A `struct` in GLSL is just a namespace, so you have to set the struct fields individually
```cpp
lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
```

Doesn't look right :/ ~~The specular/shininess isn't there~~ Shininess was at 0.32f, oops

![Material properties 1](images/materials-properties-1.png)

![Material properties 2](images/materials-properties-2.png)

Shiniess at 4:

![Materials properties 3](images/materials-properties-3.png)

Still looks off...

### Light properties
We also need to set the intensities of the *light source* itself for each of the components: ambient, diffuse, and specular.

Here we lowered the ambience intensity to 0.2, diffuse to 0.5, and specular to 1.0:

![Light intensities](images/with-light-intensities-1.png)

Other angles... none of these look right :/

![Light intensities](images/with-light-intensities-2.png)

![Light intensities](images/with-light-intensities-3.png)

### Different Light Colors

![Colors over time](images/colors-over-time.gif)

ChatGPT helped me with the `webm` to `mp4` conversion !

```
ffmpeg -i my-recording.webm -vf "scale=788:trunc(ow/a/2)*2"  my-recording.mp4
```
(Can't embed mp4, sometimes gifs turn out not right...)

Some settings over time:
```cpp
lightColor.x = sin(glfwGetTime() * 2.0f);
lightColor.y = sin(glfwGetTime() * 0.7f);
lightColor.z = sin(glfwGetTime() * 1.3f);
// ...
glm::vec3 ambientColor = lightColor * glm::vec3(0.2);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5);
lightingShader.setVec3("lightColor", lightColor);
lightingShader.setVec3("light.diffuse", diffuseColor);
lightingShader.setVec3("light.ambient", ambientColor);

```

![Diffuse ambient over time](images/diffuse-ambient-over-time.mp4)

Added this line:
```cpp
lightingShader.setFloat("material.shininess", sin(glfwGetTime()));
```

Weird bug:

![Shininess bug](images/shininess-bug-overtime.mp4)

If the `shininess` is negative (from the `sin()` calculation) then it renders this bug:

![Negative shininess](images/negative-shininess.gif)

Keeping the light color constant and with negative shininess (-0.5):

![Constant light negative shininess](images/constant-light-negative-shininess.gif)

#### Other Materials

Found the bug on why the colors were slightly off... I was still using `objectColor` in the fragment shader's ligthing calculation:
```glsl
FragColor = vec4(objectColor * lightEffect, 1.0f);
```

Removed `objectColor` and it looks like the dull brown:

![Removed objectColor](images/removed-object-color.png)

Emerald:

![Emerald 1](images/emerald-1.png)

![Emerald 2](images/emerald-2.png)

Cyan plastic, looks off:

![Cyan plastic 1](images/cyan-plastic-1.png)

![Cyan plastic 2](images/cyan-plastic-2.png)

I never got the specular highlight right...

I forgot to normalize the light direction...
```glsl
vec3 reflectionDirection = reflect(-normalize(lightDirection), normalVec); 
```

Now it  makes sense.... I think the colors were scaled by some factor, all exceeding 255/white.

Shininess at 32:

![Specular 32](images/specular-fix-32.png)

Shininess at 256:

![Specular 256](images/specular-fix-256.png)

Forgot to reset the lighting intensities to `vec3(1.0)`. Gold material:

![Gold](images/gold.png)

Cyan plastic:

![Cyan plastic fixed](images/cyan-plastic-fixed.png)
