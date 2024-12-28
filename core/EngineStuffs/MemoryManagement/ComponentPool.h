#pragma once
#include "Component.h"
#include "MemoryPool.h"
#include <cstring>

#define POOL_PARENT MemoryPool

template <typename Comp>
class ComponentPool : public MemoryPool
{
public:
	//So apparently template objects require all functions to be written in the .h
	ComponentPool()
	{
		POOL_PARENT::mCount = 20;
		poolDir.reserve(POOL_PARENT::mCount);
		activeCheckDir.reserve(POOL_PARENT::mCount);
		POOL_PARENT::mPool = DBG_NEW char[sizeof(Comp) * POOL_PARENT::mCount];

		size_t sizeToAllocate = sizeof(Comp);
		char* base = POOL_PARENT::mPool;
		try
		{
			for (int i = 0; i < count; i++)
			{
				Comp* toScoot = POOL_PARENT::template AllocateObj<Comp>(base);
				activeCheckDir.push_back(toScoot);
				poolDir.push_back(toScoot);
				POOL_PARENT::objMap.emplace(activeCheckDir.back()->GetID(), toScoot);
				base += sizeToAllocate;
			}
		}
		catch (...)
		{
			std::cout << "Non Component Type!, Failure imminent.";
			exit(1);
		}
	}
	~ComponentPool()
		{
			//delete[] MemoryPool<Comp>::mPool;
			activeCheckDir.clear();
			//poolDir.clear();
		}
		ComponentPool(unsigned int numComp)
		{
			POOL_PARENT::mCount = numComp;
			poolDir.reserve(POOL_PARENT::mCount);
			activeCheckDir.reserve(POOL_PARENT::mCount);
			POOL_PARENT::mPool = DBG_NEW char[sizeof(Comp) * POOL_PARENT::mCount];

			size_t sizeToAllocate = sizeof(Comp);
			char* base = POOL_PARENT::mPool;
			try
			{
				for (int i = 0; i < POOL_PARENT::mCount; i++)
				{
					Comp* toScoot = POOL_PARENT::template AllocateObj<Comp>(base);
					activeCheckDir.push_back(toScoot);
					poolDir.push_back(toScoot);
					POOL_PARENT::objMap.emplace(activeCheckDir.back()->GetID(), toScoot);
					base += sizeToAllocate;
				}
			}
			catch (...)
			{
				std::cout << "Non Component Type!, Failure imminent.";
				exit(1);
			}
		}

		Comp* AddComponent()
		{
			if (activeLine == POOL_PARENT::mCount)
			{
				ResizePool();
			}
			activeCheckDir[activeLine]->SetActive(true);
			activeLine++;
			return poolDir[activeLine - 1];
		}

		void DestroyComponent(Comp& comp)
		{
			auto f = std::find(poolDir.begin(), poolDir.end(), comp);

			if (f != poolDir.end())
			{
				size_t index = distance(poolDir.begin(), f);
				activeCheckDir[index]->SetActive(false);
			}
		}

		//Wipes the pool, doesn't do so on the Object's side
		void ClearPool()
		{
			for (int i = 0; i < activeCheckDir.size(); i++)
			{
				//POOL_PARENT::objMap.clear();
				activeCheckDir[i]->SetActive(false);
				activeCheckDir[i]->setControlledObject(LilObj<GameObject>());
			}
			activeLine = 0;
		}

		unsigned int GetActiveLine() { return activeLine; };

		//Two finger compaction
		void CompactPool(int active) override
		{
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

		void SwapObjects(Comp* obj1, Comp* obj2)
		{
			objMap[obj1->GetID()] = obj2;
			objMap[obj2->GetID()] = obj1;

			Comp temp = *obj1;
			*obj1 = *obj2;
			*obj2 = temp;
		}

	ActiveTracker<Comp*> getPool() {return {poolDir};}

	protected:

		void SortPool()
		{
		}

		void ResizePool() override
		{
			//Clear previous data (that isn't needed)
			POOL_PARENT::objMap.clear();
			poolDir.resize(poolDir.size() * 2);
			activeCheckDir.resize(activeCheckDir.size() * 2);
			unsigned int preActiveLine = activeLine;
			activeLine = 0;
			vector<Comp*> tempDir(poolDir);
			poolDir.clear();

			//Make new pool to copy things over to.
			char* tempPool = DBG_NEW char[2 * sizeof(Comp) * POOL_PARENT::mCount];
			size_t sizeToAllocate = sizeof(Comp);

			//Populate New Pool
			char* base = tempPool;
			for (int i = 0; i < POOL_PARENT::mCount * 2; i++)
			{
				Comp* toScoot = POOL_PARENT::template AllocateObj<Comp>(base);
				poolDir.push_back(toScoot);
				activeCheckDir.push_back(toScoot);
				base += sizeToAllocate;
			}

			//Copy all active objects over from previous pool to new pool.
			int nextSpace = 0;
			for (int i = 0; i < preActiveLine; i++)
			{
				if (activeCheckDir[i]->GetActive() == true)
				{
					*poolDir[nextSpace] = *tempDir[i];
					nextSpace++;
					activeLine++;
				}
			}

			//Vector swap
			activeCheckDir.clear();
			activeCheckDir.assign(poolDir.begin(), poolDir.end());

			//Final swap over.
			POOL_PARENT::mCount *= 2;
			delete[] POOL_PARENT::mPool;
			POOL_PARENT::mPool = tempPool;
			//Reload Map
			for (int i = 0; i < POOL_PARENT::mCount; i++)
			{
				POOL_PARENT::objMap.emplace(activeCheckDir[i]->GetID(), poolDir[i]);
			}
		}

		std::vector<Comp*> poolDir;
		std::vector<Component*> activeCheckDir;

		unsigned int activeLine = 0;
};

