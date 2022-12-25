#include <inttypes.h>
#include "Allocators.h"

#include <stdio.h>

Allocators::~Allocators()
{
	delete FreeList;
}

void* Allocators::alloc(size_t size)
{
	if (size > DataSize)
	{
		return nullptr;
	}
	size_t index = 0;
	if (!FreeList->FindFirstSetBit(index))
	{
		return nullptr;
	}
	FreeList->clearBit(index);
	assert(reinterpret_cast<void*>(start + index * DataSize));
	allocations++;
	printf("malloc %zu\n", size);
	return reinterpret_cast<void*>(start + index * DataSize);
}

bool Allocators::free(void* ptr)
{

	ptrdiff_t diff = reinterpret_cast<uintptr_t>(ptr) - start;
	if (diff > 0 && diff % DataSize == 0 && (diff / DataSize) < Numberofblocks)
	{
		ptrdiff_t difference = reinterpret_cast<uintptr_t>(ptr) - start;
		FreeList->setBit(difference / DataSize);
		printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(ptr));
		return true;
	}
	return false;
}

void Allocators::destroy() {
	FreeList->clearAll();
}

void Allocators::showOutstandingBlocks() const
{
}

void Allocators::showFreeBlocks() const
{
}
