#pragma once
#include <assert.h>
#include "../LCTypeDefines.h"

namespace LC
{
	template <typename T>
	class Array
	{
	public:
		Array() { mpArr = new T[mCapacity]; };
		Array(Uint32 num) { assert(num > 0); mQuantity = num; mpArr = new T[num]; mCapacity = num; };
		Array(Uint32 num, bool canGrow) { assert(num > 0); mQuantity = num; mpArr = new T[num]; mAutoGrowth = canGrow; mCapacity = num; };
		Array(const Array& a) { mQuantity = a.mQuantity; mpArr = a.mpArr; mCapacity = a.mCapacity; }
		~Array() { delete[] mpArr; }

		Uint32 getQuantity() const { return mQuantity; }
		T* getArray() { return mpArr; }
		const T* getArray() const { return mpArr; }

		T& operator[] (Uint32 index)
		{
			assert(index < mQuantity);
			return mpArr[index];
		}

		const T& operator[] (Uint32 index)
		{
			assert(index < quantity);
			return arr[index];
		}

		//Adds to back, automatically grows if not big enough to hold new item
		void push_back(const T& rtElement)
		{

			if (mQuantity == mCapacity)
			{
				if (mAutoGrowth)
				{
					mCapacity *= mGrowBy;
					T* newArr = new T[mCapacity];
					for (Uint32 i = 1; i <= mQuantity; i++)
					{
						newArr[i] = mpArr[i];
					}
					delete[] mpArr;
					mpArr = newArr;
				}
				else
				{
					T* newArr = new T[mQuantity + 1];
					for (Uint32 i = 1; i <= mQuantity; i++)
					{
						newArr[i] = mpArr[i];
					}
					delete[] mpArr;
					mpArr = newArr;
					mCapacity++;
				}
			}

			mpArr[mQuantity] = rtElement;
			mQuantity++;
		}

		void push_front(const T& rtElement)
		{
			if (mQuantity == mCapacity)
			{
				if (mAutoGrowth)
				{
					mCapacity *= mGrowBy;
					T* newArr = new T[mCapacity];
					for (Uint32 i = 1; i <= mQuantity; i++)
					{
						newArr[i] = mpArr[i-1];
					}
					delete[] mpArr;
					mpArr = newArr;
				}
				else
				{
					T* newArr = new T[mQuantity + 1];
					for (Uint32 i = 1; i <= mQuantity; i++)
					{
						newArr[i] = mpArr[i-1];
					}
					delete[] mpArr;
					mpArr = newArr;
					mCapacity++;
				}
			}
			
			mpArr[0] = rtElement;
			mQuantity++;
		}

		void setElement(Uint32 index, const T& rtElement)
		{
			assert(index >= 0);
			if (index < mQuantity)
			{
				mpArr[index] = rtElement;
			}
			else
			{
				Uint32 difference = mQuantity + (index - mQuantity) + 1;
				T* newArr = new T[difference];
				for (Uint32 i = 0; i < difference; i++)
				{
					newArr[i] = mpArr[i];
				}
				newArr[index] = rtElement;
				delete[] mpArr;
				mpArr = newArr;
				mQuantity++;
			}
		}

		void remove(Uint32 index)
		{
			for (Uint32 i = index + 1; i < mQuantity; i++)
			{
				mpArr[i - 1] = mpArr[i];
			}
			mQuantity--;
		}

		void removeAll()
		{
			for (Uint32 i = 0; i < mQuantity; i++)
			{
				delete mpArr[i];
				mpArr[i] = nullptr;
			}
			mQuantity = 0;
		}
		
		void setAutoGrow(bool grow) { mAutoGrowth = grow; };
		bool getAutoGrow() { return mAutoGrowth; };

		void setGrowBy(float32 iGrowBy) { assert(iGrowBy > 1); mGrowBy = iGrowBy; };
		Uint32 getGrowBy() const { return mGrowBy; };
	private:

		//Autogrowth is faster but has more memory overhead. Use with caution
		bool mAutoGrowth = false;
		float32 mGrowBy = 2;
		T* mpArr = nullptr;
		Uint32 mQuantity = 1;
		Uint32 mCapacity = 10;
	};
}