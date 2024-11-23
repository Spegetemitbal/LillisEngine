#include "FrameBuffer.h"
#include "glad/gl.h"

using namespace LILLIS;

FrameBuffer LILLIS::createFrameBuffer(unsigned int width, unsigned int height, int colorFormat)
{
	FrameBuffer newBuffer = FrameBuffer();
	newBuffer.width = width;
	newBuffer.height = height;

	//FrameBuffer object creation
	glCreateFramebuffers(1, &newBuffer.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, newBuffer.fbo);

	//? bit RGBA color buffer
	glGenTextures(1, newBuffer.colorBuffer);
	glBindTexture(GL_TEXTURE_2D, newBuffer.colorBuffer[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, colorFormat, width, width);
	return newBuffer;
}