#include <iostream>
#include <Windows.h>
#include <cassert>
#include "HeapManagerProxy.h"
#include "HeapManager.h"

using namespace HeapManagerProxy;
using namespace std;

//void* pHeapMemory;
MemoryBlock* FreeMemoryList;
size_t i_BlocksMemoryBytes = 1024 * 4;

MemoryBlock* InitializeMemoryBlocks(void* pHeapMemory, size_t i_BlocksMemoryBytes)
{
	assert((pHeapMemory != nullptr) && (i_BlocksMemoryBytes > sizeof(MemoryBlock)));

	FreeMemoryList = reinterpret_cast<MemoryBlock*>(pHeapMemory);
	/*pFreeList->baseadd = pHeapMemory;
	pFreeList->blocksize = i_BlocksMemoryBytes;*/

	const size_t NumberofBlocks = i_BlocksMemoryBytes / sizeof(MemoryBlock);

	MemoryBlock* pCurrentBlock = FreeMemoryList;
	for (size_t i = 0; i < (NumberofBlocks - 1); i++, pCurrentBlock++)
	{
		pCurrentBlock->baseadd = nullptr;
		pCurrentBlock->blocksize = 0;
		pCurrentBlock->nextMemBlock = pCurrentBlock + 1;
	}//last block
	pCurrentBlock->baseadd = nullptr;
	pCurrentBlock->blocksize = 0;
	pCurrentBlock->nextMemBlock = nullptr;
	return FreeMemoryList;
}

HeapManager* HeapManager::create(void* pHeapMemory, const size_t sizeHeap, const unsigned int numDescriptors) {
	HeapManager* pHeapManager = static_cast<HeapManager*>(pHeapMemory);
	//pHeapManager->heapMemory = static_cast<HeapManager*>(pHeapMemory);
	pHeapManager->memBlockAdd = pHeapMemory;

	FreeMemoryList = InitializeMemoryBlocks(pHeapMemory, i_BlocksMemoryBytes);

	MemoryBlock* FreeList = static_cast<MemoryBlock*>(pHeapMemory) + i_BlocksMemoryBytes;
	FreeList->baseadd = static_cast<MemoryBlock*>(pHeapMemory) + i_BlocksMemoryBytes;
	FreeList->blocksize = sizeHeap - i_BlocksMemoryBytes;
	FreeList->nextMemBlock = nullptr;
	pHeapManager->freeList = FreeList;

	MemoryBlock* OutstandingAllocations = nullptr;
	/*OutstandingAllocations->baseadd = nullptr;
	OutstandingAllocations->blocksize = 0;
	OutstandingAllocations->nextMemBlock = nullptr;*/
	pHeapManager->outstandingAllocations = OutstandingAllocations;

	return pHeapManager;
}

void TrackAllocation(MemoryBlock* outstandingAllocations, MemoryBlock* pBlock) {

	pBlock->nextMemBlock = outstandingAllocations;
	outstandingAllocations = pBlock;
}

void* HeapManager::_alloc(size_t sizeAlloc, const unsigned int alignment) {
	MemoryBlock* pBlock = FreeMemoryList;
	MemoryBlock* pFreeBlock = this->freeList;
	MemoryBlock* prev = NULL;

	if (pFreeBlock->baseadd != NULL && pFreeBlock->blocksize > sizeAlloc) {
		pBlock->baseadd = pFreeBlock->baseadd;
		pBlock->blocksize = sizeAlloc;
		TrackAllocation(outstandingAllocations, pBlock);
		this->freeList += sizeAlloc;
		this->freeList->blocksize -= sizeAlloc;
		FreeMemoryList = FreeMemoryList->nextMemBlock;
		return pBlock->baseadd;
	}
	while (pFreeBlock && pFreeBlock->nextMemBlock != NULL && pFreeBlock->blocksize <= sizeAlloc) {
		prev = pFreeBlock;
		pFreeBlock = pFreeBlock->nextMemBlock;
	}

	//check if no block is big enough
	assert(pFreeBlock);
	pBlock->baseadd = pFreeBlock->baseadd;
	pBlock->blocksize = sizeAlloc;
	TrackAllocation(outstandingAllocations, pBlock);

	//shrink block
	//prev->nextMemBlock = pFreeBlock->nextMemBlock;
	this->freeList += sizeAlloc;
	this->freeList->blocksize -= sizeAlloc;
	FreeMemoryList = FreeMemoryList->nextMemBlock;
	return pBlock->baseadd;
}

void* HeapManager::_alloc(size_t sizeAlloc) {
	MemoryBlock* pBlock = FreeMemoryList;
	MemoryBlock* pFreeBlock = freeList;
	MemoryBlock* prev = NULL;

	if (pFreeBlock->baseadd != NULL && pFreeBlock->blocksize > sizeAlloc) {
		//freeList = pFreeBlock->nextMemBlock;
		pBlock->baseadd = pFreeBlock->baseadd;
		pBlock->blocksize = sizeAlloc;
		TrackAllocation(outstandingAllocations, pBlock);
		freeList += sizeAlloc;
		freeList->blocksize -= sizeAlloc;
		FreeMemoryList = FreeMemoryList->nextMemBlock;
		return pBlock->baseadd;
	}
	while (pFreeBlock && pFreeBlock->nextMemBlock != NULL && pFreeBlock->blocksize <= sizeAlloc) {
		prev = pFreeBlock;
		pFreeBlock = pFreeBlock->nextMemBlock;
	}

//check if no block is big enough
	assert(pFreeBlock);
	pBlock->baseadd = pFreeBlock->baseadd;
	pBlock->blocksize = sizeAlloc;
	TrackAllocation(outstandingAllocations, pBlock);

	//shrink block
	//prev->nextMemBlock = pFreeBlock->nextMemBlock;
	freeList += sizeAlloc;
	freeList->blocksize -= sizeAlloc;
	FreeMemoryList = FreeMemoryList->nextMemBlock;
	return pBlock->baseadd;
}


void HeapManager::collect() {
}
bool HeapManager::Contains(void* pPtr) {
	return true;
}

MemoryBlock* RemoveOutstandingAllocation(MemoryBlock* outstandingAllocations, void* pPtr) {
	MemoryBlock* freeBlock;
	MemoryBlock* temp = outstandingAllocations;
	MemoryBlock* prev = NULL;
	if (temp->baseadd != NULL && temp->baseadd == pPtr) {
		outstandingAllocations = temp->nextMemBlock;
		freeBlock = static_cast<MemoryBlock*>(pPtr);
		freeBlock->baseadd = pPtr;
		freeBlock->blocksize = temp->blocksize;
		return freeBlock;
	}
	while (temp->nextMemBlock != NULL && temp->baseadd != pPtr) {
		prev = temp;
		temp = temp->nextMemBlock;
	}
	prev->nextMemBlock = temp->nextMemBlock;
	freeBlock = static_cast<MemoryBlock*>(pPtr);
	freeBlock->baseadd = pPtr;
	freeBlock->blocksize = temp->blocksize;
	return freeBlock;
}

bool HeapManager::_free(void* pPtr) {
	//remove block for this pointer from OutstandingAllocations
	MemoryBlock* pBlock = RemoveOutstandingAllocation(outstandingAllocations, pPtr);
	assert(pBlock);
	//put the block on the Freelist

	pBlock->nextMemBlock = this->freeList;
	this->freeList = pBlock;
	return true;
}
bool HeapManager::IsAllocated(void* pPtr) {
	MemoryBlock* temp = outstandingAllocations;
	while (temp->nextMemBlock!= NULL){
		if (temp->baseadd == pPtr) return true;
		temp = temp->nextMemBlock;
	}
	return false;
}
size_t HeapManager::getLargestFreeBlock()
{
	return size_t();
}
size_t HeapManager::getTotalFreeMemory()
{
	return size_t();
}
void HeapManager::ShowFreeBlocks(){
	int i = 0;
	while (freeList != nullptr) {
		cout << "Free Block " << i << " : " << freeList->baseadd << "with size "<< freeList->blocksize<<endl;
		i++;
	}
}
void HeapManager::ShowOutstandingAllocations() {
	int i = 0;
	while (outstandingAllocations != nullptr) {
		cout << "Free Block " << i << " : " << outstandingAllocations->baseadd << "with size " << outstandingAllocations->blocksize << endl;
		i++;
	}
}
void HeapManager::destroy() {
}

//MemoryBlock* GetMemoryBlock()
//{
//	assert(pFreeList != nullptr);
//	MemoryBlock* pReturnBlock = pFreeList;
//	pFreeList = pFreeList->nextMemBlock;
//	return pReturnBlock;
//}
//
//void ReturnMemoryBlock(MemoryBlock* i_pFreeBlock)
//{
//	assert(i_pFreeBlock != nullptr);
//	i_pFreeBlock->baseadd = nullptr;
//	i_pFreeBlock->blocksize = 0;
//	i_pFreeBlock->nextMemBlock = pFreeList;
//	pFreeList = i_pFreeBlock;
//}


