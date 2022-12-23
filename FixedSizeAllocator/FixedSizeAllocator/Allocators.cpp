#include <inttypes.h>
#include "Allocators.h"

#include <stdio.h>


void * __cdecl malloc(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("malloc %zu\n", i_size);
	return _aligned_malloc(i_size, 4);
}

void __cdecl free(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return _aligned_free(i_ptr);
}

void * operator new(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new %zu\n", i_size);
	return _aligned_malloc(i_size, 4);
}

void operator delete(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return _aligned_free(i_ptr);
}

void * operator new[](size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new [] %zu\n", i_size);
	return _aligned_malloc(i_size, 4);
}

void operator delete [](void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return _aligned_free(i_ptr);
}

Allocators::~Allocators()
{
	delete FreeAddressList;
}

void* Allocators::alloc(size_t size)
{
	if (size > DataSize)
	{
		return nullptr;
	}
	size_t index = 0;
	if (!FreeAddressList->FindFirstSetBit(index))
	{
		return nullptr;
	}
	FreeAddressList->clearBit(index);
	assert(reinterpret_cast<void*>(start + index * DataSize));
	allocations++;
	return reinterpret_cast<void*>(start + index * DataSize);
}

bool Allocators::free(void* ptr)
{
	//assert(ptr);
	ptrdiff_t diff = reinterpret_cast<uintptr_t>(ptr) - start;
	if (diff > 0 && diff % DataSize == 0 && (diff / DataSize) < Numberofblocks)
	{
		ptrdiff_t difference = reinterpret_cast<uintptr_t>(ptr) - start;
		FreeAddressList->setBit(difference / DataSize);
		return true;
	}
	return false;
}

void Allocators::destroy() {
	FreeAddressList->clearAll();
}

void Allocators::showOutstandingBlocks() const
{
}

void Allocators::showFreeBlocks() const
{
}
