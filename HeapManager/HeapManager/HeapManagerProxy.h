#pragma once

struct MemoryBlock {
	void* baseadd;
	size_t blocksize;
	struct MemoryBlock* nextMemBlock;
};

class HeapManager {
public:
	HeapManager* heapMemory;
	MemoryBlock* freeList;
	MemoryBlock* outstandingAllocations;
	size_t size;
	unsigned int descriptors;

	void* alloc(HeapManager* pHeapManager, size_t sizeAlloc, const unsigned int alignment);
	void Collect(HeapManager* pHeapManager);
	bool Contains(HeapManager* pHeapManager, void* pPtr);
	bool free(HeapManager* pHeapManager, void* pPtr);
	bool IsAllocated(HeapManager* pHeapManager, void* pPtr);
	void ShowFreeBlocks(HeapManager* pHeapManager);
	void ShowOutstandingAllocations(HeapManager* pHeapManager);
	void Destroy(HeapManager* pHeapManager);
};
HeapManager* CreateHeapManager(void* pHeapMemory, const size_t sizeHeap, const unsigned int numDescriptors);

