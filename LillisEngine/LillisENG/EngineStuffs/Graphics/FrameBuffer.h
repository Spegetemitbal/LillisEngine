#pragma once

namespace LILLIS 
{

	struct FrameBuffer
	{
		unsigned int fbo;
		unsigned int colorBuffer[8];
		unsigned int depthBuffer;
		unsigned int width;
		unsigned int height;

	};
	FrameBuffer createFrameBuffer(unsigned int width, unsigned int height, int colorFormat);
}