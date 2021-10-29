This code prints a rainbow cube, or a rainbow dragon skull. Can also show various lighting solutions.

The code is both compiled and run by build.sh
    ./build.sh
    sh build.sh
Both above commands should compile, run, and clean up after this program.

It requires the modified version of cube.obj that is included in this zip.
The unmodified dragon.obj included in this zip works as well.

In order to change the object shown you must modify the file name on line 93 of assignment3.cpp
    To show cube:
        Do not modify
    To show dragon skull:
        Replace line 93 with
            FILE * file = fopen("dragon.obj", "r");

When the code is run, there are three ways to interact with the image. 
    The UP and DOWN arrow keys will scale the object shown
    The MOUSE can rotate the object
    The Z key will show true color
    The X key will show z-depth in grayscale
    The C key will show Phong shading
    The V key will show Gouraud shading
