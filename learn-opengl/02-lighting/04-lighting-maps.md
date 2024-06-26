# [Lighting Maps](https://learnopengl.com/Lighting/Lighting-maps)

Objects in real life are made up of many *different* materials of different light/color properties
* Ex) car: window, frame, tires, etc. all react to light differently
* We need a *diffuse* and *specular* maps to define how each part of an object react to light

## Diffuse Maps
We are going to use the same process we used in texture mapping.

I reused the existing logic to load and set textures and updated the texture file path:
```cpp
const char* textureFilenames[] = {"textures/container2.png", "textures/linux-penguin-with-outline.png"};
```

![New crate  bug](images/texture-bug.png)

The old crate looks fine:

![Old crate](images/old-crate-texture.png)

So the image loading is probably incorrect, `jpg` versus `png` issue?
[Yup, forgot to update the format](https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors):
```cpp
GLenum imageFormats[] = {GL_RGBA, GL_RGBA};
```
`GL_RGBA` is for `png`, and `GL_RGB` is for `.jpg`.

Fixed!

![Texture fixed](images/texture-fixed.png)

It's brighter because I left the light attributes to `vec3(1.0)` from the previous chapter's exercise.


## Specular Maps

In our crate, the wooden part should not show specular highlights, while the metal border should.

We can use a *specular map* that defines the specular properties of each part of the object. 

Here we are using a black and white image, where white indiciates more intense specular effects. Black means no specular effect.

White `vec3(1.0)` will result in a larger value in the multiplication in the fragment shader, resulting in a brighter highlight. Black `vec3(0.0)` will result in no highlight.

### Sampling Specular Maps

Similar process to samping the diffuse map. We create a `Sampler2D` field for the diffuse attribute of our material and assign the diffuse map to the correct texture unit.

The border is shinier!

![Specular map](images/specular-map.png)

## Exercises

Ambient, diffuse, and specular lighting at `0.5`:

![0.5 lighting](images/0.5-lighting.png)

### ImGui (revisited)

Fixed the "not being able to interact with the GUI" problem!!". Went and re-read this [StackOverFlow post](https://stackoverflow.com/questions/71680516/how-do-i-handle-mouse-events-in-general-in-imgui-with-glfw), which one answer said you have to initialize ImGui *after* calling the GLFW callbacks (for mouse input, scroll input, etc.).

The UI is now at least draggable and interactable 😭 Amazing!

To not move the camera when hovering over ImGui, updated the GLFW mouse callback:
```cpp
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        camera.updateFrontVector(xpos, ypos);
    }
}
```

Made a click and drag camera (so only move the camera if mouse is pressed):
```cpp
// global
bool mouse_presed = false;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse && mouse_pressed) {
        camera.updateFrontVector(xpos, ypos);
    }
}

// Handler for checking when the mouse is pressed and released
void mouse_pressed_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouse_pressed = true;
    } else {
        mouse_pressed = false;
    }
}
```

The GLFW callback:
```cpp
glfwSetMouseButtonCallback(window, mouse_pressed_callback);
```

Controllable lighting:

![Controllable lighting](images/lighting-controls.gif)

### Inverted specular map

The naive first guess is to literally invert the values:
```glsl
vec3 texture_color = -vec3(texture(material.specular, TextureCoordinates));
vec3 specular = light.specular * texture_color * spec;
```
Red specular lighting turns green (opposite):

![Literally invert specular](images/literally-invert-specular.gif)

Actually inverting the map correctly:

We want white `(1.0, 1.0, 1.0)` to map to black `(0, 0, 0)`, so we just take the difference between the current texture color and the largest possible color `1.0`:

```glsl
vec3 texture_color = vec3(texture(material.specular, TextureCoordinates));
// Invert the specular map
for(int i=0; i<3; i++) {
    texture_color[i] = abs(texture_color[i]- 1.0);
}
vec3 specular = light.specular * texture_color * spec;
```
Now the wood is shiny!

![Inverted specular map](images/inverted-specular-map.gif)


### Creating a specular map from the diffuse texture
Simply replaced the current specular map with the colored one:
```cpp
const char* textureFilenames[] = {"textures/container2.png", "textures/lighting_maps_specular_color.png", "textures/linux-penguin-with-outline.png"};
```

Colored map:

<img src="../src/textures/lighting_maps_specular_color.png" alt="colored specular map" width="300"/>

You get the colored highlight:

![Colored specular map](images/colored-specular-map.png)

### Emission Map

I inverted the existing specular map to get the inner square's area and make it white:

<img src="../src/textures/container2_specular_inverted.png" alt="colored specular map" width="300"/>

This is to make sure the emission only occurs in the inner square and not the metal borders.

We add two fields in our Materials struct to hold the emission map and the emission area map:
```glsl
// Properties of a material surface
struct Material {
    // ...
    sampler2D emission;
    sampler2D emission_area;
    // ...
};

```

We load the textures in our C++ code, and set the texture units to the Materials struct in the fragment shader:
```c++
// Use the matrix emission map
lightingShader.setInt("material.emission", 2);
// Indicate where the emission occurs
lightingShader.setInt("material.emission_area", 3);

```

Then we attach these loaded textures to the texture unit in the render loop.

```c++
// Matrix emission map    
glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D, textureIds[2]);    

// Matrix emission map area
glActiveTexture(GL_TEXTURE3);
glBindTexture(GL_TEXTURE_2D, textureIds[3]);    

```

In our fragment shader, since we want to emit colors regardless of lighting we add the emission value on top of the final diffuse color:

```glsl
float diffuseStrength = max(dot(normalize(lightDirection), normalVec), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, TextureCoordinates)) +
        (vec3(texture(material.emission, TextureCoordinates)) * vec3(texture(material.emission_area, TextureCoordinates)));

```

![Emission](images/emission.png)

Got it to glow over time =D Just have an emission strength that changes over time using `abs(cos())`.

![Emission over time](images/emission_overtime.gif)
