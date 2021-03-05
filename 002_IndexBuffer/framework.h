// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>    // value_ptr


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
