#pragma once
#include "GameObject.h"
#include <cstring>

class GameObjPool
{
public:
	GameObjPool()
	{
		count = 20;
		poolDir.reserve(count);
		mPool = DBG_NEW char[sizeToAllocate * count];

		char* base = mPool;
		for (int i = 0; i < count; i++)
		{
			GameObject* toScoot = AllocateObj(base);
			poolDir.push_back(toScoot);

			base += sizeToAllocate;
		}
	}
	~GameObjPool()
	{
		delete[] mPool;
		poolDir.clear();
	}
	GameObjPool(unsigned int numComp)
	{
		count = numComp;
		poolDir.reserve(count);
		mPool = DBG_NEW char[sizeToAllocate * count];

		char* base = mPool;
		for (int i = 0; i < count; i++)
		{
			GameObject* toScoot = AllocateObj(base);
			poolDir.push_back(toScoot);

			base += sizeToAllocate;
		}
	}

	GameObject* AddObject()
	{
		if (activeLine == count)
		{
			ResizePool();
		}
		activeLine++;
		poolDir[activeLine - 1]->SetActive(true);
		return poolDir[activeLine - 1];
	}

	//Wipes the pool, doesn't do so on the Object's side
	void ClearPool()
	{
		for (int i = 0; i < poolDir.size(); i++)
		{
			poolDir[i]->SetActive(false);
		}
		activeLine = 0;
	}

	unsigned int GetActiveLine() { return activeLine; };

	std::vector<GameObject*> poolDir;

private:

	void SortPool() {}
	void ResizePool()
	{

		poolDir.resize(poolDir.size() * 2);
		poolDir.clear();
		char* tempPool = DBG_NEW char[2 * sizeof(GameObject) * count];
		//std::copy(std::begin(mPool), std::end(mPool), std::begin(tempPool));
		std::strcpy(mPool, tempPool);
		size_t sizeToAllocate = sizeof(GameObject);
		char* base = tempPool + (sizeof(GameObject) * count);
		char* start = tempPool;

		for (int i = 0; i < count; i++)
		{
			//Probably don't do this much...
			GameObject* toScoot = reinterpret_cast<GameObject*>(start);
			poolDir.push_back(toScoot);
			start += sizeToAllocate;
		}

		for (int i = 0; i < count; i++)
		{
			GameObject* toScoot = AllocateObj(base);
			poolDir.push_back(toScoot);

			base += sizeToAllocate;
		}
		count *= 2;
		delete[] mPool;
		mPool = tempPool;
	}

	GameObject* AllocateObj(char* base)
	{
		return new (base)GameObject();
	}

	bool isGameObj = false;

	size_t sizeToAllocate = sizeof(GameObject);
	unsigned int activeLine = 0;
	unsigned int numActive = 0;
	unsigned int count = 0;
	char* mPool = nullptr;
};
