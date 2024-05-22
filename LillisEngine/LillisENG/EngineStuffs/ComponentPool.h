#pragma once
#include "Component.h"

template <typename Obj>
class ComponentPool
{
public:
	ComponentPool();
	~ComponentPool() { delete mPool; };
	ComponentPool(unsigned int numComp);

	Component AddComponent();
	void DestroyComponent(Component& comp);

	//Typecast to specific component in game loop.
	std::vector<Component*> poolDir;

private:

	void SortPool();
	void ResizePool();

	Obj* AllocateObj(char* base);

	bool isGameObj = false;

	unsigned int activeLine = 0;
	unsigned int count = 0;
	char* mPool = nullptr;
};

