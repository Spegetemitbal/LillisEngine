#include "pch.h"
#include "ComponentPool.h"

template <typename Obj>
Obj* ComponentPool<Obj>::AllocateObj(char* base)
{
	size_t sizeToAllocate = sizeof(Obj);
	return new (base)Obj();
}

template <typename Obj>
ComponentPool<Obj>::ComponentPool()
{
	count = 100;
	poolDir.reserve(count);
	mPool = new char[sizeof(Obj) * 100];

	size_t sizeToAllocate = sizeof(Obj);
	char* base = mPool;
	for (int i = 0; i < count; i++)
	{
		try
		{
			poolDir.push_back(AllocateObj(base));
		}
		catch (...)
		{
			std::cout << "Non Component Type!, Failure imminent.";
			exit(1);
		}
		base += sizeToAllocate;
	}
}

template <typename Obj>
ComponentPool<Obj>::ComponentPool(unsigned int num)
{
	count = num;
	poolDir.reserve(count);
	mPool = new char[sizeof(Obj) * num];

	size_t sizeToAllocate = sizeof(Obj);
	char* base = mPool;
	for (int i = 0; i < count; i++)
	{
		try
		{
			poolDir.push_back(AllocateObj(base));
		}
		catch (...)
		{
			std::cout << "Non Component Type!, Failure imminent.";
			exit(1);
		}
		base += sizeToAllocate;
	}
}

template <typename Obj>
Component ComponentPool<Obj>::AddComponent()
{

}

template <typename Obj>
void ComponentPool<Obj>::DestroyComponent(Component& comp)
{

}

template <typename Obj>
void ComponentPool<Obj>::SortPool()
{

}

template <typename Obj>
void ComponentPool<Obj>::ResizePool()
{

}