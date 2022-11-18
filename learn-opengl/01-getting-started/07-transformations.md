# [Transformations](https://learnopengl.com/Getting-started/Transformations)

(mostly reviewwwww)

## In Practice

[OpenGL Mathematics library, GLM](https://glm.g-truc.net/0.9.9/index.html), to perform math operations tailored for OpenGL.

1. Download the `.tar.gz` from [the GitHub repository](https://github.com/g-truc/glm/tags)
2. Extract the `.tar.gz`
3. Move the `glm/` directory (that is inside the extraacted directory) to `/usr/include`:
```
sudo mv glm-0.9.9.8/glm /usr/include/
```

Play around with mathmematics library in `glm_playground.cpp`, then compile and run:
```
g++ glm_playground.cpp -o glm_playground
./glm_playground
```
