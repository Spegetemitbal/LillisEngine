//
// Created by Somed on 1/13/2025.
//

#ifndef LILCURSOR_H
#define LILCURSOR_H
#include "Texture.h"
#include "GLFW/glfw3.h"


class LilCursor {
public:
    LilCursor() = delete;
    ~LilCursor() = default;
    LilCursor(int width, int height, unsigned char* data);
private:
    friend class GraphicsSystem;
    GLFWcursor* mCursor;
};



#endif //LILCURSOR_H
