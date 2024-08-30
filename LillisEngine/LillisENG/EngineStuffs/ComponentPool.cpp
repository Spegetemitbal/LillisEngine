#include "pch.h"
#include "ComponentPool.h"
#include "Component.h"

using namespace LILLIS;

template <class Obj>
Obj* ComponentPool<Obj>::AllocateObj(char* base)
{
	size_t sizeToAllocate = sizeof(Obj);
	return new (base)Obj();
}

template <class Obj>
ComponentPool<Obj>::ComponentPool()
{
	count = 100;
	poolDir.reserve(count);
	mPool = new char[sizeof(Obj) * 100];

	size_t sizeToAllocate = sizeof(Obj);
	char* base = mPool;
	try 
	{
		for (int i = 0; i < count; i++)
		{
			Obj* toScoot = AllocateObj(base);
			//activeCheckDir.push_back(toScoot);
			poolDir.push_back(toScoot);
		
			base += sizeToAllocate;
		}
	}
	catch (...)
	{
		std::cout << "Non Component Type!, Failure imminent.";
		exit(1);
	}
}

template <class Obj>
ComponentPool<Obj>::ComponentPool(unsigned int num)
{
	count = num;
	poolDir.reserve(count);
	mPool = new char[sizeof(Obj) * num];

	size_t sizeToAllocate = sizeof(Obj);
	char* base = mPool;
	try
	{
		for (int i = 0; i < count; i++)
		{
			Obj* toScoot = AllocateObj(base);
			//activeCheckDir.push_back(toScoot);
			poolDir.push_back(toScoot);

			base += sizeToAllocate;
		}
	}
	catch (...)
	{
		std::cout << "Non Component Type!, Failure imminent.";
		exit(1);
	}
}

template <class Obj>
Obj* ComponentPool<Obj>::AddComponent()
{
	if (activeLine == count)
	{
		ResizePool();
	}

	//Reset thing here.
	//activeCheckDir[activeLine]->initComponent();
	//Possibly move this and do it elsewhere sir...
	//activeCheckDir[activeLine]->isActive = true;
}

template <class Obj>
void ComponentPool<Obj>::DestroyComponent(Obj& comp)
{
	iterator f = std::find(poolDir.begin(), poolDir.end(), comp);
	if (f != poolDir.end())
	{
		int index = distance(poolDir.begin(), f);
		//activeCheckDir[index]->isActive = false;
		numActive--;
	}

	if (numActive / activeLine < 0.5)
	{
		SortPool();
	}
}

template <class Obj>
void ComponentPool<Obj>::SortPool()
{
	//Look up how to do this sir.
}

template <class Obj>
void ComponentPool<Obj>::ResizePool()
{
	poolDir.resize(poolDir.size() * 2);
	char* tempPool = new char[2 * sizeof(mPool)];
	std::copy(std::begin(mPool), std::end(mPool), std::begin(tempPool));

	delete mPool;
	mPool = tempPool;
}