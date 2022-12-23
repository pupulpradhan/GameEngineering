#pragma once
#define SUPPORTS_ALIGNMENT
#define SUPPORTS_SHOWFREEBLOCKS
#include "Allocators.h"

struct MemoryBlock {
	void* baseadd;
	size_t blocksize;
	struct MemoryBlock* nextMemBlock;
};

class HeapManager {
private:
	void* memBlockAdd;
	MemoryBlock* freeList;
	MemoryBlock* outstandingAllocations;
	MemoryBlock* tail;
	size_t size;
	//unsigned int descriptors;
public:
	HeapManager* create(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors); //done
	Allocators* allocators[3];
	int allocatorindex = 0;
	void destroy();
	void* _alloc(size_t i_size); // done
	void* _alloc(size_t i_size, unsigned int i_alignment); //done
	bool _free(void* i_ptr); // done
	void FSAInitialize(size_t size, size_t numblock);
	void collect();
	bool Contains(void* i_ptr);
	bool IsAllocated(void* i_ptr);
	size_t getLargestFreeBlock();
	size_t getTotalFreeMemory();
	void ShowFreeBlocks(); //done
	void ShowOutstandingAllocations(); //done
};