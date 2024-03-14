#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem(unsigned int width, unsigned int height, std::string name)
{
	_windowName = name;
	_width = width;
	_height = height;
}

GraphicsSystem::~GraphicsSystem()
{

}

void GraphicsSystem::error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool GraphicsSystem::Init()
{
	if (!glfwInit())
	{
		return false;
	}

	//The following are for MacOS specific stuff, integrate later.
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(_width, _height, _windowName.data(), NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetErrorCallback(error_callback);

	return true;
}

void GraphicsSystem::ShutDown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GraphicsSystem::Update()
{
	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//const float ratio = width / (float)height;

	//glViewport(0, 0, width, height);
	//glClear(GL_COLOR_BUFFER_BIT);

	//glfwSwapBuffers(window);
	glfwPollEvents();
}