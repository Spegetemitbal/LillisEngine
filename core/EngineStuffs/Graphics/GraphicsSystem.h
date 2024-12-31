#pragma once

#define GLFW_INCLUDE_NONE
#include "SpriteRenderer.h"
#include "../../Utils/ResourceManager.h"
#include "LillisWindow.h"
#include "Camera.h"
#include "Sprite.h"
#include "../GameObject.h"

class GraphicsSystem
{
public:

	GraphicsSystem(unsigned int width, unsigned int height, std::string name);
	~GraphicsSystem();

	bool Init();
	void ShutDown();

	void Update();

	void PreDraw();
	void RenderSprite(Sprite& spr);
	void PostDraw();

	LillisWindow* GetWin() { return &_win; }

	void closeWindow() { if (_win.window) { glfwSetWindowShouldClose(_win.window, GLFW_TRUE); } }
	bool isWindowOpen() { return glfwWindowShouldClose(_win.window); }

	static void error_callback(int error, const char* description);

private:

	SpriteRenderer* defaultRenderer;
	LILLIS::Camera testCam;

	LillisWindow _win;
	std::string _windowName = "";
	unsigned int _height = 0, _width = 0;
};