#include "GraphicsSystem.h"
#include "glad/gl.h"
#include "../Physics/AABB.h"
#include <glm/vec4.hpp>

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

	if (!_win.InitWindow(_width, _height, _windowName.data()))
	{
		printf("GLFW failed to create window");
		return false;
	}

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return false;
	}

	mainCamera = LILLIS::Camera(glm::vec2(0, 0), _width, _height);
	
	glfwSetErrorCallback(error_callback);

	// load shaders
	ResourceManager::loadDefaultPipeline();
	// configure shaders
	ResourceManager::GetShader("Default").Use().SetInteger("image", 0);
	ResourceManager::GetShader("Default").SetMatrix4("projection", mainCamera.projectionMatrix());
	// set render-specific controls
	defaultRenderer = DBG_NEW SpriteRenderer(ResourceManager::GetShader("Default"));
	// load textures

	//For the love of god, move the sprite holder here.
	//ResourceManager::LoadTextureRecursive("assets",true,false);
	return true;
}

void GraphicsSystem::ShutDown()
{
	delete defaultRenderer;
	_win.DelWindow();
	glfwTerminate();
}

std::vector<Sprite *> GraphicsSystem::CullToScreen(ActiveTracker<Sprite *> &sprites, unsigned int lastSprite)
{
	std::vector<Sprite *> culledSprites = std::vector<Sprite *>();

	glm::vec4 camAB = mainCamera.getAABB();
	AABB cameraAABB = {camAB.x, camAB.y, camAB.z, camAB.w};

	for (int i = 0; i < lastSprite; i++)
	{
		Sprite* spr = sprites[i];
		if (spr->GetActive())
		{
			if (spr->image.empty())
			{
				continue;
			}
			if (AABB::Intersect(spr->getAABB(), cameraAABB))
			{
				culledSprites.push_back(sprites[i]);
			}
		}
	}

	return culledSprites;
}


void GraphicsSystem::RenderCall(ActiveTracker<Sprite*>& sprites, unsigned int lastSprite)
{
	std::vector<Sprite *> spritesOnScreen = CullToScreen(sprites,lastSprite);

	for (int i = 0; i < spritesOnScreen.size(); i++)
	{
		Sprite* spr = spritesOnScreen[i];
		//Might be dereferencing something you shouldn't.
		if (spr->image.empty())
		{
			return;
		}
		Texture2D tex = ResourceManager::GetTexture(spr->image);
		defaultRenderer->DrawSprite(tex, spr->getRenderLocation(), spr->frame,
			spr->RenderSize() * spr->getRenderScale(), spr->getRenderRotation());
	}
}

void GraphicsSystem::PreDraw()
{
	glViewport(0, 0, _width, _height);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Don't call until rearchitectured.
void GraphicsSystem::Update()
{

}

void GraphicsSystem::PostDraw()
{
	glfwSwapBuffers(_win.window);
	glfwPollEvents();
}

void GraphicsSystem::SetCursor(const std::string &imageName, unsigned int xHot, unsigned int yHot)
{
	if (_cursors.find(imageName) != _cursors.end())
	{
		glfwSetCursor(_win.window, _cursors[imageName]);
		return;
	}

	Texture2D tex = ResourceManager::GetTexture(imageName);
	GLubyte* pixels = DBG_NEW GLubyte[tex.Width * tex.Height * 4];
	glActiveTexture(GL_TEXTURE0);
	tex.Bind();
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	GLFWimage image;
	image.width = tex.Width;
	image.height = tex.Height;
	image.pixels = pixels;
	_cursors.try_emplace(imageName,glfwCreateCursor(&image, xHot, yHot));
	glfwSetCursor(_win.window, _cursors[imageName]);
	delete pixels;
}
