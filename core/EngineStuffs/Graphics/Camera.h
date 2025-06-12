#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>

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

		inline glm::vec4 getAABB(unsigned int PPU) const
		{
			float ppu = 1.0f / (float)PPU;
			float halfWidth = WIDTH * 0.5f * (float)PPU;
			float halfHeight = HEIGHT * 0.5f * (float)PPU;
			glm::vec2 pos = position * ppu;
			return {pos.x - halfWidth, pos.y - halfHeight, halfWidth + pos.x, halfHeight + pos.y};
		}

		inline glm::mat4 viewMatrix()const {
			//Simply determine what's in the camera space here.
			return glm::mat4();
		}
		inline glm::mat4 projectionMatrix()const {
			float halfWidth = WIDTH * 0.5f;
			float halfHeight = HEIGHT * 0.5f;
			glm::mat4 view = glm::ortho(position.x - halfWidth, halfWidth + position.x, halfHeight + position.y, position.y - halfHeight, 10.0f, 20.0f);
			view[1][1] *= -1.0f;
			return view;
		}
	};
}