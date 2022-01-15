# [Math Operations on Points and Vectors](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/math-operations-on-points-and-vectors)

## Vector Length
A vector `V` indictates:
```
A ----------> B
```
* direction from point A to B
* distance/length between point A and B = `|| V ||`
    ```
    || V || = sqrt(V.x^2 + V.y^2 + V.z^2)
    ```
* In mathematics, double bar `||` indicates the **vector length**
  * Length is also called the **norm** or **magnitude**

* The axes of the 3D cartesian coordinate system are *unit vectors*

## Normalizing a Vector

* A **normalized vector**/**unit vector** is a vector whose length is 1 = ˆV
    * TIL: on a Mac: `Option + I + letter` = circumflex mark Û
* Normalize a vector by dividing the vector by its length:    
    ```
    ˆV = V / || V ||
    ```

<img src="images/vectors-unit-vector.png">

* On a computer, multiplying is less costly than division
  * Better to compute `1/||V||` once and multiply with that value than dividing by `||V||` for each value

* **norm** can also mean the function that assigns a length/size/distance to a vector
* `ˆV = V / || V ||` is called the **Euclidean norm**

## Dot Product


## Questions
* Why is division more expensive than multiplication?

