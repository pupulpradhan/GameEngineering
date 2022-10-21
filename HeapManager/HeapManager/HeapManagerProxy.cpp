#include <iostream>
#include <Windows.h>
#include <cassert>
#include "HeapManagerProxy.h"


void* pHeapMemory;
MemoryBlock* FreeMemoryList;
MemoryBlock* pFreeList = nullptr;
MemoryBlock* FreeList = nullptr;
MemoryBlock* OutstandingAllocations = nullptr;
size_t i_BlocksMemoryBytes = 1024 * 4;

HeapManager* CreateHeapManager(void* pHeapMemory, const size_t sizeHeap, const unsigned int numDescriptors) {
	HeapManager* pHeapManager;
	pHeapManager->heapMemory = static_cast<HeapManager*>(pHeapMemory);
	pHeapManager->size = sizeHeap;
	pHeapManager->descriptors = numDescriptors;

	MemoryBlock* FreeMemoryList = InitializeMemoryBlocks(pHeapMemory, i_BlocksMemoryBytes);

	FreeList->baseadd = pHeapMemory + i_BlocksMemoryBytes;
	FreeList->blocksize -= i_BlocksMemoryBytes;
	FreeList->nextMemBlock = nullptr;
	pHeapManager->freeList = FreeList;

	OutstandingAllocations->baseadd = nullptr;
	OutstandingAllocations->blocksize = 0;
	OutstandingAllocations->nextMemBlock = nullptr;
	pHeapManager->outstandingAllocations = OutstandingAllocations;

	return pHeapManager;
}

MemoryBlock* InitializeMemoryBlocks(void* pHeapMemory, size_t i_BlocksMemoryBytes)
{
	assert((pHeapMemory != nullptr) && (i_BlocksMemoryBytes > sizeof(MemoryBlock)));

	pFreeList = reinterpret_cast<MemoryBlock*>(pHeapMemory);
	const size_t NumberofBlocks = i_BlocksMemoryBytes / sizeof(MemoryBlock);

	MemoryBlock * pCurrentBlock = pFreeList;
	for (size_t i = 0; i < (NumberofBlocks - 1); i++, pCurrentBlock++)
	{
		pCurrentBlock->baseadd = nullptr;
		pCurrentBlock->blocksize = 0;
		pCurrentBlock->nextMemBlock = pCurrentBlock + 1;
	}//end. last block
	pCurrentBlock->baseadd = nullptr;
	pCurrentBlock->blocksize = 0;
	pCurrentBlock->nextMemBlock = nullptr;
	return pCurrentBlock;
}

void TrackAllocation(HeapManager* pHeapManager, MemoryBlock* pBlock) {
	
	OutstandingAllocations->nextMemBlock = OutstandingAllocations;
	OutstandingAllocations->baseadd = pBlock->baseadd;
	OutstandingAllocations->blocksize = pBlock->blocksize;
	pHeapManager->outstandingAllocations = OutstandingAllocations;
}

void* HeapManager::alloc(HeapManager* pHeapManager, size_t sizeAlloc, const unsigned int alignment){

	MemoryBlock* pBlock = FreeMemoryList;
	MemoryBlock* pFreeBlock = pHeapManager->freeList;
	while (pFreeBlock)
	{
		if (pFreeBlock->blocksize > sizeAlloc)
			break;
		pFreeBlock = pFreeBlock->nextMemBlock;
	}//no block big enough
	assert(pFreeBlock);
	pBlock->baseadd = pFreeBlock->baseadd;
	pBlock->blocksize = sizeAlloc;
	TrackAllocation(pHeapManager, pBlock);

	//shrink block
	pFreeBlock += sizeAlloc;
	pFreeBlock->blocksize -= sizeAlloc;
	return pBlock->baseadd;
}
void HeapManager::Collect(HeapManager* pHeapManager) {
}
bool HeapManager::Contains(HeapManager* pHeapManager, void* pPtr) {
}

MemoryBlock* RemoveOutstandingAllocation(HeapManager* pHeapManager, void* pPtr) {
	MemoryBlock* freeBlock;
	if (pPtr = pHeapManager->outstandingAllocations) {
		/*FreeList->nextMemBlock = FreeList;
		FreeList->baseadd = i_ptr;
		FreeList->blocksize = OutstandingAllocations->blocksize;*/
		freeBlock = static_cast<MemoryBlock*>(pPtr);
		freeBlock->blocksize = pHeapManager->outstandingAllocations->blocksize;
		pHeapManager->outstandingAllocations += pHeapManager->outstandingAllocations->blocksize;
	}
	return freeBlock;
}

bool HeapManager::free(HeapManager* pHeapManager, void* pPtr) {
	//remove block for this pointer from OutstandingAllocations
	MemoryBlock* pBlock = RemoveOutstandingAllocation(pHeapManager, pPtr);
	assert(pBlock);
	//put the block on the Freelist
	pBlock->nextMemBlock = FreeList;
	FreeList = pBlock;
}
bool HeapManager::IsAllocated(HeapManager* pHeapManager, void* pPtr) {
}
void HeapManager::ShowFreeBlocks(HeapManager* pHeapManager) {
}
void HeapManager::ShowOutstandingAllocations(HeapManager* pHeapManager) {
}
void HeapManager::Destroy(HeapManager* pHeapManager) {
}

MemoryBlock* GetMemoryBlock()
{
	assert(pFreeList != nullptr);
	MemoryBlock* pReturnBlock = pFreeList;
	pFreeList = pFreeList->nextMemBlock;
	return pReturnBlock;
}

void ReturnMemoryBlock(MemoryBlock* i_pFreeBlock)
{
	assert(i_pFreeBlock != nullptr);
	i_pFreeBlock->baseadd = nullptr;
	i_pFreeBlock->blocksize = 0;
	i_pFreeBlock->nextMemBlock = pFreeList;
	pFreeList = i_pFreeBlock;
}


