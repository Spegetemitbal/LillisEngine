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
			"uniform vec4 spriteColor;"
			"void main() { vec4 texColor = texture(image, TexCoords);"
			"if (texColor.a == 0.0) {discard;} "
			"color = spriteColor * texColor; }");

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

		particleFragment = std::string(
			"#version 450\n"
			"in vec2 TexCoords;"
			"in float duration;"
			"out vec4 color;"
			"uniform sampler2D image;"
			"uniform vec4 startColor;"
			"uniform vec4 endColor;"
			"uniform float maxTime;"
			"void main() { vec4 texColor = texture(image, TexCoords);"
			"if (texColor.a == 0.0) {discard;} "
			"color = mix(startColor,endColor, 1.0 - (duration / maxTime)) * texColor; }");

		particleVertex = std::string(
		"#version 450\n"
			"layout(location = 0) in vec2 vertex;" // <vec2 position, vec2 texCoords>
			"layout(location = 1) in vec2 halfsize;"
			"layout(location = 2) in float duration;"
			"out vec2 halfSize;"
			"out float dur;"
			"uniform float renderValue;"
			"uniform float upSprite;"
			"uniform float invDist;"
			"uniform float layerDif;"
			"uniform float _ppu;"
			"uniform mat4 projection;"
			"void main(){halfSize = halfsize * _ppu; dur = duration;"
			"gl_Position = projection * vec4(vertex.x * _ppu, vertex.y * _ppu,"
			"-(renderValue - (((upSprite - vertex.y) * invDist) * layerDif)), 1.0);}");

		particleGeometry = std::string(
			"#version 450\n"
			"layout(points) in;"
			"in vec2 halfSize[];"
			"in float dur[];"
			"uniform vec4 TexQuad;"
			"uniform mat4 projection;"
			"out vec2 TexCoords;"
			"out float duration;"
			"layout (triangle_strip, max_vertices = 4) out;"
			"void build_quad(vec4 position) {"
			"gl_Position = position + projection * vec4(-halfSize[0].x, -halfSize[0].y, 0.0, 0.0);"    // 1:bottom-left
			"TexCoords = vec2(TexQuad.x, TexQuad.z);"
			"duration = dur[0];"
			"EmitVertex();"
			"gl_Position = position + projection * vec4( halfSize[0].x, -halfSize[0].y, 0.0, 0.0);"    // 2:bottom-right
			"TexCoords = vec2(TexQuad.y, TexQuad.z);"
			"duration = dur[0];"
			"EmitVertex();"
			"gl_Position = position + projection * vec4(-halfSize[0].x,  halfSize[0].y, 0.0, 0.0);"    // 3:top-left
			"TexCoords = vec2(TexQuad.x, TexQuad.w);"
			"duration = dur[0];"
			"EmitVertex();"
			"gl_Position = position + projection * vec4( halfSize[0].x,  halfSize[0].y, 0.0, 0.0);"    // 4:top-right
			"TexCoords = vec2(TexQuad.y, TexQuad.w);"
			"duration = dur[0];"
			"EmitVertex();"
			"EndPrimitive();}"
			"void main() { build_quad(gl_in[0].gl_Position);}");

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

		procGenVertex = std::string(
			"#version 450\n"
			"layout(location = 0) in vec2 vertex1;"
			"layout(location = 1) in vec4 vertexColor;"
			"out vec4 color;"
			"uniform mat4 _projection;"
			"uniform float _ppu;"
			"void main(){ color = vertexColor; gl_Position = _projection * vec4(vertex1.x * _ppu, vertex1.y * _ppu,-15.0, 1.0);}");

		procGenFragment = std::string(
			"#version 450\n"
			"in vec4 color;"
			"out vec4 FragColor;"
			"void main(){FragColor = color;}");

	}

	std::string fragment;
	std::string vertex;
	std::string geometry;

	std::string particleFragment;
	std::string particleVertex;
	std::string particleGeometry;

	std::string uiVertex;

	std::string postProcessVertex;
	std::string postProcessFragment;

	std::string procGenVertex;
	std::string procGenFragment;
};