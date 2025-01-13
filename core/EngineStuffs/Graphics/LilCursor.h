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
    LilCursor(Texture2D* texture);
private:
    friend class GraphicsSystem;
    //GLFWimage* mImage;
    GLFWcursor* mCursor;
};



#endif //LILCURSOR_H
