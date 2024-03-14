#pragma once

#include "RectangleRenderer.h"
#include <glfw3.h>

class GraphicsSystem
{
public:

	GraphicsSystem(unsigned int width, unsigned int height, std::string name);
	~GraphicsSystem();

	bool Init();
	void ShutDown();

	void Update();

	void closeWindow() { if (window) { glfwSetWindowShouldClose(window, GLFW_TRUE); } }
	bool isWindowOpen() { return glfwWindowShouldClose(window); }

	static void error_callback(int error, const char* description);

	GLFWwindow* getWindow() { return window; }

private:

	GLFWwindow* window = nullptr;
	std::string _windowName = "";
	unsigned int _height = 0, _width = 0;
};