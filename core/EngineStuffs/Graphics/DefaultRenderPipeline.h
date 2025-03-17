#pragma once

class DefaultRenderPipeline
{
public:
	DefaultRenderPipeline()
	{
		fragment = std::string(
			"#version 450\n"
			"in vec2 TexCoords;"
			"out vec4 color;"
			"uniform sampler2D image;"
			"uniform vec3 spriteColor;"
			"void main() { color = vec4(spriteColor, 1.0) * texture(image, TexCoords); }");

		vertex = std::string(
			"#version 450\n"
			"layout(location = 0) in vec2 vertex;" // <vec2 position, vec2 texCoords>
			"layout(location = 1) in vec2 texCoord;"
			"out vec2 TexCoords;"
			"uniform mat4 model;"
			"uniform float renderValue;"
			"uniform mat4 projection;"
			"void main(){ TexCoords = texCoord;"
			"gl_Position = projection * model * vec4(vertex.xy, -renderValue, 1.0);}");

		uiVertex = std::string(
			"#version 450\n"
			"layout(location = 0) in vec2 vertex;" // <vec2 position, vec2 texCoords>
			"layout(location = 1) in vec2 texCoord;"
			"out vec2 TexCoords;"
			"void main(){ TexCoords = texCoord;"
			"gl_Position = vec4(vertex.xy, 0.0, 1.0);}");

		postProcessVertex = std::string(
			"#version 450\n"
			"out vec2 UV;"
			"vec4 vertices[3] = { vec4(-1,-1,0,0), vec4(3,-1,2,0), vec4(-1,3,0,2)};"
			"void main(){UV = vertices[gl_VertexID].zw;"
			"gl_Position = vec4(vertices[gl_VertexID].xy,0,1);}");

		postProcessFragment = std::string(
			"#version 450\n"
			"out vec4 FragColor;"
			"in vec2 UV;"
			"uniform sampler2D _ColorBuffer;"
			"void main(){vec3 color = texture(_ColorBuffer,UV).rgb; FragColor = vec4(color,1.0);}");
	}

	std::string fragment;
	std::string vertex;
	std::string geometry;

	std::string uiVertex;

	std::string postProcessVertex;
	std::string postProcessFragment;
};