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
#include "../Tilemaps/TileMap.h"

class GraphicsSystem
{
public:

	static GraphicsSystem* createInstance(const RenderSettings &render_settings = RenderSettings(), const std::string& name = "LILLIS");
	static GraphicsSystem* getInstance();
	static void delInstance();

	GraphicsSystem() = delete;

	bool Init();
	void ShutDown();
	bool GetIsInitted() const { return isInitted;}

	//void Update();

	void PreDraw();
	void RenderCall(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite,
		ActiveTracker<ParticleEmitter*>& emitters, unsigned int lastEmitter, std::vector<TileMap>& tile_maps);

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

	unsigned int GetPixelsPerUnit() const
	{
		return render_settings.pixelsPerUnit;
	}

	void SetCameraPosition(const glm::vec2& pos)
	{
		mainCamera.position = glm::vec2(pos.x, -pos.y) * (float)render_settings.pixelsPerUnit;
	}

	LillisWindow* GetWin() { return &_win; }

	void closeWindow()
	{
		if (!isInitted) {return;}
		if (_win.window) { glfwSetWindowShouldClose(_win.window, GLFW_TRUE); }
	}
	bool isWindowOpen()
	{
		if (!isInitted)
		{
			return false;
		}
		return glfwWindowShouldClose(_win.window);
	}

	static void error_callback(int error, const char* description);
	//void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:

	GraphicsSystem(const RenderSettings &render_settings = RenderSettings(), const std::string &name = "LILLIS");
	~GraphicsSystem();

	bool isInitted = false;

	static GraphicsSystem* instance;

	unsigned int depthBuffer{}, fbo{}, colorBuffer{};
	unsigned int postProcessVAO{}, postProcessFBO{}, postProcessColorBuffer{};

	std::vector<LILLIS::Shader> postProcessChain;

	RenderSettings render_settings;

	std::vector<Sprite*> CullToScreen(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite);

	LILLIS::Camera mainCamera;

	std::unordered_map<std::string, GLFWcursor*> _cursors;

	float upSprite = 0, downSprite = 0;

	LillisWindow _win;
	std::string _windowName = "";
};