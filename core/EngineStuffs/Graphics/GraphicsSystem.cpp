#include "GraphicsSystem.h"
#include "glad/gl.h"
#include "../Physics/AABB.h"
#include "RenderOrder.h"
#include <glm/vec4.hpp>

#include "Parallax.h"
#include "ProcGen.h"
#include "EngineStuffs/Particles/ParticleEmitter.h"
#include "Pipeline/BackgroundPipelineSegment.h"
#include "Pipeline/DefaultRenderPipeline.h"
#include "Utils/InputSystem.h"
#include "Pipeline/ParticlePipelineSegment.h"
#include "Pipeline/SpritePipelineSegment.h"
#include "Pipeline/PostProcessSegment.h"
#include "Pipeline/ProcGenPipelineSegment.h"
#include "Pipeline/UIPipelineSegment.h"

GraphicsSystem* GraphicsSystem::instance = nullptr;

GraphicsSystem* GraphicsSystem::getInstance()
{
	if (instance == nullptr)
	{
		return createInstance();
	}
	return instance;
}

GraphicsSystem* GraphicsSystem::createInstance(const RenderSettings &render_settings, const std::string& name)
{
	if (instance == nullptr)
	{
		instance = DBG_NEW GraphicsSystem(render_settings, name);
	}
	return instance;
}

void GraphicsSystem::delInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}


GraphicsSystem::GraphicsSystem(const RenderSettings &render_settings, const std::string &name)
{
	_windowName = name;
	this->render_settings = render_settings;
}

GraphicsSystem::~GraphicsSystem()
{
	ShutDown();
}

void GraphicsSystem::error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void GraphicsSystem::SetParticlePipeline(ParticlePipelineSegment *pipeline, bool init)
{
	particlePipeline = pipeline;
	if (init)
	{
		pipeline->InitSegment();
	}
}

void GraphicsSystem::SetSpritePipeline(SpritePipelineSegment *pipeline, bool init)
{
	spritePipeline = pipeline;
	if (init)
	{
		pipeline->InitSegment();
	}
}

void GraphicsSystem::SetPostProcessPipeline(PostProcessSegment *pipeline, bool init)
{
	postProcessPipeline = pipeline;
	if (init)
	{
		pipeline->InitSegment();
	}
}

void GraphicsSystem::SetProcGenPipeline(ProcGenPipelineSegment *pipeline, bool init)
{
	procGenPipeline = pipeline;
	if (init)
	{
		pipeline->InitSegment();
	}
}

void GraphicsSystem::SetUIPipeline(UIPipelineSegment *pipeline, bool init)
{
	uiPipeline = pipeline;
	if (init)
	{
	//	pipeline->InitSegment();
	}
}

void GraphicsSystem::SetBackgroundPipeline(BackgroundPipelineSegment *pipeline, bool init)
{
	backgroundPipeline = pipeline;
	if (init)
	{
		pipeline->InitSegment();
	}
}



bool GraphicsSystem::Init()
{
	if (isInitted)
	{
		return true;
	}

	if (!glfwInit())
	{
		return false;
	}

	//The following are for macOS specific stuff, integrate later.
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//TODO: Add dynamic resizing systems.
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	if (!_win.InitWindow(render_settings.windowWidth, render_settings.windowHeight, _windowName.data(), render_settings.fullScreen))
	{
		printf("GLFW failed to create window");
		return false;
	}

	InputSystem::Init(&_win);

	if (render_settings.fullScreen)
	{
		std::pair<int, int> window_size = _win.getWindowSize();

		render_settings.windowWidth = window_size.first;
		render_settings.windowHeight = window_size.second;
	}

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return false;
	}

	mainCamera = LILLIS::Camera(glm::vec2(0, 0), render_settings.resolutionWidth, render_settings.resolutionHeight);
	
	glfwSetErrorCallback(error_callback);

	glCreateVertexArrays(1, &postProcessVAO);

	// load shaders
	ResourceManager::loadDefaultPipeline();
	// configure shaders
	ResourceManager::GetShader("Default").Use().SetInteger("image", 0);
	ResourceManager::GetShader("Default").SetMatrix4("projection", mainCamera.projectionMatrix(true));
	ResourceManager::GetShader("DefaultProcGen").SetMatrix4("_projection", mainCamera.projectionMatrix(true));
	// set render-specific controls

	SetSpritePipeline(DBG_NEW SpritePipelineSegment(render_settings, ResourceManager::GetShader("Default")), true);
	SetParticlePipeline(DBG_NEW ParticlePipelineSegment(render_settings, ResourceManager::GetShader("DefaultParticle")), true);
	SetProcGenPipeline(DBG_NEW ProcGenPipelineSegment(render_settings, ResourceManager::GetShader("DefaultProcGen")), true);
	SetPostProcessPipeline(DBG_NEW PostProcessSegment(render_settings, ResourceManager::GetShader("DefaultPostProcess")), true);
	SetBackgroundPipeline(DBG_NEW BackgroundPipelineSegment(render_settings, ResourceManager::GetShader("Default")), true);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//For the love of god, move the sprite holder here.
	//ResourceManager::LoadTextureRecursive("assets",true,false);

	isInitted = true;
	return true;
}

void GraphicsSystem::ShutDown()
{
	delete spritePipeline;
	spritePipeline = nullptr;
	delete postProcessPipeline;
	postProcessPipeline = nullptr;
	delete procGenPipeline;
	procGenPipeline = nullptr;
	delete uiPipeline;
	uiPipeline = nullptr;
	delete particlePipeline;
	particlePipeline = nullptr;
	delete backgroundPipeline;
	backgroundPipeline = nullptr;

	if (postProcessFBO != 0)
	{
		glDeleteFramebuffers(1, &postProcessFBO);
	}
	_win.DelWindow();
	glfwTerminate();
	isInitted = false;
}

//Add more complex culling for static geometry
std::vector<Sprite *> GraphicsSystem::CullToScreen(ActiveTracker<Sprite *> &sprites, unsigned int lastSprite)
{
	std::vector<Sprite *> culledSprites = std::vector<Sprite *>();

	upSprite = -std::numeric_limits<float>::infinity();
	downSprite = std::numeric_limits<float>::infinity();

	glm::vec4 camAB = mainCamera.getAABB(render_settings.pixelsPerUnit, render_settings.pixelPerfect);
	AABB cameraAABB = {camAB.x, camAB.y, camAB.z, camAB.w};

	for (int i = 0; i < lastSprite; i++)
	{
		Sprite* spr = sprites[i];
		if (spr->GetActive() && spr->GetisEnabled())
		{
			if (spr->getImageName().empty())
			{
				continue;
			}
			if (AABB::Intersect(spr->getAABB().Scale((float)render_settings.pixelsPerUnit), cameraAABB))
			{
				float yVal = spr->getRenderLocation().y;
				if (yVal > upSprite)
				{
					upSprite = yVal;
				} else if (yVal < downSprite)
				{
					downSprite = yVal;
				}
				culledSprites.push_back(sprites[i]);
			}
		}
	}

	return culledSprites;
}

bool compareSprites(Sprite* a, Sprite* b) {
	// Sort primarily by the first element in ascending order
	if (a->getLayer() < b->getLayer())
	{
		return true;
	}
	//Do getTransparent.
	if (a->getLayer() == b->getLayer())
	{
		return a->getIsOpaque() > b->getIsOpaque();
	}
	return false;
}

bool compareWrappers(const ColorBufferWrapper& obj1, const ColorBufferWrapper& obj2) {
	return obj1.depth < obj2.depth;
}

void GraphicsSystem::RenderCall(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite,
	ActiveTracker<ParticleEmitter*>& emitters, unsigned int lastEmitter, std::vector<TileMap>& tile_maps, BackgroundManager* backgrounds)
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}

	//If pixel perfect, snap camera.
	if (pixelPerfect)
	{
		//mainCamera.position = Parallax::snapCamera(mainCamera.position);
		glDisable(GL_MULTISAMPLE);
	}

	//Do broad phase, cull all sprites not on screen.
	std::vector<Sprite *> spritesOnScreen = CullToScreen(sprites,lastSprite);
	std::sort(spritesOnScreen.begin(), spritesOnScreen.end(), compareSprites);

	glm::vec4 camRect = mainCamera.getAABB(render_settings.pixelsPerUnit, render_settings.pixelPerfect);
	AABB camAABB = {camRect.x, camRect.y, camRect.z, camRect.w};

	//Cull all tilemaps on screen.
	for (auto & tMap: tile_maps)
	{
		if (tMap.active)
		{
			glm::vec2 cullVals = tMap.CullMap(camAABB, render_settings.pixelsPerUnit);
			if (cullVals.x < downSprite)
			{
				downSprite = cullVals.x;
			}
			if (cullVals.y > upSprite)
			{
				upSprite = cullVals.y;
			}
		}
	}

	//Do the same for tiles
	//MAKE SURE THIS IS ALWAYS CALLED BEFORE OTHER CALCULATE ORDER... or you might have 1 goofy frame?
	for (auto & tMap: tile_maps)
	{
		if (tMap.active)
		{
			RenderOrder::CalculateTileOrder(&tMap, upSprite, downSprite);
		}
	}

	//Set the relevant depth values
	RenderOrder::CalculateOrder(spritesOnScreen, upSprite, downSprite);
	RenderOrder::CalculateOrder(emitters.extractData());

	std::vector<ColorBufferWrapper> cbWrappers = std::vector<ColorBufferWrapper>();

	backgroundPipeline->PreRender();
	std::vector<BackgroundImage> backgroundImages = backgrounds->GetBackgrounds(mainCamera, doParallax, render_settings.pixelPerfect);
	cbWrappers = backgroundPipeline->RenderBackgrounds(backgroundImages, mainCamera, backgrounds->numBackgrounds());
	backgroundPipeline->PostRender();

	spritePipeline->PreRender();
	std::vector<ColorBufferWrapper> sprtWrap = spritePipeline->DoStep(spritesOnScreen, lastSprite, tile_maps, mainCamera);
	spritePipeline->PostRender();

	cbWrappers.insert(cbWrappers.end(), sprtWrap.begin(), sprtWrap.end());

	particlePipeline->PreRender();
	particlePipeline->DoStep(emitters, lastEmitter, upSprite, downSprite, mainCamera);
	particlePipeline->PostRender();

	procGenPipeline->PreRender();
	ProcGen::getInstance()->Render(mainCamera, procGenPipeline);
	procGenPipeline->PostRender();

	postProcessPipeline->PreRender();
	std::sort(cbWrappers.begin(), cbWrappers.end(), compareWrappers);
	postProcessPipeline->DoPostProcess(cbWrappers, (int)sprtWrap.size());
	postProcessPipeline->PostRender();
}

void GraphicsSystem::PreDraw()
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}
	glfwPollEvents();
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, (GLsizei)render_settings.resolutionWidth, (GLsizei)render_settings.resolutionHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsSystem::PostDraw()
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}
	glfwSwapBuffers(_win.window);
}

void GraphicsSystem::SetDoParallax(bool par, bool pixelPerfect)
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}
	doParallax = par;
	this->pixelPerfect = pixelPerfect;
	backgroundPipeline->deferredRender = pixelPerfect;
	spritePipeline->deferredRender = pixelPerfect;
	backgroundPipeline->doParallax = par;
	spritePipeline->doParallax = par;
}

void GraphicsSystem::SetPostProcessUsage(bool doSprite, bool doBackground, bool doUI)
{
	if (spritePipeline == nullptr || backgroundPipeline == nullptr)
	{
		return;
	}
	spritePipeline->doPostProcess = doSprite;
	backgroundPipeline->doPostProcess = doBackground;
}

bool GraphicsSystem::GeneratePostProcess(const char *fShaderFile, const std::string &name, bool finalShader)
{
	if (ResourceManager::Shaders.contains(name))
	{
		std::cout << "Shader name: " << name << " already exists." << std::endl;
		return false;
	}


	DefaultRenderPipeline pipeline = DefaultRenderPipeline();
	const char* vppShaderCode = pipeline.postProcessVertex.c_str();

	std::string fragmentCode;
	try
	{
		// open files
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream fShaderStream;
		// read file's buffer contents into streams
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		fragmentShaderFile.close();
		// convert stream into string
		fragmentCode = fShaderStream.str();
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* fShaderCode = fragmentCode.c_str();

	Shader postProc;
	if (postProc.Compile(vppShaderCode, fShaderCode, nullptr))
	{
		ResourceManager::Shaders[name] = postProc;
		return true;
	} else
	{
		std::cout << "Shader: " << name << " could not be compiled." << std::endl;
		return false;
	}
}



void GraphicsSystem::SetCursor(const std::string &imageName, unsigned int xHot, unsigned int yHot)
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}
	if (_cursors.find(imageName) != _cursors.end())
	{
		glfwSetCursor(_win.window, _cursors[imageName]);
		return;
	}

	Texture2D tex = ResourceManager::GetTexture(imageName);
	GLubyte* pixels = DBG_NEW GLubyte[tex.Width * tex.Height * 4];
	glActiveTexture(GL_TEXTURE0);
	tex.Bind();
	//You may be getting the wrong texture here
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	GLFWimage image;
	image.width = (int)tex.Width;
	image.height = (int)tex.Height;
	image.pixels = pixels;
	_cursors.try_emplace(imageName,glfwCreateCursor(&image, (int)xHot, (int)yHot));
	glfwSetCursor(_win.window, _cursors[imageName]);
	delete pixels;
}
