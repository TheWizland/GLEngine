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

Windows: Pre-compiled binaries recommended.

Linux: 

2a. cmake -S ./path-to-glfw3 -B ./build-directory

2b (Build Directory). make install

3. Install GLEW

https://glew.sourceforge.net/

4. Install GLM

https://github.com/g-truc/glm


Note: source.cpp provides a working code example, but 3rd party assets used have not been included.

Replacement assets will need to be added for source.cpp to run.
