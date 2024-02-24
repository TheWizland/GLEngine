# GLEngine

Rudimentary OpenGL framework. Somewhat WIP.

Requires GLFW, GLEW, and GLM. Uses stb_image library (included in repository).

GLFW: https://www.glfw.org/

GLEW: https://glew.sourceforge.net/

GLM: https://github.com/g-truc/glm

stb_image.h: https://github.com/nothings/stb/tree/master

**SETUP**
1. Install CMake

Windows: https://cmake.org/download/

Linux: sudo apt install cmake

2. Install GLFW3

https://www.glfw.org/download.html

https://www.glfw.org/docs/3.3/compile.html

Linux: 

2a. cmake -S ./path-to-glfw3 -B ./build-directory

2b (Build Directory). make install

3. Install GLEW

https://glew.sourceforge.net/

4. Install GLM

https://github.com/g-truc/glm

Windows: Place glfw3.lib and glew32.lib in /libs/

Place header files in /include/GLFW/, /include/GL/, and /include/glm/. Place stb_image.h in /include/

Note: source.cpp provides a working code example, but 3rd party assets used have not been included.

Replacement assets will need to be added for source.cpp to run.
