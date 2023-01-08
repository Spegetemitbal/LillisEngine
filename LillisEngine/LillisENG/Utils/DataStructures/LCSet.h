#pragma once
#include "../LCTypeDefines.h"

namespace LC
{
	template <typename T>
	class Set
	{
	public:
		Set()
		{

		}

		Set(const Set& set)
		{

		}

		Set(Uint32 iMaxQuantity, Uint32 growBy)
		{

		}

		~Set()
		{

		}

		Uint32 GetMaxQuantity() const { return maxQuantity; }
		Uint32 GetGrowBy() const { return growBy; }
		Uint32 GetQuantity() const { return quantity; }

		T* GetElements();
		const T* GetElements() const;
		T& operator[] (int i);
		const T& operator[] (int i) const;

	private:
		Uint32 maxQuantity;
		Uint32 growBy;
		Uint32 quantity;
	};
}