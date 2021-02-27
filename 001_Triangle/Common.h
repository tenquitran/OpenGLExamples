#pragma once


// GLEW library.
#include <GL/glew.h>
#include <GL/wglew.h>

#pragma comment(lib, "glew32.lib")


// GLFW library.
#include <GLFW/glfw3.h>

#pragma comment(lib, "glfw3.lib")


// GLM library.
#define GLM_FORCE_RADIANS    // enforce all angles to be in radians, not in degrees
#include <glm/glm.hpp>       // the most common GLM headers


// WGL (Windows OpenGL).
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


// Logging.
#include <fstream>
#include <iostream>

#include "../../OglLib/OglLib/OglLib.h"
