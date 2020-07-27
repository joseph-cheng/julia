# julia
Julia set generator written in C++ usnig OpenGL

Press 'c' to toggle colour cycling (epilepsy warning) and space to pause the oscillation of a and b

## Compilation

First, compile glfw for your system.

Put the resulting static library in `lib/`, then run the following commands

`$ mkdir build`

`$ cd build`

`$ mv ../src/*.glsl .`

`$ cmake ..`

`$ make`

You can now run using `./fractal`

