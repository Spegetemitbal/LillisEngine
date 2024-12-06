#pragma once
#include "Component.h"
#include "MemoryPool.h"
#include <cstring>

template <typename Comp>
class ComponentPool : public MemoryPool<Comp>
{
public:
	//So apparently template objects require all functions to be written in the .h
	ComponentPool()
	{
		MemoryPool<Comp>::count = 20;
		poolDir.reserve(count);
		activeCheckDir.reserve(MemoryPool<Comp>::count);
		MemoryPool<Comp>::mPool = DBG_NEW char[sizeof(Comp) * count];

		size_t sizeToAllocate = sizeof(Comp);
		char* base = MemoryPool<Comp>::mPool;
		try
		{
			for (int i = 0; i < count; i++)
			{
				Comp* toScoot = MemoryPool<Comp>::AllocateObj(base);
				activeCheckDir.push_back(toScoot);
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
	~ComponentPool()
		{
			delete[] MemoryPool<Comp>::mPool;
			activeCheckDir.clear();
			poolDir.clear();
		}
		ComponentPool(unsigned int numComp)
		{
			MemoryPool<Comp>::count = numComp;
			poolDir.reserve(count);
			activeCheckDir.reserve(MemoryPool<Comp>::count);
			MemoryPool<Comp>::mPool = DBG_NEW char[sizeof(Comp) * count];

			size_t sizeToAllocate = sizeof(Comp);
			char* base = MemoryPool<Comp>::mPool;
			try
			{
				for (int i = 0; i < count; i++)
				{
					Comp* toScoot = MemoryPool<Comp>::AllocateObj(base);
					activeCheckDir.push_back(toScoot);
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

		Comp* AddComponent()
		{
			if (activeLine == count)
			{
				ResizePool();
			}
			activeCheckDir[activeLine]->isActive = true;
			activeCheckDir[activeLine]->isEnabled = true;
			activeLine++;
			return poolDir[activeLine - 1];
		}

		void DestroyComponent(Comp& comp)
		{
			std::iterator f = std::find(poolDir.begin(), poolDir.end(), comp);

			if (f != poolDir.end())
			{
				int index = distance(poolDir.begin(), f);
				activeCheckDir[index]->isActive = false;
				activeCheckDir[index]->isEnabled = false;
				MemoryPool<Comp>::numActive--;
			}

			if (MemoryPool<Comp>::numActive / activeLine < 0.5)
			{
				SortPool();
			}
		}

		//Wipes the pool, doesn't do so on the Object's side
		void ClearPool()
		{
			for (int i = 0; i < activeCheckDir.size(); i++)
			{
				activeCheckDir[i]->isActive = false;
				activeCheckDir[i]->isEnabled = false;
				activeCheckDir[i]->setControlledObject(nullptr);
			}
			activeLine = 0;
		}

		unsigned int GetActiveLine() { return activeLine; };

		std::vector<Comp*> poolDir;

	protected:

		void SortPool() {}
		void CompactPool() override {}
		void ResizePool() override
		{

			poolDir.resize(poolDir.size() * 2);
			activeCheckDir.resize(activeCheckDir.size() * 2);
			poolDir.clear();
			activeCheckDir.clear();
			char* tempPool = DBG_NEW char[2 * sizeof(Comp) * count];
			//std::copy(std::begin(mPool), std::end(mPool), std::begin(tempPool));
			std::strcpy(MemoryPool<Comp>::mPool, tempPool);
			size_t sizeToAllocate = sizeof(Comp);
			char* base = tempPool + (sizeof(Comp) * count);
			char* start = tempPool;

			for (int i = 0; i < count; i++)
			{
				//Probably don't do this much...
				Comp* toScoot = reinterpret_cast<Comp*>(start);
				activeCheckDir.push_back(toScoot);
				poolDir.push_back(toScoot);
				start += sizeToAllocate;
			}

			for (int i = 0; i < count; i++)
			{
				Comp* toScoot = MemoryPool<Comp>::AllocateObj(base);
				activeCheckDir.push_back(toScoot);
				poolDir.push_back(toScoot);

				base += sizeToAllocate;
			}
			count *= 2;
			delete[] MemoryPool<Comp>::mPool;
			MemoryPool<Comp>::mPool = tempPool;
		}

		std::vector<Component*> activeCheckDir;

		unsigned int activeLine = 0;
};

