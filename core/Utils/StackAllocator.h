#pragma once

namespace LILLIS
{
	typedef char byte;
}
class StackAllocator
{
public:
	template <typename T>
	T* alloc();

	template <typename T>
	T* alloc(size_t arrayCount);

	template <typename T>
	T* placementNew();

	template <typename T>
	T* placementNew(size_t arrayCount);

	//Frees everything
	void clearStack() { base = buffer; }

	StackAllocator() { stackSize = 1024 * 1024 * 32; buffer = new LILLIS::byte[stackSize]; base = buffer; }
	StackAllocator(int MB) { stackSize = 1024 * 1024 * MB; buffer = new LILLIS::byte[stackSize]; base = buffer; }
	~StackAllocator() { delete[] buffer; }
private:
	//because chars are 1 byte, you literally just replace them with whatever.
	LILLIS::byte* buffer = nullptr;
	LILLIS::byte* base = nullptr;
	int stackSize = 0;
};

// Normally, we have two options for allocations:
// * Stack: Everything is deallocated as soon as it goes out of scope, so no chance to access invalid memory on accident;
// * Heap: Manually allocate, manually deallocate. 1 new <-> 1 delete. Possible to access invalid memory on accident.
// * Our stack allocator: Manually allocate, *automatic* deallocate. If  keep a pointer around too long, it can "magically"
//		Change values, so watch out.

//ALWAYS ALWAYS clear stack at the end of the frame, do not try and make variables to use outside of that frame. Ever.