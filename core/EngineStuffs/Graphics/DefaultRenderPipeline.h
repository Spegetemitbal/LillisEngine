#pragma once

class DefaultRenderPipeline
{
public:
	DefaultRenderPipeline()
	{
		fragment = std::string(
			"#version 330 core\n"
			"in vec2 TexCoords;"
			"out vec4 color;"
			"uniform sampler2D image;"
			"uniform vec3 spriteColor;"
			"void main() { color = vec4(spriteColor, 1.0) * texture(image, TexCoords); }");

		vertex = std::string(
			"#version 330 core\n"
			"layout(location = 0) in vec2 vertex;" // <vec2 position, vec2 texCoords>
			"layout(location = 1) in vec2 texCoord;"
			"out vec2 TexCoords;"
			"uniform mat4 model;"
			"uniform mat4 projection;"
			"void main(){ TexCoords = texCoord;"
			"gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);}");
	}

	std::string fragment;
	std::string vertex;
	std::string geometry;
};