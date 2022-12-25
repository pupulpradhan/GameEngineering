#include "MemorySystem.h"

using namespace HeapManagerProxy;

HeapManager* InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	auto heapmanager = CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
	heapmanager->InitializeFixedSizedAllocator(16, 100);
	heapmanager->InitializeFixedSizedAllocator(32, 200);
	heapmanager->InitializeFixedSizedAllocator(96, 400);
	return heapmanager;
	// create your HeapManager and FixedSizeAllocators
	//return true;
}

void Collect(HeapManager* heap)
{
	// coalesce free blocks
	// you may or may not need to do this depending on how you've implemented your HeapManager
	//HeapManager* i_pManager;
	heap->collect();
}

void DestroyMemorySystem(HeapManager* heap)
{
	// Destroy your HeapManager and FixedSizeAllocators
	heap->destroy();
}

