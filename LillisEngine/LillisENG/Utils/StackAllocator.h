#pragma once

typedef char byte;

class StackAllocator
{
public:
	template <typename T>
	T* alloc()
	{
		size_t sizeToAllocate = sizeof(T);
		void* allocationPoint = base;

		//Makes sure we're not writing past the end of the stack!
		if (base + sizeToAllocate > buffer + stackSize)
		{
			return nullptr;
		}
		else
		{
			base += sizeToAllocate;
			return (T*)allocationPoint;
		}
	}

	template <typename T>
	T* alloc(size_t arrayCount)
	{
		size_t sizeToAllocate = sizeof(T) * arrayCount;
		void* allocationPoint = base;

		//Makes sure we're not writing past the end of the stack!
		if (base + sizeToAllocate > buffer + stackSize)
		{
			return nullptr;
		}
		else
		{
			base += sizeToAllocate;
			return (T*)allocationPoint;
		}
	}

	template <typename T>
	T* placementNew()
	{
		size_t sizeToAllocate = sizeof(T);
		void* allocationPoint = base;

		if (base + sizeToAllocate > buffer + stackSize)
		{
			return nullptr;
		}
		else
		{
			base += sizeToAllocate;
			return new (allocationPoint)T();
		}
	}

	template <typename T>
	T* placementNew(size_t arrayCount)
	{
		size_t sizeToAllocate = sizeof(T) * arrayCount;
		void* allocationPoint = base;

		if (base + sizeToAllocate > buffer + stackSize)
		{
			return nullptr;
		}
		else
		{
			base += sizeToAllocate;
			return new (allocationPoint)T();
		}

	}

	//Frees everything
	void clearStack() { base = buffer; }

	StackAllocator() { stackSize = 1024 * 1024 * 32; buffer = new byte[stackSize]; base = buffer; }
	StackAllocator(int MB) { stackSize = 1024 * 1024 * MB; buffer = new byte[stackSize]; base = buffer; }
	~StackAllocator() { delete[] buffer; }
private:
	//because chars are 1 byte, you literally just replace them with whatever.
	byte* buffer = nullptr;
	byte* base = nullptr;
	int stackSize = 0;
};

// Normally, we have two options for allocations:
// * Stack: Everything is deallocated as soon as it goes out of scope, so no chance to access invalid memory on accident;
// * Heap: Manually allocate, manually deallocate. 1 new <-> 1 delete. Possible to access invalid memory on accident.
// * Our stack allocator: Manually allocate, *automatic* deallocate. If  keep a pointer around too long, it can "magically"
//		Change values, so watch out.

//ALWAYS ALWAYS clear stack at the end of the frame, do not try and make variables to use outside of that frame. Ever.