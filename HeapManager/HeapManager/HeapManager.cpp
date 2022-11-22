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
	size_t hSize = sizeof(HeapManager);
	pHeapManager->memBlockAdd = static_cast<char*>(pHeapMemory) + hSize;
	pHeapManager->size = sizeHeap - hSize;

	FreeMemoryList = InitializeMemoryBlocks(static_cast<char*>(pHeapMemory) + hSize, i_BlocksMemoryBytes);

	MemoryBlock* FreeList = reinterpret_cast<MemoryBlock*>(reinterpret_cast<uintptr_t>(pHeapMemory) + hSize + i_BlocksMemoryBytes);
	FreeList->baseadd = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pHeapMemory) + hSize + i_BlocksMemoryBytes);
	FreeList->blocksize = sizeHeap - i_BlocksMemoryBytes - hSize;
	FreeList->nextMemBlock = nullptr;
	pHeapManager->freeList = FreeList;

	MemoryBlock* OutstandingAllocations = nullptr;
	/*OutstandingAllocations->baseadd = nullptr;
	OutstandingAllocations->blocksize = 0;
	OutstandingAllocations->nextMemBlock = nullptr;*/
	pHeapManager->outstandingAllocations = OutstandingAllocations;

	return pHeapManager;
}

MemoryBlock* TrackAllocation(MemoryBlock* outstandingAllocations, MemoryBlock* pBlock) {

	pBlock->nextMemBlock = outstandingAllocations;
	outstandingAllocations = pBlock;
	return outstandingAllocations;
}

void* HeapManager::_alloc(size_t sizeAlloc, const unsigned int alignment) {
	MemoryBlock* pBlock = FreeMemoryList;
	MemoryBlock* pFreeBlock = this->freeList;
	MemoryBlock* prev = NULL;

	if (pFreeBlock->baseadd != NULL && pFreeBlock->blocksize > sizeAlloc) {
		FreeMemoryList = FreeMemoryList->nextMemBlock;
		pBlock->baseadd = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pFreeBlock->baseadd) + sizeof(MemoryBlock));
		pBlock->blocksize = sizeAlloc + sizeof(MemoryBlock);
		pBlock->nextMemBlock = this->outstandingAllocations;
		this->outstandingAllocations = pBlock;
		//TrackAllocation(this->outstandingAllocations, pBlock);
		this->freeList = reinterpret_cast<MemoryBlock*>(reinterpret_cast<uintptr_t>(this->freeList) + sizeAlloc);
		this->freeList->baseadd = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this->freeList) + sizeAlloc);
		this->freeList->blocksize -= sizeAlloc;
		
		return pBlock->baseadd;
	}
	while (pFreeBlock && pFreeBlock->nextMemBlock != NULL && pFreeBlock->blocksize <= sizeAlloc) {
		prev = pFreeBlock;
		pFreeBlock = pFreeBlock->nextMemBlock;
	}

	//check if no block is big enough
	assert(pFreeBlock);
	FreeMemoryList = FreeMemoryList->nextMemBlock;
	pBlock->baseadd = pFreeBlock->baseadd;
	pBlock->blocksize = sizeAlloc;
	TrackAllocation(outstandingAllocations, pBlock);

	//shrink block
	//prev->nextMemBlock = pFreeBlock->nextMemBlock;
	this->freeList += sizeAlloc;
	this->freeList->blocksize -= sizeAlloc;
	
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

MemoryBlock* RemoveOutstandingAllocation(MemoryBlock* outstandingAllocations, void* pPtr) {
	MemoryBlock* freeBlock;
	MemoryBlock* temp = outstandingAllocations;
	MemoryBlock* prev = outstandingAllocations;
	if (temp != NULL && temp->baseadd != NULL && temp->baseadd == pPtr) {// if pPtr is the 1st elem
		outstandingAllocations = temp->nextMemBlock;
		freeBlock = static_cast<MemoryBlock*>(pPtr);
		freeBlock->baseadd = pPtr;
		freeBlock->blocksize = temp->blocksize;
		return freeBlock;
	}
	while (temp->baseadd != pPtr) {
		if (temp->nextMemBlock == NULL) {
			break;
		}
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
	//MemoryBlock** refOutstandingAllocations = &outstandingAllocations;
	MemoryBlock* pBlock = RemoveOutstandingAllocation(this->outstandingAllocations, pPtr);
	
	assert(pBlock);
	//put the block on the Freelist

	pBlock->nextMemBlock = this->freeList;
	this->freeList = pBlock;
	return true;
}

void HeapManager::collect() {
}

bool HeapManager::Contains(void* pPtr) {
	/*if (pPtr >= this->memBlockAdd && pPtr <= this->memBlockAdd + this->size) {
		return true;
	}
	return false;*/

	/*MemoryBlock* temp1 = outstandingAllocations;
	while (temp1->nextMemBlock != NULL) {
		if (temp1->baseadd == pPtr) {
			return true;
		}
		temp1 = temp1->nextMemBlock;
	}
	MemoryBlock* temp2 = freeList;
	while (temp2->nextMemBlock != NULL) {
		if (temp2->baseadd == pPtr) {
			return true;
		}
		temp2 = temp2->nextMemBlock;
	}
	return false;*/
	return true;
}

bool HeapManager::IsAllocated(void* pPtr) {
	/*MemoryBlock* temp = outstandingAllocations;
	while (temp->nextMemBlock!= NULL){
		if (temp->baseadd == pPtr) return true;
		temp = temp->nextMemBlock;
	}
	return false;*/
	return true;

}
size_t HeapManager::getLargestFreeBlock()
{
	size_t temp = 0;
	while (freeList != NULL) {
		if(temp <= freeList->blocksize)
			temp = freeList->blocksize;
		freeList = freeList->nextMemBlock;
	}
	return temp;
}
size_t HeapManager::getTotalFreeMemory()
{

	size_t temp = 0;
	while (freeList != NULL) {
		temp += freeList->blocksize;
		freeList = freeList->nextMemBlock;
	}
	return temp;
}
void HeapManager::ShowFreeBlocks(){
	int i = 0;
	while (freeList != nullptr) {
		cout << "Free Block " << i << " : " << freeList->baseadd << " with size "<< freeList->blocksize<<endl;
		freeList = freeList->nextMemBlock;
		i++;
	}
}
void HeapManager::ShowOutstandingAllocations() {
	int i = 0;
	while (outstandingAllocations != nullptr) {
		cout << "Free Block " << i << " : " << outstandingAllocations->baseadd << "with size " << outstandingAllocations->blocksize << endl;
		outstandingAllocations = outstandingAllocations->nextMemBlock;
		i++;
	}
}
void HeapManager::destroy() {
}


