# Instalar OpenGL

sudo apt-get install libglew1.5-dev freeglut3-dev libglm-dev

# Bugs conhecidos

Se ao compilar apresentar o(s) seguinte(s) erro(s): 

The following variables are used in this project, but they are set to NOTFOUND.
Please set them or make sure they are set and tested correctly in the CMake files:
GLUT_Xi_LIBRARY (ADVANCED)
linked by target "SketchWOGL" in directory /home/me/devl/c++/game/2DGame/src
GLUT_Xmu_LIBRARY (ADVANCED)
linked by target "SketchWOGL" in directory /home/me/devl/c++/game/2DGame/src

Instalar:
    sudo apt-get install libxmu-dev libxi-dev
