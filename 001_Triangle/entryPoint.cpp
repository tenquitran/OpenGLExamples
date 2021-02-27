#include "Common.h"
#include "Scene.h"

//////////////////////////////////////////////////////////////////////////

using namespace OglLib;
using namespace TriangleApp;

//////////////////////////////////////////////////////////////////////////

const int WndWidth  = 400;
const int WndHeight = 300;

void processInput(GLFWwindow* window);
void resizeCallback(GLFWwindow* window, int width, int height);

//////////////////////////////////////////////////////////////////////////


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// Redirect std::wcout and std::wcerr output to the log file.
	std::wofstream out("log.txt");
	std::wcout.rdbuf(out.rdbuf());
	std::wcerr.rdbuf(out.rdbuf());

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *pWindow = glfwCreateWindow(WndWidth, WndHeight, "Triangle", nullptr, nullptr);
	if (!pWindow)
	{
		std::wcerr << L"Failed to create GLFW window" << std::endl;
		glfwTerminate(); return 1;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, resizeCallback);

	// GLEW: load all OpenGL extensions.

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::wcerr << L"Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate(); return 1;
	}

	std::wcout << L"Using GLEW (version " << (const char *)glewGetString(GLEW_VERSION) << L")\n";

	Scene scene;

	if (!scene.initialize())
	{
		std::wcerr << L"Scene initialization failed\n";
		glfwTerminate(); return 1;
	}

	// Rendering loop.
	while (!glfwWindowShouldClose(pWindow))
	{
		processInput(pWindow);

		glClearColor(0.0f, 0.64f, 0.91f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		scene.render();

		glfwSwapBuffers(pWindow);
		glfwPollEvents();    // poll I/O events (keys presses, mouse movement, etc.)
	}

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* pWindow)
{
	if (GLFW_PRESS == glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}

void resizeCallback(GLFWwindow* pWindow, int width, int height)
{
	UNREFERENCED_PARAMETER(pWindow);

	glViewport(0, 0, width, height);
}
