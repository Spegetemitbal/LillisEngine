#pragma once
#include "Component.h"
#include "MemoryPool.h"
#include <cstring>

#define POOL_PARENT MemoryPool<Comp>

template <typename Comp>
class ComponentPool : public MemoryPool<Comp>
{
public:
	//So apparently template objects require all functions to be written in the .h
	ComponentPool()
	{
		POOL_PARENT::mCount = 20;
		POOL_PARENT::poolDir.reserve(POOL_PARENT::mCount);
		activeCheckDir.reserve(POOL_PARENT::mCount);
		POOL_PARENT::mPool = DBG_NEW char[sizeof(Comp) * POOL_PARENT::mCount];

		size_t sizeToAllocate = sizeof(Comp);
		char* base = POOL_PARENT::mPool;
		try
		{
			for (int i = 0; i < count; i++)
			{
				Comp* toScoot = POOL_PARENT::AllocateObj(base);
				activeCheckDir.push_back(toScoot);
				POOL_PARENT::poolDir.push_back(toScoot);
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
			POOL_PARENT::poolDir.reserve(POOL_PARENT::mCount);
			activeCheckDir.reserve(POOL_PARENT::mCount);
			POOL_PARENT::mPool = DBG_NEW char[sizeof(Comp) * POOL_PARENT::mCount];

			size_t sizeToAllocate = sizeof(Comp);
			char* base = POOL_PARENT::mPool;
			try
			{
				for (int i = 0; i < POOL_PARENT::mCount; i++)
				{
					Comp* toScoot = POOL_PARENT::AllocateObj(base);
					activeCheckDir.push_back(toScoot);
					POOL_PARENT::poolDir.push_back(toScoot);
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
			activeCheckDir[activeLine]->isActive = true;
			activeCheckDir[activeLine]->isEnabled = true;
			activeLine++;
			++POOL_PARENT::numActive;
			return POOL_PARENT::poolDir[activeLine - 1];
		}

		void DestroyComponent(Comp& comp)
		{
			auto f = std::find(POOL_PARENT::poolDir.begin(), POOL_PARENT::poolDir.end(), comp);

			if (f != POOL_PARENT::poolDir.end())
			{
				size_t index = distance(POOL_PARENT::poolDir.begin(), f);
				activeCheckDir[index]->isActive = false;
				activeCheckDir[index]->isEnabled = false;
				--POOL_PARENT::numActive;
			}

			//Maybe find a better time to call this?
			if (POOL_PARENT::numActive / activeLine < 0.5)
			{
				CompactPool();
			}
		}

		//Wipes the pool, doesn't do so on the Object's side
		void ClearPool()
		{
			for (int i = 0; i < activeCheckDir.size(); i++)
			{
				POOL_PARENT::objMap.clear();
				activeCheckDir[i]->isActive = false;
				activeCheckDir[i]->isEnabled = false;
				activeCheckDir[i]->setControlledObject(LilObj<GameObject>());
			}
			activeLine = 0;
		}

		unsigned int GetActiveLine() { return activeLine; };

	protected:

		void SortPool()
		{
		}
		//Two finger compaction
		void CompactPool() override
		{
			size_t freeSpace = 0;
			size_t scan = activeLine;

			while (freeSpace < scan)
			{
				while (activeCheckDir[freeSpace]->isActive == true && freeSpace < scan)
				{
					freeSpace++;
				}

				while (activeCheckDir[scan]->isActive == false && freeSpace < scan)
				{
					scan--;
				}

				if (freeSpace < scan)
				{
					*activeCheckDir[freeSpace] = *activeCheckDir[scan];
					activeCheckDir[scan]->isActive = false;
					activeLine--;
				}
			}
		}
		void ResizePool() override
		{
			//Clear previous data (that isn't needed)
			POOL_PARENT::objMap.clear();
			POOL_PARENT::poolDir.resize(POOL_PARENT::poolDir.size() * 2);
			activeCheckDir.resize(activeCheckDir.size() * 2);
			unsigned int preActiveLine = activeLine;
			activeLine = 0;
			vector<Comp*> tempDir(POOL_PARENT::poolDir);
			POOL_PARENT::poolDir.clear();

			//Make new pool to copy things over to.
			char* tempPool = DBG_NEW char[2 * sizeof(Comp) * POOL_PARENT::mCount];
			size_t sizeToAllocate = sizeof(Comp);

			//Populate New Pool
			char* base = tempPool;
			for (int i = 0; i < POOL_PARENT::mCount * 2; i++)
			{
				Comp* toScoot = POOL_PARENT::AllocateObj(base);
				POOL_PARENT::poolDir.push_back(toScoot);
				activeCheckDir.push_back(toScoot);
				base += sizeToAllocate;
			}

			//Copy all active objects over from previous pool to new pool.
			int nextSpace = 0;
			for (int i = 0; i < preActiveLine; i++)
			{
				if (activeCheckDir[i]->isActive == true)
				{
					*POOL_PARENT::poolDir[nextSpace] = *tempDir[i];
					nextSpace++;
					activeLine++;
				}
			}

			//Vector swap
			activeCheckDir.clear();
			activeCheckDir.assign(POOL_PARENT::poolDir.begin(), POOL_PARENT::poolDir.end());

			//Final swap over.
			POOL_PARENT::mCount *= 2;
			delete[] POOL_PARENT::mPool;
			POOL_PARENT::mPool = tempPool;
			//Reload Map
			for (int i = 0; i < POOL_PARENT::mCount; i++)
			{
				POOL_PARENT::objMap.emplace(activeCheckDir[i]->GetID(), POOL_PARENT::poolDir[i]);
			}
		}

		std::vector<Component*> activeCheckDir;

		unsigned int activeLine = 0;
};

