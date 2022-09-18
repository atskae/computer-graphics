# [OpenGL](https://learnopengl.com/Getting-started/Creating-a-window)

## State Machine

OpenGL is one giant state machine
* Stores many variables that define how OpenGL should operate
* The current state is called its **context**

## Objects

The core of OpenGL libraries are written in C.
* Language bindings are built on top of this

An **object** is a subset of OpenGL's state
* The main OpenGL state (the *context*) consists of smaller *objects* to organize the types of settings that can be set
* Pseudo-code:
```cpp
struct OpenGL_Context {
    OpenGL_Object* color_settings;
    OpenGL_Object* window_settings;
    ...
};
```

Typical workflow to set values to a settings object:
1. Choose an integer for a unique ID
2. Generate a new object with that unique ID assigned to it
3. Bind the object to the target location in the OpenGL context (ex. create a mapping between the OpenGL context's window settings to the object via its uniqueId).
4. Set the object's settings
5. Unbind the object from the context (ex. set the uniqueId to the window settings to zero)

In graphics development, we create many objects to hold settings and image data (ex. 3D model data)
* We can bind and unbind the objects/settings when we need them