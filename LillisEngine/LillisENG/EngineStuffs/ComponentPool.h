#pragma once
#include "Component.h"

class ComponentPool
{
public:
	ComponentPool();
	~ComponentPool() { delete mPool; };
	ComponentPool(unsigned int numComp);

	Component AddComponent();
	void DestroyComponent(Component& comp);

private:

	void SortPool();
	void ResizePool();

	unsigned int activeLine = 0;
	unsigned int count = 0;
	Component* mPool;
};

