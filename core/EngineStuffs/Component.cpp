#include "Component.h"

//unsigned int Component::nextID = 1;

void Component::SetActive(bool active)
{
    isActive = active;
    isEnabled = active;
}

