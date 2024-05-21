#include "pch.h"
#include "ComponentPool.h"

ComponentPool::ComponentPool()
{
	mPool = new Component[100];
	count = 100;
}

ComponentPool::ComponentPool(unsigned int num)
{
	mPool = new Component[num];
	count = num;
}

Component ComponentPool::AddComponent()
{
	if (activeLine < count)
	{
		mPool[activeLine] = Component();
	}
	else
	{
		ResizePool();

	}
}

void ComponentPool::DestroyComponent(Component& comp)
{

}

void ComponentPool::SortPool()
{

}

void ComponentPool::ResizePool()
{

}