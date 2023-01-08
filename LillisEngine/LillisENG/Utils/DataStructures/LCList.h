#pragma once
#include <assert.h>
#include "../LCTypeDefines.h"

namespace LC
{
    template <typename T>
	class LinkedList
	{
	public:

		LinkedList() {}
		~LinkedList() { delete item; }
		LinkedList(const T& rtItem, LinkedList* pkNext)
		{
			item = rtItem; next = pkNext;
		}

		void SetItem(const T& rtItem) { item = rtItem; }
		T& GetItem() { return item; }
		const T& GetItem() const { return item; }
		void SetNext(LinkedList* pkNext) { next = pkNext; }
		LinkedList*& Next() { return next; }
		const LinkedList* GetNext() const { return next; }

		Uint32 getQuantity() const
		{
			int count = 1;
			if (next != nullptr)
			{
				count++;
				LinkedList* iter = next;
				while (iter->next != nullptr)
				{
					iter = iter->next;
					count++;
				}
			}
			return count;
		}

	private:
		T& item;
		LinkedList* next = nullptr;
	};
}