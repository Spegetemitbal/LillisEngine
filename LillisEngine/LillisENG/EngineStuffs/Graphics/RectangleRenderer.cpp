#include "RectangleRenderer.h"
#include "../GameObject.h"

Component* RectangleRenderer::CreateRectangleRenderer(GameObject* G, int* params)
{
	return G->CreateRenderer(Color(params[0], params[1], params[2]), params[3], params[4]);
}