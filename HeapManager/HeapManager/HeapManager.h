#pragma once

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
	HeapManager* create(void* i_pMemory, const  size_t i_sizeMemory, const unsigned int i_numDescriptors);
	void destroy();
	void* _alloc(const size_t i_size);
	void* _alloc(const size_t i_size, const unsigned int i_alignment);
	bool _free(void* i_ptr);
	void collect();
	bool Contains(void* i_ptr) const;
	bool IsAllocated(void* i_ptr) const;
	size_t getLargestFreeBlock() const;
	size_t getTotalFreeMemory() const;
	void ShowFreeBlocks() const;
	void ShowOutstandingAllocations() const;
};