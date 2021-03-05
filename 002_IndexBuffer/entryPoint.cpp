#include "framework.h"
#include "Scene.h"

//////////////////////////////////////////////////////////////////////////

using namespace OglLib;
using namespace TriangleApp;

//////////////////////////////////////////////////////////////////////////

const int WndWidth = 400;
const int WndHeight = 300;

void processInput(GLFWwindow* window, Scene& scene);
void resizeCallback(GLFWwindow* window, int width, int height);

//////////////////////////////////////////////////////////////////////////


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// Redirect std::cout and std::cerr output to the log file.
	std::ofstream out("log.txt");
	std::cout.rdbuf(out.rdbuf());
	std::cerr.rdbuf(out.rdbuf());

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(WndWidth, WndHeight, "Index Buffer", nullptr, nullptr);
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

	std::wcout << L"Using GLEW (version " << (const char*)glewGetString(GLEW_VERSION) << L")\n";

	Scene scene;

	if (!scene.initialize())
	{
		std::wcerr << L"Scene initialization failed\n";
		glfwTerminate(); return 1;
	}

	// Rendering loop.
	while (!glfwWindowShouldClose(pWindow))
	{
		processInput(pWindow, scene);

		glClearColor(0.0f, 0.64f, 0.91f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		scene.render();

		glfwSwapBuffers(pWindow);
		glfwPollEvents();    // poll I/O events (keys presses, mouse movement, etc.)
	}

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* pWindow, Scene& scene)
{
	if (GLFW_PRESS == glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
	
	if (GLFW_PRESS == glfwGetKey(pWindow, GLFW_KEY_RIGHT))
	{
		scene.rotateCamera(glm::vec3(0.0f, 2.0f, 0.0f));
	}
	
	if (GLFW_PRESS == glfwGetKey(pWindow, GLFW_KEY_LEFT))
	{
		scene.rotateCamera(glm::vec3(0.0f, -2.0f, 0.0f));
	}
	
	if (GLFW_PRESS == glfwGetKey(pWindow, GLFW_KEY_UP))
	{
		scene.rotateCamera(glm::vec3(2.0f, 0.0f, 0.0f));
	}
	
	if (GLFW_PRESS == glfwGetKey(pWindow, GLFW_KEY_DOWN))
	{
		scene.rotateCamera(glm::vec3(-2.0f, 0.0f, 0.0f));
	}
}

void resizeCallback(GLFWwindow* pWindow, int width, int height)
{
	UNREFERENCED_PARAMETER(pWindow);

	glViewport(0, 0, width, height);
}
