#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

//Exclusively 2D camera for now.
namespace LILLIS
{
	struct Camera
	{
		Camera() { WIDTH = 600; HEIGHT = 480; };
		Camera(glm::vec2 pos, float w, float h)
		{
			WIDTH = w;
			HEIGHT = h;
			position = pos;
		}


		glm::vec2 position = glm::vec2(0.0f, 0.0f);
		//Also have a rotation ya goober.

		float WIDTH, HEIGHT;

		float fov = 60.0f;
		//float nearPlane = 0.01f;
		//float farPlane = 100.0f;
		//float orthoHeight = 6.0f;
		float aspectRatio = 0;


		inline glm::mat4 viewMatrix()const {
			//Simply determine what's in the camera space here.
			return glm::mat4();
		}
		inline glm::mat4 projectionMatrix()const {
			return glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -1.0f, 1.0f);
		}
	};
}