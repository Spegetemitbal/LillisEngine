#pragma once
#include <glfw3.h>

class LillisWindow
{
public:

	friend class GraphicsSystem;
	friend class InputSystem;

	bool InitWindow(unsigned int _width, unsigned int _height, const char* _name)
	{
		window = glfwCreateWindow(_width, _height, _name, NULL, NULL);
		if (window == NULL)
		{
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
	}

	void DelWindow()
	{
		glfwDestroyWindow(window);
	}

	LillisWindow(unsigned int _width, unsigned int _height, const char* _name)
	{
		//window = glfwCreateWindow(_width, _height, _name, NULL, NULL);
	}

	LillisWindow(const LillisWindow& win) = delete;

	~LillisWindow()
	{
		DelWindow();
	};

private:

	GLFWwindow* window = nullptr;

	LillisWindow() {};
};