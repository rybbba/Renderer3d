# Renderer3d

## Building Renderer3d
You need to have a C++ compiler (supporting C++20), CMake and a `make` installed
as well as [SFML dependencies](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php#installing-dependencies).

Update repository Git submodules:
```
git submodule sync
git submodule update --init
```

then run
```
mkdir build && cd build
cmake ..
make
```
from Renderer3d root directory.