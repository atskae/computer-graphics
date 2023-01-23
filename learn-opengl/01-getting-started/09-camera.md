# [Camera](https://learnopengl.com/Getting-started/Camera)

There isn't really a concept of a *camera*, instead, you just make everything in the scene move in reverse to give the *illusion* that we are moving a camera around.

## Camera and View Space

Create a coordinate system with the camera's position as the origin.

To create a coordinate system of the view/camera space, we need vectors, each that describes:
* The position of the camera (the vector points at the camera)
* The camera's direction (sorta): which way is the camera facing (positive z-axis)
  * Subtract two vectors: the position of the camera - the scene's origin
    * This results in a vector pointing from the scene's origin to the camera's position
    * So the camera's direction vector is actually facing *away* from the origin
* The right vector: the positive x-axis
  * The up vector: points upwards in world space
  * To compute the right vector, take the cross product of the up vector and the camera's direction vector
    * Order matters here, if we reverse the cross product arguments, we'd get the negative x-axis instead
* The up vector: the positive y-axis
  * Take the cross product of the camera's direction and the right vector

([Gram-Schmidt process](https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process))

## Look At Matrix

The Look At matrix transforms coordinates from world space to camera/view space. To create this matrix we need:
* The right vector
* The up vector
* The direction vector
* The camera's position

![ChatGPT negative zero](images/chatgpt-negative-zero.png)

It said "Sure!"! That was delightful... 🥲

![ChatGPT y-axis rotation](images/chatgpt-matrix-y-axis.png)

![ChatGPT asy](images/chatgpt-asy.png)

Wow, it even apologizes and understands where the confusion might have stemmed from 🤯

![ChatGPT up vector](images/chatgpt-up-vector-clarification.png)

We normalize the vectors of the coordinate system so that we don't apply any scaling.
```cpp
// Positive z-axis is outside of the screen
glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(
    cameraPosition - cameraTarget
);
```

When you create a `glm::mat`, pass in the vectors in column-major order. The values are stored in column-major order, so to print the martix to the console (so we have to do this by row):
```cpp
// Print in column-major order
void print_matrix(std::string info, glm::mat4 matrix) {
    std::cout << info << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << std::setw(2) << matrix[j][i] << " "; // i and j are swapped
        }
        std::cout << std::endl;
    }   
}

```

Rotate radius 10:

![Rotate radius 10](images/rotate-radius-10.png)

Rotate radius 5:

![Rotate radius 5](images/rotate-radius-5.png)

## Walk Around

[*Strafe effect*](https://en.wikipedia.org/wiki/Strafing_%28video_games%29) when using the camera: the act of moving sideways in a video game while still looking at a constant target
* Ex. moving sideways while looking at an enemy

WASD to move the camera:

```cpp
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    // Goes forward, into the screen
    // cameraSpeed * cameraFront = negative
    cameraPosition += cameraSpeed * cameraFront;
} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    // Goes backwards, away from the screen
    // cameraSpeed * cameraFront = negative
    // -= negative is addition, positive z-axis is away from the screen
    cameraPosition -= cameraSpeed * cameraFront;
} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    // Move left
    // Get the direction of the positive x-axis
    // Normalize so that we don't apply a scaling effect while moving camera
    glm::vec3 rightVector = glm::normalize(glm::cross(cameraUp, cameraFront));
    cameraPosition += cameraSpeed * rightVector;
} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    // Move right
    glm::vec3 rightVector = glm::normalize(glm::cross(cameraUp, cameraFront));
    cameraPosition -= cameraSpeed * rightVector;
} 
```

### Movement Speed

Depending on the machine, the number of frames/sec that is rendered in the render loop differs.
So having a constant for `cameraSpeed` will give a different camera movement speed depending on the machine.

`deltaTime` is the time it took to render the current frame. If this value is high, then we increase the velocity of the `cameraSpeed` to make up for the longer render time.

```
Camera speed: 0.042851
Camera speed: 0.0396228
Camera speed: 0.0436187
Camera speed: 0.043993
Camera speed: 0.0410223
Camera speed: 0.0418043
Camera speed: 0.0422931 
```

## Look Around

(*look around, how lucky we are to be alive right now* 💃🎶)

**Euler angles** are three angles that can represent any rotation in 3D:
* **Pitch**: how much we are looking up/down
  * rotation around the x-axis
* **Yaw**: how much we are looking left/right
  * rotation around the y-axis
* **Roll**: how much we are rolling (like in a space-flight camera)
  * rotation around the z-axis

In our camera, we only care about the *pitch* and *yaw* values for now.
* Using the pitch and yaw, we can compute a new 3D direction vector

### Mouse Movement

The greater the difference of the previous mouse position and the current mouse position, the greater the angle change
* Horizontal mouse movement to change the *yaw* angle (rotating around the y-axis).
* Vertical mouse movement to change the *pitch* angle (rotating around the x-axis)

**Capturing mouse movement** means to keep the mouse cursor in the center of the window. To hide and capture the mouse in GLFW:
```cpp
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
```

* `xpos` goes from left to right
* `ypos` goes from top to bottom

([source](https://www.glfw.org/docs/3.0/group__input.html#ga01d37b6c40133676b9cea60ca1d7c0cc))


#### Pitch Angle

Why does pitch influence the x-coordinate?

```cpp
// Direction vector for camera
direction.x = cos(yawRadians) * cos(pitchRadians);
```

~~You need `cos(pitchRadians)` for the camera controls to feel smooth... (without the view spins off a decent amount), but other than that I'm not sure why it works mathematically...~~

The yaw angle (rotation around the y-axis) determines the axis that the pitch angle will rotate around.
The pitch angle is not constrained to the world coordinate's x-axis.

A helpful picture from the comments!! (TIL read the comments (this ain't YouTube))

![Why pitch afffects x-coordinate](https://uploads.disquscdn.com/images/58fa1f1a3dd8d736a9345b3b168dd55caf0f14d485e9dae7e06b8e185348a42a.png)

`Φ` is the yaw angle, and this rotates *first*.
`θ` is the pitch angle.


### Zoom

[GLFW read in mouse scroll wheel input](https://www.glfw.org/docs/latest/input_guide.html#scrolling)
* `yoffset` is negative when scrolling down
  * Positive when scrolling up

If we decrease the field of view, then we zoom in and see less of the scene.
* Increasing field of view zooms out to see more of the scene.

```cpp
fieldOfView -= yoffset;
```
A few things going on here:
* Zoom
* How much of the scene we can see
* Field of view angle/value

Clarification:
* Scrolling down (negative `yoffset`) *increases* the field of view (we do `fielOfView =- yoffset`)
  * Increasing the field of view zooms *out*
  * Zoom *out* to see *more*

## Exercises

### First-person Shooter (FPS) camera
<sidenote> I tried to look up FPS camera (the word "shooter" is never mentioned in the chapter) and I only got "frame rate per second, camera", so I asked ChatGPT what FPS stood for in this context and it gave me the right answer!!! </sidenote>

To have a first-person shooter camera, we just need to prevent any camera movement in the y direction. The user should still be able to look up and zoom in and out.
```cpp
// moveForward(), moveBackwards(), moveLeft(), moveRight()
if (isFPS) this->position.y = 0; // einfach!
```

### LookAt Function

Took forever to debug... had to cheat a bit and I looked at the [`glm::lookAt` source code](https://github.com/g-truc/glm/blob/b3f87720261d623986f164b2a7f6a0a938430271/glm/ext/matrix_transform.inl#L98-L119)

Before moving left and right would curve, and the scroll/looking out would revert its direction at some point, and looking up and down was more limited in range.
```cpp  
glm::vec3 directionVector = glm::normalize(this->position - this->front);
```

This was the fix!
```cpp
glm::vec3 cameraTarget = this->position + this->front;
glm::vec3 directionVector = glm::normalize(this->position - cameraTarget);
```

I'll look into this more later
```
Direction vector: vec3(0.297046, 0.275637, -0.914214)
Direction vector incorrect: vec3(0.139801, 0.129725, 0.981645)
```

#### Camera direction vector != target position
The camera's *direction vector* is the final vector that is used as the camera's z-axis in the lookAt matrix

```cpp
// Where the camera sits, positive z-coordinate = outside the screen
glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
// What the camera is looking at
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);

// The direction vector that is used to build the lookAt matrix
// Points in the *opposite* direction of the target position
// Vector subtraction: X - Y = a vector from Y->X
// cameraTarget -> cameraPos
glm::vec3 cameraDirection = glm::normalize(
  cameraPos - cameraTarget
);
```

The second argument to `glm::lookAt()` is the *target position*, not the direction vector. GLM will compute the direction vector for you. The above was just showing you how you'd compute the direction vector yourself.

Later we break up `cameraTarget` so that we can change it as we move around the scene.

The `cameraTarget` is the exact position we are looking at, it does not need to be inverted (like the direction vector).

```cpp
// Negative z-axis is into the screen
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
// The camera sits outside the screen: cameraPos(0.0f, 0.0f, 3.0f)
glm::vec3 cameraTarget = cameraPos + cameraFront;
glm::lookAt(
  cameraPos,
  cameraTarget,
  upVector
);
```

The many confusions:
* When a `vec3` is used as a vector or a position
* When to invert the vec3 or not
* Using `direction vector` to mean different things (camera's *actual* direction vector as a row in the lookAt matrix, or the camera's front vector)
* The up vector of the world versus the up vector of the camera
  * Moving left/right maybe it just doesn't matter
  * For the lookAt, we compute a new up vector
