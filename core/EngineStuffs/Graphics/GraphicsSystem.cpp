#include "GraphicsSystem.h"
#include "glad/gl.h"

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

	testCam = LILLIS::Camera(glm::vec2(0, 0), _width, _height);
	
	glfwSetErrorCallback(error_callback);

	// load shaders
	ResourceManager::loadDefaultPipeline();
	// configure shaders
	ResourceManager::GetShader("Default").Use().SetInteger("image", 0);
	ResourceManager::GetShader("Default").SetMatrix4("projection", testCam.projectionMatrix());
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

void GraphicsSystem::RenderSprite(Sprite& spr)
{
	Texture2D tex = ResourceManager::GetTexture(spr.image);
	defaultRenderer->DrawSprite(tex, spr.getRenderLocation(),
		glm::vec2(tex.Width, tex.Height) * spr.getRenderScale(), spr.getRenderRotation());
}

void GraphicsSystem::PreDraw()
{
	glViewport(0, 0, _width, _height);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Don't call until rearchitectured.
void GraphicsSystem::Update()
{
	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	const float ratio = _width / (float)_height;

	glViewport(0, 0, _width, _height);
	glClear(GL_COLOR_BUFFER_BIT);

	//At one point, render them all here.

	defaultRenderer->DrawSprite(ResourceManager::GetTexture("face"),
		glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glfwSwapBuffers(_win.window);
	glfwPollEvents();
}

void GraphicsSystem::PostDraw()
{
	glfwSwapBuffers(_win.window);
	glfwPollEvents();
}