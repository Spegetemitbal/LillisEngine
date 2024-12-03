#pragma once
#include "Component.h"
#include <cstring>

namespace LILLIS
{
	template <typename Obj>
	class ComponentPool
	{
	public:
		//So apparently template objects require all functions to be written in the .h
		ComponentPool()
		{
			count = 20;
			poolDir.reserve(count);
			activeCheckDir.reserve(count);
			mPool = DBG_NEW char[sizeof(Obj) * count];

			size_t sizeToAllocate = sizeof(Obj);
			char* base = mPool;
			try
			{
				for (int i = 0; i < count; i++)
				{
					Obj* toScoot = AllocateObj(base);
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
			delete[] mPool; 
			activeCheckDir.clear();
			poolDir.clear();
		}
		ComponentPool(unsigned int numComp)
		{
			count = numComp;
			poolDir.reserve(count);
			activeCheckDir.reserve(count);
			mPool = DBG_NEW char[sizeof(Obj) * count];

			size_t sizeToAllocate = sizeof(Obj);
			char* base = mPool;
			try
			{
				for (int i = 0; i < count; i++)
				{
					Obj* toScoot = AllocateObj(base);
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

		Obj* AddComponent()
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

		void DestroyComponent(Obj& comp)
		{
			std::iterator f = std::find(poolDir.begin(), poolDir.end(), comp);

			if (f != poolDir.end())
			{
				int index = distance(poolDir.begin(), f);
				activeCheckDir[index]->isActive = false;
				activeCheckDir[index]->isEnabled = false;
				numActive--;
			}

			if (numActive / activeLine < 0.5)
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

		std::vector<Obj*> poolDir;

	private:

		void SortPool() {}
		void CompactPool() {}
		void ResizePool()
		{

			poolDir.resize(poolDir.size() * 2);
			activeCheckDir.resize(activeCheckDir.size() * 2);
			poolDir.clear();
			activeCheckDir.clear();
			char* tempPool = DBG_NEW char[2 * sizeof(Obj) * count];
			//std::copy(std::begin(mPool), std::end(mPool), std::begin(tempPool));
			std::strcpy(mPool, tempPool);
			size_t sizeToAllocate = sizeof(Obj);
			char* base = tempPool + (sizeof(Obj) * count);
			char* start = tempPool;

			for (int i = 0; i < count; i++)
			{
				//Probably don't do this much...
				Obj* toScoot = reinterpret_cast<Obj*>(start);
				activeCheckDir.push_back(toScoot);
				poolDir.push_back(toScoot);
				start += sizeToAllocate;
			}

			for (int i = 0; i < count; i++)
			{
				Obj* toScoot = AllocateObj(base);
				activeCheckDir.push_back(toScoot);
				poolDir.push_back(toScoot);

				base += sizeToAllocate;
			}
			count *= 2;
			delete[] mPool;
			mPool = tempPool;
		}

		std::vector<Component*> activeCheckDir;

		Obj* AllocateObj(char* base)
		{
			//size_t sizeToAllocate = sizeof(Obj);
			return new (base)Obj();
		}

		unsigned int activeLine = 0;
		unsigned int numActive = 0;
		unsigned int count = 0;
		char* mPool = nullptr;
	};
}
