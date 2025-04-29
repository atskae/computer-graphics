# Exercise 1

## Build and Run

First configure CMake in the `e1` directory:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Then while still in the `build` directory, run:
```
make
```

This will build an executable `e1`:
```
./e1
```

## Adjacent Faces

We can find all the adjacent faces to a vertex with:
```cpp
igl::vertex_triangle_adjacency(V, F, VF, VFi);
```

Given a matrix of vertices `V` (row = vertex_index, col = actual vertex values themselves) and a matrix of faces `F` (for each face (row), a list of indices into `V` - these are the vertices of the face), `VF` will be populated with all adjacent faces for each vertex.

`VF` is an adjacency list, with:
```
VF[vertex_index] = [list of adjacent faces]
```

A verbose log of one vertex (of `cube.off`):
```
Vertex #1511 (-2.40159 0.608307  4.81405): 6 adjacent faces:
Face 357: (-2.40159 0.608307  4.81405), (-2.40159 0.608307  4.68905), (-2.52659 0.608307  4.68905), 
Face 1455: (-2.40159 0.608307  4.93905), (-2.40159 0.608307  4.81405), (-2.52659 0.608307  4.81405), 
Face 1456: (-2.27659 0.608307  4.93905), (-2.27659 0.608307  4.81405), (-2.40159 0.608307  4.81405), 
Face 1893: (-2.52659 0.608307  4.81405), (-2.40159 0.608307  4.81405), (-2.52659 0.608307  4.68905), 
Face 2992: (-2.40159 0.608307  4.93905), (-2.27659 0.608307  4.93905), (-2.40159 0.608307  4.81405), 
Face 2993: (-2.40159 0.608307  4.81405), (-2.27659 0.608307  4.81405), (-2.40159 0.608307  4.68905), 
```

Just logging the indices:
```
Vertex #1511
Face 357: 1511 1118  110
Face 1455:  396 1511  398
Face 1456:  108 1120 1511
Face 1893:  398 1511  110
Face 2992:  396  108 1511
Face 2993: 1511 1120 1118
```

The vertex index (so `1511` in the example above) is in every adjacent face (as expected).

The last parameter into `vertex_triangle_adjacency()`, `VFi`, tells us for each adjacent face, which vertex in the face is the vertex that we are inquiring about.

So for the example above:
```
Vertex #1511
Face 357: 1511 1118  110
Face 1455:  396 1511  398
Face 1456:  108 1120 1511
Face 1893:  398 1511  110
Face 2992:  396  108 1511
Face 2993: 1511 1120 1118
Index of incidence: 0, 1, 2, 1, 2, 0
```

For the first face (Face 357), Vertex #1511 is at index 0 of the face's vertices.
For the second face (Face 1455), Vertex #1511 is at index 1 of the face's vertices.
For the fifth face (Face 2992), Vertex #1511 is at index 2 of the face's vertices.
