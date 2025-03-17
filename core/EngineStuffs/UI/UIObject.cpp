//
// Created by Somed on 3/15/2025.
//

#include "UIObject.h"

#include "UISystem.h"

void UIObject::SetLayer(unsigned int lyr)
{
    layer = lyr;
    UISystem::getInstance()->UpdateUILayer(this);
}
