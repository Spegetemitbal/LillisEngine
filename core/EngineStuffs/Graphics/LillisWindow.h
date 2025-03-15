#pragma once
#include <GLFW/glfw3.h>

class LillisWindow
{
public:

	friend class GraphicsSystem;
	friend class InputSystem;

	bool InitWindow(unsigned int _width, unsigned int _height, const char* _name, bool isFullScreen)
	{
		if (isFullScreen)
		{
			monitor = glfwGetPrimaryMonitor();
			window = glfwCreateWindow(_width, _height, _name, monitor, NULL);
		} else
		{
			window = glfwCreateWindow(_width, _height, _name, NULL, NULL);
		}
		if (window == NULL)
		{
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		return true;
	}

	void DelWindow()
	{
		glfwDestroyWindow(window);
	}

	std::pair<int, int> getWindowSize()
	{
		if (monitor == NULL)
		{
			return {};
		}
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		return {mode->width, mode->height};
	}

	LillisWindow(const LillisWindow& win) = delete;

	~LillisWindow()
	{
		DelWindow();
	};

private:

	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;

	LillisWindow() {};
};