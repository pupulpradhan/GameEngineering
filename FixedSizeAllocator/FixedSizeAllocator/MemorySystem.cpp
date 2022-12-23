#include "MemorySystem.h"
#include "HeapManagerProxy.h"

using namespace HeapManagerProxy;

bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	auto hm = CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
	hm->FSAInitialize(16, 100);
	hm->FSAInitialize(32, 200);
	hm->FSAInitialize(96, 400);
	return hm;
	CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
	// create your HeapManager and FixedSizeAllocators
	return true;
}

void Collect()
{
	// coalesce free blocks
	// you may or may not need to do this depending on how you've implemented your HeapManager
	//HeapManager* i_pManager;
	Collect();
}

void DestroyMemorySystem()
{
	// Destroy your HeapManager and FixedSizeAllocators
}

