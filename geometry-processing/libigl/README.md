# libigl

Learning how to use [libigl](https://libigl.github.io/).

Coding is broken up into exercises into respective directories (`e1`, `e2`, etc.).

## Troubleshooting

### Building and Running Tutorials

(*edit*) Ok... the best solution was to just stick with Linux Ubuntu 😆;;;

Configure CMake (on Linux):
```
cd libigl/
mkdir build
cd build
cmake ../
```

To see all the tutorials (from libigl/build):
```
cd tutorial
cmake --build . --target help
```

To build one tutorial (for example, 101_FileIO), in libigl/build/tutorial, run:
```
cmake --build . --target 101_FileIO
```
This will take a while, like > 10 minutes, the first time you compile anything. Afterwards it is fairly quick.

Then to run the tutorial:
```
../bin/101_FileIO
```

---

(I'll just keep the nonsense below for record purposes...)

(on MacOS)

First I had a CMake version that was too old, so I ran:
```
brew upgrade cmake
```

which installed:
```
$ cmake --version
cmake version 4.0.0
```

Trying to build the tutorials from the [documentation](https://libigl.github.io/tutorial/):
```
cd libigl/
mkdir build
cd build
cmake ../
make
```

There were a bunch of errors on "minimum CMake version required needs to be >= 3.5" so I manually updated to `cmake_minimum_required(VERSION 3.5)` in various `CMakeLists.txt` files.

Now running into:
```
CMake Error at cmake/igl/modules/copyleft/cgal.cmake:19 (target_link_libraries):
  Target "igl_copyleft_cgal" links to:

    CGAL::CGAL_Core

  but the target was not found.  Possible reasons include:

    * There is a typo in the target name.
    * A find_package call is missing for an IMPORTED target.
    * An ALIAS target is missing.

Call Stack (most recent call first):
  cmake/igl/igl_include.cmake:32 (include)
  cmake/igl/igl_include.cmake:40 (_igl_include_full)
  cmake/libigl.cmake:27 (igl_include_optional)
  CMakeLists.txt:151 (include)
```

I then tried to install [`cgal`](https://www.cgal.org/):
```
brew install cgal
```

but that did not fix the problem... The libigl docs also say that "CGAL has been notoriously difficult to setup" so...

Ok, giving up on the tutorials for now and will try to build the simplest libigl program...
