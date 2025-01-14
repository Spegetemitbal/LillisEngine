//
// Created by Somed on 1/13/2025.
//

#include "LilCursor.h"

LilCursor::LilCursor(int width, int height, unsigned char* data)
{
    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = data;

    mCursor = glfwCreateCursor(&image, 0, 0);
}
