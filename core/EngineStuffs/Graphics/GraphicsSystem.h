#pragma once

#define GLFW_INCLUDE_NONE
#include "SpriteRenderer.h"
#include "../../Utils/ResourceManager.h"
#include "LillisWindow.h"
#include "Camera.h"
#include "Sprite.h"
#include "Animator.h"
#include "RenderSettings.h"
#include "../GameObject.h"

class GraphicsSystem
{
public:

	GraphicsSystem(RenderSettings render_settings, std::string name);
	~GraphicsSystem();

	bool Init();
	void ShutDown();

	//void Update();

	void PreDraw();
	void RenderCall(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite);

	void RunPostProcessing();
	void AddPostProcess(LILLIS::Shader shader);
	void RemovePostProcess(LILLIS::Shader shader)
	{
		for (int i = 0; i < postProcessChain.size(); i++)
		{
			if (postProcessChain[i].ID == shader.ID)
			{
				postProcessChain.erase(postProcessChain.begin() + i);
				break;
			}
		}
	}

	void PostDraw();

	//Major allocation, do not use often.
	void SetCursor(const std::string& imageName, unsigned int xHot, unsigned int yHot);

	glm::vec2 GetCameraPosition() const
	{
		return mainCamera.position;
	}

	glm::vec2 GetCameraDimensions() const
	{
		return {mainCamera.WIDTH, mainCamera.HEIGHT};
	}

	void SetCameraPosition(const glm::vec2& pos)
	{
		mainCamera.position = pos;
	}

	LillisWindow* GetWin() { return &_win; }

	void closeWindow() { if (_win.window) { glfwSetWindowShouldClose(_win.window, GLFW_TRUE); } }
	bool isWindowOpen() { return glfwWindowShouldClose(_win.window); }

	static void error_callback(int error, const char* description);
	//void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:

	unsigned int depthBuffer{}, fbo{}, colorBuffer{};
	unsigned int postProcessVAO{}, postProcessFBO{}, postProcessColorBuffer{};

	std::vector<LILLIS::Shader> postProcessChain;

	RenderSettings render_settings;

	std::vector<Sprite*> CullToScreen(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite);

	LILLIS::Camera mainCamera;

	std::unordered_map<std::string, GLFWcursor*> _cursors;

	float upSprite, downSprite;

	LillisWindow _win;
	std::string _windowName = "";
};