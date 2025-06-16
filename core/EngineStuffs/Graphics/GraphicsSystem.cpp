#include "GraphicsSystem.h"
#include "glad/gl.h"
#include "../Physics/AABB.h"
#include "RenderOrder.h"
#include <glm/vec4.hpp>

#include "ProcGen.h"
#include "EngineStuffs/Particles/ParticleEmitter.h"
#include "Utils/InputSystem.h"

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
	SpriteRenderer::setPixelsPerUnit(render_settings.pixelsPerUnit);
}

GraphicsSystem::~GraphicsSystem()
{
	ShutDown();
}

void GraphicsSystem::error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
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


	//Generate frame buffer.
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//8 bit RGBA color buffer
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, render_settings.resolutionWidth, render_settings.resolutionHeight);

	//Make depth buffer.
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	//Maybe use a higher precision buffer if issues are arising.
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, render_settings.resolutionWidth, render_settings.resolutionHeight);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer,0);

	glCreateVertexArrays(1, &postProcessVAO);

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer incomplete: %d", fboStatus);
		return false;
	}

	// load shaders
	ResourceManager::loadDefaultPipeline();
	// configure shaders
	ResourceManager::GetShader("Default").Use().SetInteger("image", 0);
	ResourceManager::GetShader("Default").SetMatrix4("projection", mainCamera.projectionMatrix());
	ResourceManager::GetShader("DefaultProcGen").SetMatrix4("_projection", mainCamera.projectionMatrix());
	// set render-specific controls
	SpriteRenderer::setDefaultShader(ResourceManager::GetShader("Default"));
	SpriteRenderer::setDefaultUIShader(ResourceManager::GetShader("DefaultUI"));
	SpriteRenderer::setDefaultProcGenShader(ResourceManager::GetShader("DefaultProcGen"));
	SpriteRenderer::setDefaultParticleShader(ResourceManager::GetShader("DefaultParticle"));
	SpriteRenderer::initRenderData();
	// load textures

	//Beep beep I'm a sheep
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//For the love of god, move the sprite holder here.
	//ResourceManager::LoadTextureRecursive("assets",true,false);

	isInitted = true;
	return true;
}

void GraphicsSystem::ShutDown()
{
	glDeleteFramebuffers(1, &fbo);
	if (postProcessFBO != 0)
	{
		glDeleteFramebuffers(1, &postProcessFBO);
	}
	SpriteRenderer::shutdownRenderData();
	_win.DelWindow();
	glfwTerminate();
	isInitted = false;
}

//Add more complex culling for static geometry
std::vector<Sprite *> GraphicsSystem::CullToScreen(ActiveTracker<Sprite *> &sprites, unsigned int lastSprite)
{
	std::vector<Sprite *> culledSprites = std::vector<Sprite *>();
	std::vector<Sprite *> transparentSprites = std::vector<Sprite *>();

	upSprite = -std::numeric_limits<float>::infinity();
	downSprite = std::numeric_limits<float>::infinity();

	glm::vec4 camAB = mainCamera.getAABB(render_settings.pixelsPerUnit);
	AABB cameraAABB = {camAB.x, camAB.y, camAB.z, camAB.w};

	for (int i = 0; i < lastSprite; i++)
	{
		Sprite* spr = sprites[i];
		if (spr->GetActive() && spr->GetisEnabled())
		{
			if (spr->image.empty())
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
				if (spr->opaque)
				{
					culledSprites.push_back(sprites[i]);
				} else
				{
					transparentSprites.push_back(sprites[i]);
				}
			}
		}
	}

	std::sort(transparentSprites.begin(), transparentSprites.end(), [](const Sprite *s1, const Sprite *s2)
		{return s1->getLayer() < s2->getLayer();});
	culledSprites.insert(culledSprites.end(), transparentSprites.begin(), transparentSprites.end());

	return culledSprites;
}

void GraphicsSystem::RenderCall(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite,
	ActiveTracker<ParticleEmitter*>& emitters, unsigned int lastEmitter, std::vector<TileMap>& tile_maps)
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}
	//Do broad phase, cull all sprites not on screen.
	std::vector<Sprite *> spritesOnScreen = CullToScreen(sprites,lastSprite);

	glm::vec4 camRect = mainCamera.getAABB(render_settings.pixelsPerUnit);
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

	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < spritesOnScreen.size(); i++)
	{
		Sprite* spr = spritesOnScreen[i];
		if (spr->image.empty())
		{
			throw;
		}
		Texture2D tex = ResourceManager::GetTexture(spr->image);
		SpriteRenderer::DrawSprite(tex, spr->getRenderLocation(), spr->getRenderValue(), (int)spr->frame, mainCamera.projectionMatrix(),
			spr->RenderSize() * spr->getRenderScale(), spr->getRenderRotation());
	}

	for (auto & tMap: tile_maps)
	{
		if (tMap.active)
		{
			glm::vec2 renderSize = tMap.getTileSize();
			for (int i = 0; i < tMap.tilesToRender.size(); i++)
			{
				TileLoc t = tMap.tilesToRender[i];
				std::string img = tMap.getImageFromIndex(t.tile);
				unsigned int frm = tMap.getFrameFromIndex(t.tile);
				if (img.empty())
				{
					throw;
				}
				Texture2D tex = ResourceManager::GetTexture(img);
				SpriteRenderer::DrawSprite(tex, t.worldPos, t.zVal, (int)frm, mainCamera.projectionMatrix(),
					renderSize, 0);
			}
		}
	}

	bool doRenderAxis = RenderOrder::GetRenderAxis() != glm::vec2(0);
	float invDist = 1 / (upSprite - downSprite);
	for (int i = 0; i < lastEmitter; i++)
	{
		if (emitters[i]->GetActive())
		{
			SpriteRenderer::DrawParticles(*emitters[i], mainCamera.projectionMatrix(), upSprite, invDist, doRenderAxis);
		}
	}

	glDisable(GL_DEPTH_TEST);
	ProcGen::getInstance()->Render(mainCamera.projectionMatrix());

	RunPostProcessing();
}

void GraphicsSystem::PreDraw()
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}
	glfwPollEvents();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
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

void GraphicsSystem::RunPostProcessing()
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}
	glBindVertexArray(postProcessVAO);

	bool usingAlternativeFBO = false;
	for (int i = 0; i < postProcessChain.size(); i++)
	{
		LILLIS::Shader currentShader = postProcessChain[i];
		currentShader.Use();
		currentShader.SetInteger("_ColorBuffer", 0);

		if (usingAlternativeFBO)
		{
			glBindTextureUnit(0, postProcessColorBuffer);
		} else
		{
			glBindTextureUnit(0, colorBuffer);
		}

		//Do something here idk man.
		if (i < postProcessChain.size() - 1)
		{
			if (usingAlternativeFBO)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
			} else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			}
			glViewport(0, 0, (GLsizei)render_settings.resolutionWidth, (GLsizei)render_settings.resolutionHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//DRAW FULLSCREEN TRIANGLE
			glDrawArrays(GL_TRIANGLES, 0, 3);

			usingAlternativeFBO = !usingAlternativeFBO;
		} else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, (GLsizei)render_settings.windowWidth, (GLsizei)render_settings.windowHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//DRAW FULLSCREEN TRIANGLE
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	if (postProcessChain.empty())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, (GLsizei)render_settings.windowWidth, (GLsizei)render_settings.windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LILLIS:Shader defaultPostProcess = ResourceManager::GetShader("DefaultPostProcess");
		defaultPostProcess.Use();
		defaultPostProcess.SetInteger("_ColorBuffer", 0);

		glBindTextureUnit(0, colorBuffer);
		//3 vertices because triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

void GraphicsSystem::AddPostProcess(LILLIS::Shader shader)
{
	if (!isInitted)
	{
		std::cerr << "Graphics system isn't initialized" << std::endl;
		return;
	}
	postProcessChain.push_back(shader);
	if (postProcessChain.size() > 2 && postProcessFBO < 1)
	{
		//Generate frame buffer.
		glGenFramebuffers(1, &postProcessFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);

		//8 bit RGBA color buffer
		glGenTextures(1, &postProcessColorBuffer);
		glBindTexture(GL_TEXTURE_2D, postProcessColorBuffer);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (GLsizei)render_settings.resolutionWidth, (GLsizei)render_settings.resolutionHeight);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, postProcessColorBuffer, 0);

		GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
			printf("Framebuffer incomplete: %d", fboStatus);
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
