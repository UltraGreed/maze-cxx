Simple pseudo-3D ray-casting demo made with C++/SFML as educational project.

https://github.com/UltraGreed/theBestLabyrinth/assets/35086784/a3c8ac07-8c3e-4d08-9b9f-fc9351cdb4f7

Details
-------

Uses ray casting for rendering.

New maze is generated each run with [Recursive division method](https://en.wikipedia.org/wiki/Maze_generation_algorithm#Recursive_division_method).

Controls reference [Legend of Grimrock](https://store.steampowered.com/app/207170/Legend_of_Grimrock/): cell-based movement with always exactly 2 inputs in buffer.

Usage
-----

Follow [SFML](https://github.com/SFML/SFML) installation instructions for your system.

Then clone and compile project with CMake:

    git clone https://github.com/UltraGreed/maze-cxx &&
    cd maze-cxx &&
    mkdir build &&
    cd build &&
    cmake .. &&
    make &&
    ./maze

Use WASD to walk, Q/E to rotate, ESC to exit.

Configuration can be provided via GameHeaders/config.h file. Generated maze in saved as mazemap.txt

See also
--------
- maze2-zig - coming soon!

