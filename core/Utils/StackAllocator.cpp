#include "pch.h"
#include "StackAllocator.h"

template <typename T>
T* StackAllocator::alloc()
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
T* StackAllocator::alloc(size_t arrayCount)
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
T* StackAllocator::placementNew()
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
T* StackAllocator::placementNew(size_t arrayCount)
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