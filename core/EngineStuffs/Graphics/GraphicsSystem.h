#pragma once

#define GLFW_INCLUDE_NONE
#include "../../Utils/ResourceManager.h"
#include "LillisWindow.h"
#include "Camera.h"
#include "Sprite.h"
#include "Animator.h"
#include "RenderSettings.h"
#include "../GameObject.h"
#include "../Tilemaps/TileMap.h"

class SpritePipelineSegment;
class ParticlePipelineSegment;
class ProcGenPipelineSegment;
class UIPipelineSegment;
class PostProcessSegment;
class BackgroundPipelineSegment;
class ParticleEmitter;
class BackgroundManager;

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
	void SetSpritePipeline(SpritePipelineSegment* pipeline, bool init = false);
	void SetParticlePipeline(ParticlePipelineSegment* pipeline, bool init = false);
	void SetProcGenPipeline(ProcGenPipelineSegment* pipeline, bool init = false);
	void SetUIPipeline(UIPipelineSegment* pipeline, bool init = false);
	void SetPostProcessPipeline(PostProcessSegment* pipeline, bool init = false);
	void SetBackgroundPipeline(BackgroundPipelineSegment* pipeline, bool init = false);
	SpritePipelineSegment* GetSpritePipeline() { return spritePipeline; }
	ParticlePipelineSegment* GetParticlePipeline() { return particlePipeline; }
	ProcGenPipelineSegment* GetProcGenPipeline() { return procGenPipeline; }
	UIPipelineSegment* GetUIPipeline() { return uiPipeline; }
	PostProcessSegment* GetPostProcessPipeline() { return postProcessPipeline; }
	BackgroundPipelineSegment* GetBackgroundPipeline() { return backgroundPipeline; }

	void PreDraw();
	void RenderCall(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite,
		ActiveTracker<ParticleEmitter*>& emitters, unsigned int lastEmitter, std::vector<TileMap>& tile_maps, BackgroundManager* backgrounds);

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

	SpritePipelineSegment* spritePipeline = nullptr;
	ParticlePipelineSegment* particlePipeline = nullptr;
	ProcGenPipelineSegment* procGenPipeline = nullptr;
	UIPipelineSegment* uiPipeline = nullptr;
	PostProcessSegment* postProcessPipeline = nullptr;
	BackgroundPipelineSegment* backgroundPipeline = nullptr;


	GraphicsSystem(const RenderSettings &render_settings = RenderSettings(), const std::string &name = "LILLIS");
	~GraphicsSystem();

	bool isInitted = false;

	static GraphicsSystem* instance;

	//unsigned int depthBuffer{}, fbo{}, colorBuffer{};
	unsigned int postProcessVAO{}, postProcessFBO{}, postProcessColorBuffer{};

	RenderSettings render_settings;

	std::vector<Sprite*> CullToScreen(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite);

	LILLIS::Camera mainCamera;

	std::unordered_map<std::string, GLFWcursor*> _cursors;

	float upSprite = 0, downSprite = 0;

	LillisWindow _win;
	std::string _windowName = "";
};
