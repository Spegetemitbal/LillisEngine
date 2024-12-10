#pragma once
#include "GameObject.h"
#include "MemoryPool.h"
#include <cstring>

class GameObjPool : public MemoryPool<GameObject>
{
public:
	GameObjPool()
	{
		mCount = 20;
		poolDir.reserve(mCount);
		mPool = DBG_NEW char[sizeToAllocate * mCount];

		char* base = mPool;
		for (int i = 0; i < mCount; i++)
		{
			GameObject* toScoot = AllocateObj<GameObject>(base);
			poolDir.push_back(toScoot);
			objMap.emplace(toScoot->GetID(), toScoot);
			base += sizeToAllocate;
		}
	}
	~GameObjPool() = default;

	GameObjPool(unsigned int numComp)
	{
		mCount = numComp;
		poolDir.reserve(mCount);
		mPool = DBG_NEW char[sizeToAllocate * mCount];

		char* base = mPool;
		for (int i = 0; i < mCount; i++)
		{
			GameObject* toScoot = AllocateObj<GameObject>(base);
			poolDir.push_back(toScoot);
			objMap.emplace(toScoot->GetID(), toScoot);
			base += sizeToAllocate;
		}
	}

	GameObject* AddObject()
	{
		if (activeLine == mCount)
		{
			ResizePool();
		}
		activeLine++;
		numActive++;
		poolDir[activeLine - 1]->SetActive(true);
		return poolDir[activeLine - 1];
	}

	void DestroyObject(GameObject* obj)
	{
		auto f = std::find(poolDir.begin(), poolDir.end(), obj);

		if (f != poolDir.end())
		{
			size_t index = distance(poolDir.begin(), f);
			poolDir[index]->SetActive(false);
			poolDir[index]->isEnabled = false;
			--numActive;
		}

		if (numActive / activeLine < 0.5)
		{
			CompactPool();
		}
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

protected:

	//Two finger compaction
	void CompactPool() override
	{
		size_t freeSpace = 0;
		size_t scan = activeLine;

		while (freeSpace < scan)
		{
			while (poolDir[freeSpace]->GetActive() == true && freeSpace < scan)
			{
				freeSpace++;
			}

			while (poolDir[scan]->GetActive() == false && freeSpace < scan)
			{
				scan--;
			}

			if (freeSpace < scan)
			{
				GameObject compacted = *poolDir[freeSpace];
				*poolDir[freeSpace] = *poolDir[scan];
				*poolDir[scan] = compacted;
				objMap[poolDir[freeSpace]->GetID()] = poolDir[freeSpace];
				objMap[poolDir[scan]->GetID()] = poolDir[scan];
				activeLine--;
			}
		}
	}

	void ResizePool() override
	{
		//Clear previous data (that isn't needed)
		objMap.clear();
		poolDir.resize(poolDir.size() * 2);
		unsigned int preActiveLine = activeLine;
		activeLine = 0;
		vector<GameObject*> tempDir(poolDir);
		poolDir.clear();

		//Make new pool to copy things over to.
		char* tempPool = DBG_NEW char[2 * sizeof(GameObject) * mCount];
		size_t sizeToAllocate = sizeof(GameObject);

		//Populate New Pool
		char* base = tempPool;
		for (int i = 0; i < mCount * 2; i++)
		{
			GameObject* toScoot = AllocateObj<GameObject>(base);
			poolDir.push_back(toScoot);
			base += sizeToAllocate;
		}

		//Copy all active objects over from previous pool to new pool.
		int nextSpace = 0;
		for (int i = 0; i < preActiveLine; i++)
		{
			if (tempDir[i]->GetActive() == true)
			{
				*poolDir[nextSpace] = *tempDir[i];
				nextSpace++;
				activeLine++;
			}
		}

		//Final swap over.
		mCount *= 2;
		delete[] mPool;
		mPool = tempPool;
		//Reload Map
		for (int i = 0; i < mCount; i++)
		{
			objMap.emplace(poolDir[i]->GetID(), poolDir[i]);
		}
	}

	size_t sizeToAllocate = sizeof(GameObject);
	unsigned int activeLine = 0;
};
