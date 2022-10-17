#include <iostream>
#include <Windows.h>


struct MemoryBlock {
	*void baseadd;
	size_t blocksize;
	struct MemoryBlock* nextMemBlock;
};

void* CreateHeapManager(size_t size) {
	void* add = TheHeap.baseadd;
	TheHeap.baseadd += size;
	Theheap.blocksize -= size;

	return add;
}
void* HeapAlloc() {
}

void* malloc(size_t i_size)
{
	MemoryBlock* pBlock = GetFreeMemoryBlock();
	MemoryBlock* pFreeBlock = FreeList;
	while (pFreeBlock)
	{
		if (pFreeBlock->BlockSize > i_size)
			break;
		pFreeBlock = pFreeBlock->pNextBlock;
	}//no block big enough
	assert(pFreeBlock);
	pBlock->pBaseAddress = pFreeBlock->pBaseAddress;
	pBlock->BlockSize = i_size;
	TrackAllocation(pBlock);

	//shrink block
	pFreeBlock.pBaseAddress += i_size;
	pFreeBlock.BlockSize -= i_size;
	return pBlock->pBaseAddress;

}

struct MemoryBlock* pFreeList = nullptr;

void InitializeMemoryBlocks(void* i_pBlocksMemory, size_t i_BlocksMemoryBytes)
{
	assert((i_pBlocksMemory != nullptr) && (i_BlocksMemorySize > sizeof(MemoryBlock));

	pFreeList = reinterpret_cast<MemoryBlock*>(i_pBlocksMemory);
	const size_t NumberofBlocks = i_BlocksMemorySize / sizeof(MemoryBlock);

	MemoryBlock * pCurrentBlock = pFreeList;
	for (size_t i = 0; i < (NumberOfBlocks - 1); i++, pCurrentBlock++)
	{
		pCurrentBlock->pBaseAddress = nullptr;
		pCurrentBlock->BlockSize = 0;
		pCurrentBlock->pNextBlock = pCurrentBlock + 1;
	}//end. last block
	pCurrentBlock->pBaseAddress = nullptr;
	pCurrentBlock->BlockSize = 0;
	pCurrentBlock->pNextBlock = nullptr;
}

MemoryBlock* GetMemoryBlock()
{
	assert(pFreeList != nullptr);
	MemoryBlock* pReturnBlock = pFreeList;
	pFreeList = pFreeList->pNextBlock;
	return pReturnBlock;
}
void ReturnMemoryBlock(MemoryBlock* i_pFreeBlock)
{
	assert(i_pFreeBlock != nullptr);
	i_pFreeBlock->pBaseAddress = nullptr;
	i_pFreeBlock->BlockSize = 0;
	i_pFreeBlock->pNextBlock = pFreeList;
	pFreeList = i_pFreeBlock;
}


struct MemoryBlock* FreeList = TheHeap;
struct MemoryBlock* OutstandingAllocations = nullptr;
void* free(void* i_ptr)
{
	//remove block for this pointer from OutstandingAllocations
	MemoryBlock* pBlock = RemoveOutstandingAllocation(i_ptr);
	assert(pBlock);
	//put the block on the Freelist
	pBlock->pNextBlock = FreeList;
	FreeList = pBlock;
}


void main() {
	const size_t 		sizeHeap = 1024 * 1024;
	const unsigned int 	numDescriptors = 2048;
	// Get SYSTEM_INFO, which includes the memory page size
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	// round our size to a multiple of memory page size
	assert(SysInfo.dwPageSize > 0);
	size_t sizeHeapInPageMultiples = SysInfo.dwPageSize * ((sizeHeap + SysInfo.dwPageSize) / SysInfo.dwPageSize);
	void* pHeapMemory = VirtualAlloc(NULL, sizeHeapInPageMultiples, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}