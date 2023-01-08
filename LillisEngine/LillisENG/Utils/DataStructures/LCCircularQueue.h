#pragma once
#include "..\LCTypeDefines.h"

namespace LC
{
	//Make this code a little less copy pastey soon
	template <class T>
	class CircularQueue
	{
	public:
		explicit CircularQueue(Uint32 size) : mCapacity(size), mFront(0), mBack(0), mNumEntries(0)
		{
			mArray = new T[size];
		}

		~CircularQueue()
		{
			delete[] mArray;
		}

		void reset()
		{
			mFront = 0;
			mBack = 0;
			mNumEntries = 0;
		}

		bool insertAtBeginning(const T& item) //returns true if added successfully
		{
			if (mNumEntries >= mCapacity)
				//no room left
				return false;

			mArray[mBack] = item;
			mBack++;
			mNumEntries++;

			if (mBack >= mCapacity)
			{
				mBack = 0;
			}

			return true;
		}

		bool removeAtBeginning(T& item)//returns true if not empty
		{
			if (mNumEntries == 0)//empty
				return false;

			item = mArray[mFront];
			mFront++;
			mNumEntries--;

			if (mFront >= mCapacity)
			{
				mFront = 0;
			}

			return true;
		}
	private:
		T* mArray;
		Uint32 mCapacity;
		Uint32 mBack;
		Uint32 mFront;
		Uint32 mNumEntries;
	};
}