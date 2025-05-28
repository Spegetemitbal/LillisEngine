#pragma once
#include "GameObject.h"
#include "MemoryPool.h"
#include <cstring>

class GameObjPool : public MemoryPool
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
	~GameObjPool()
	{
		for (int i = 0; i < poolDir.size(); i++)
		{
			poolDir[i]->~GameObject();
		}
	}

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

	GameObject* AddObject(const std::string& name)
	{
		if (activeLine == mCount)
		{
			ResizePool();
		}
		activeLine++;
		poolDir[activeLine - 1]->objName = name;
		poolDir[activeLine - 1]->SetActive(true);
		objNames.insert({name, {this, poolDir[activeLine - 1]->GetID()}});
		return poolDir[activeLine - 1];
	}

	void DestroyObject(GameObject* obj)
	{
		auto f = std::find(poolDir.begin(), poolDir.end(), obj);

		if (f != poolDir.end())
		{
			size_t index = distance(poolDir.begin(), f);
			poolDir[index]->SetActive(false);
			objNames.erase(poolDir[index]->objName);
		}
	}

	//Wipes the pool, doesn't do so on the Object's side
	void ClearPool()
	{
		for (int i = 0; i < poolDir.size(); i++)
		{
			poolDir[i]->SetActive(false);
		}
		objNames.clear();
		activeLine = 0;
	}

	unsigned int GetActiveLine() { return activeLine; };

	//Two finger compaction
	void CompactPool(int active) override
	{
		if (activeLine == 0)
		{
			return;
		}

		if (active / activeLine > 0.5)
		{
			size_t freeSpace = 0;
			size_t scan = 0;
			//First is origin, second is forwarding
			std::vector<std::pair<size_t,size_t>> forwarding = std::vector<std::pair<size_t,size_t>>();

			while (scan < activeLine)
			{
				if (poolDir[scan]->GetActive())
				{
					forwarding.emplace_back(scan, freeSpace);
					freeSpace++;
				} else
				{
					objNames.erase(poolDir[scan]->objName);
				}
				scan++;
			}

			for (int i = 0; i < forwarding.size(); i++)
			{
				pair<size_t,size_t> f = forwarding[i];
				SwapObjects(poolDir[f.first], poolDir[f.second]);
				activeLine--;
			}
		}
	}

	void SwapObjects(GameObject* obj1, GameObject* obj2)
	{
		if (obj1 == obj2)
		{
			return;
		}

		objMap[obj1->GetID()] = obj2;
		objMap[obj2->GetID()] = obj1;

		GameObject temp = *obj1;
		*obj1 = *obj2;
		*obj2 = temp;
	}

	ActiveTracker<GameObject*> getPool() {return {poolDir, activeLine, this};}

	LilObj<GameObject> GetObjectByName(const std::string& name)
	{
		if (objNames.find(name) != objNames.end())
		{
			return objNames[name];
		}
		return {};
	}

protected:
	std::vector<GameObject*> poolDir;
	std::unordered_map<std::string, LilObj<GameObject>> objNames;

	unsigned int FindObjectIndex(GameObject* g)
	{
		auto it = std::find(poolDir.begin(), poolDir.end(), g);
		return std::distance(poolDir.begin(), it);
	}

	void ResizePool() override
	{
		//Clear previous data (that isn't needed)
		objMap.clear();
		objNames.clear();
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
			if (tempDir[i]->GetActive())
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
			if (poolDir[i]->GetActive())
			{
				objNames.insert({poolDir[i]->objName, {this, poolDir[i]->GetID()}});
			}
			objMap.emplace(poolDir[i]->GetID(), poolDir[i]);
		}
	}

	size_t sizeToAllocate = sizeof(GameObject);
	unsigned int activeLine = 0;
};
