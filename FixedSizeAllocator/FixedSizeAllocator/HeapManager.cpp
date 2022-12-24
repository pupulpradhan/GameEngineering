#include <iostream>
#include <Windows.h>
#include <cassert>
#include "HeapManagerProxy.h"
#include "HeapManager.h"

using namespace HeapManagerProxy;
using namespace std;

//void* pHeapMemory;
MemoryBlock* FreeMemoryList;
size_t i_BlocksMemoryBytes = 1024 * 10;

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

	pHeapManager->freeList = FreeMemoryList;
	FreeMemoryList = FreeMemoryList->nextMemBlock;
	pHeapManager->freeList->baseadd = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pHeapMemory) + hSize + i_BlocksMemoryBytes);
	pHeapManager->freeList->blocksize = sizeHeap - i_BlocksMemoryBytes - hSize;
	pHeapManager->freeList->nextMemBlock = nullptr;

	pHeapManager->outstandingAllocations = nullptr;

	pHeapManager->tail = FreeMemoryList;
	FreeMemoryList = FreeMemoryList->nextMemBlock;
	pHeapManager->tail->blocksize = 0;
	pHeapManager->tail->baseadd = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pHeapMemory) + sizeHeap);
	pHeapManager->tail->nextMemBlock = nullptr;

	return pHeapManager;
}

MemoryBlock* TrackAllocation(MemoryBlock* outstandingAllocations, MemoryBlock* pBlock) {

	pBlock->nextMemBlock = outstandingAllocations;
	outstandingAllocations = pBlock;
	return outstandingAllocations;
}

MemoryBlock* ShrinkFreeList(MemoryBlock* freeList, MemoryBlock* pFreeBlock, size_t sizeAlloc) {
	MemoryBlock* temp = freeList;
	MemoryBlock* prev = freeList;
	while (temp != pFreeBlock) {// if pPtr is somewhere in the middle
		if (temp->nextMemBlock == NULL) {
			//break;
			cout << "Memeory Block not found in freeList." << "\n";
			return freeList;
		}
		prev = temp;
		temp = temp->nextMemBlock;
	}
	if (pFreeBlock->blocksize == sizeAlloc) {
		prev->nextMemBlock = temp->nextMemBlock;
		return freeList;
	}
	temp->baseadd = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pFreeBlock->baseadd) + sizeAlloc);
	temp->blocksize = pFreeBlock->blocksize - sizeAlloc;
	return freeList;
}

void HeapManager::FSAInitialize(size_t size, size_t numblock)
{
	Allocators* allocate = new Allocators(size, numblock);
	allocate->start = reinterpret_cast<uintptr_t>(_alloc(size * numblock));
	if (allocatorindex < 3)
	{
		allocators[allocatorindex] = allocate;
		allocatorindex++;
	}
}

void* HeapManager::_alloc(size_t sizeAlloc, const unsigned int alignment) {

	for (size_t i = 0; i < allocatorindex; i++) {
		void* ptr = allocators[i]->alloc(sizeAlloc);
		if (ptr != nullptr) return ptr;
	}

	if (FreeMemoryList == NULL || FreeMemoryList->nextMemBlock == NULL) {
		cout << "FreeMemoryList Exhausted! "<<"\n";
		return nullptr;
	}
	if (this->freeList == NULL) {
		return nullptr;
	}
	
	MemoryBlock* pBlock = FreeMemoryList;
	FreeMemoryList = FreeMemoryList->nextMemBlock;
	MemoryBlock* pFreeBlock = this->freeList;
	MemoryBlock* prev = NULL;
	if (pFreeBlock->baseadd != NULL && pFreeBlock->blocksize >= sizeAlloc) { //1st block matches size
		
		pBlock->baseadd = pFreeBlock->baseadd;
		pBlock->blocksize = sizeAlloc;
		pBlock->nextMemBlock = this->outstandingAllocations;
		this->outstandingAllocations = pBlock;
		this->freeList = ShrinkFreeList(this->freeList, pFreeBlock, sizeAlloc);
		
		return pBlock->baseadd;
	}
	while (pFreeBlock->baseadd != NULL && pFreeBlock->nextMemBlock != NULL && pFreeBlock->blocksize < sizeAlloc) { // matching block is somewhere in between
		prev = pFreeBlock;
		pFreeBlock = pFreeBlock->nextMemBlock;
	}
	if (pFreeBlock == NULL || pFreeBlock->baseadd == NULL) {
		cout << "No matching blocksize found";
		return nullptr;
	}
	
	//check if no block is big enough
	pBlock->baseadd = pFreeBlock->baseadd;
	pBlock->blocksize = sizeAlloc;
	pBlock->nextMemBlock = this->outstandingAllocations;
	this->outstandingAllocations = pBlock;

	//shrink free block
	this->freeList = ShrinkFreeList(this->freeList, pFreeBlock, sizeAlloc);
	//return unused free block to freememorylist
	MemoryBlock* temp = FreeMemoryList;
	if (pFreeBlock->blocksize == sizeAlloc) {
		while (temp->nextMemBlock != NULL) {
			temp = temp->nextMemBlock;
		}
		temp->nextMemBlock = pFreeBlock;
		temp->nextMemBlock->baseadd = nullptr;
		temp->nextMemBlock->blocksize = 0;
		temp->nextMemBlock->nextMemBlock = nullptr;
	}

	return pBlock->baseadd;
}

void* HeapManager::_alloc(size_t sizeAlloc) {
	if (FreeMemoryList == NULL || FreeMemoryList->nextMemBlock == NULL) {
		cout << "FreeMemoryList Exhausted! " << "\n";
		return nullptr;
	}
	if (this->freeList == NULL) {
		return nullptr;
	}
	MemoryBlock* pBlock = FreeMemoryList;
	FreeMemoryList = FreeMemoryList->nextMemBlock;
	MemoryBlock* pFreeBlock = this->freeList;
	MemoryBlock* prev = NULL;

	if (pFreeBlock->baseadd != NULL && pFreeBlock->blocksize >= sizeAlloc) { //1st block matches size

		pBlock->baseadd = pFreeBlock->baseadd;
		pBlock->blocksize = sizeAlloc;
		pBlock->nextMemBlock = this->outstandingAllocations;
		this->outstandingAllocations = pBlock;
		this->freeList = ShrinkFreeList(this->freeList, pFreeBlock, sizeAlloc);

		return pBlock->baseadd;
	}
	while (pFreeBlock->baseadd != NULL && pFreeBlock->nextMemBlock != NULL && pFreeBlock->blocksize < sizeAlloc) { // matching block is somewhere in between
		prev = pFreeBlock;
		pFreeBlock = pFreeBlock->nextMemBlock;
	}
	if (pFreeBlock == NULL || pFreeBlock->baseadd == NULL) {
		cout << "No matching blocksize found";
		return nullptr;
	}

	//check if no block is big enough
	pBlock->baseadd = pFreeBlock->baseadd;
	pBlock->blocksize = sizeAlloc;
	pBlock->nextMemBlock = this->outstandingAllocations;
	this->outstandingAllocations = pBlock;

	//shrink free block
	this->freeList = ShrinkFreeList(this->freeList, pFreeBlock, sizeAlloc);
	//return unused free block to freememorylist
	MemoryBlock* temp = FreeMemoryList;
	if (pFreeBlock->blocksize == sizeAlloc) {
		while (temp->nextMemBlock != NULL) {
			temp = temp->nextMemBlock;
		}
		temp->nextMemBlock = pFreeBlock;
		temp->nextMemBlock->baseadd = nullptr;
		temp->nextMemBlock->blocksize = 0;
		temp->nextMemBlock->nextMemBlock = nullptr;
	}

	return pBlock->baseadd;
}

MemoryBlock* RetrieveFromOutstandingAllocation(MemoryBlock* outstandingAllocations, void* pPtr) {
	MemoryBlock* freeBlock;
	MemoryBlock* temp = outstandingAllocations;
	MemoryBlock* prev = outstandingAllocations;
	if (temp != NULL) {
		if (temp->baseadd != NULL && temp->baseadd == pPtr) {// if pPtr is the 1st elem
			freeBlock = temp;
			freeBlock->baseadd = pPtr;
			freeBlock->blocksize = temp->blocksize;
			return freeBlock;
		}
		while (temp->baseadd != pPtr) {// if pPtr is somewhere in the middle
			if (temp->nextMemBlock == NULL) {
				break;
				/*cout << "Memory Block not found in outstanding allocations list. " << "\n";
				return NULL;*/
			}
			prev = temp;
			temp = temp->nextMemBlock;
		}
		prev->nextMemBlock = temp->nextMemBlock;
		freeBlock = temp;
		freeBlock->baseadd = pPtr;
		freeBlock->blocksize = temp->blocksize;
		return freeBlock;
	}
	return NULL;
}

MemoryBlock*  ShrinkOutstandingAllocations(MemoryBlock* outstandingAllocations, void* pPtr) {
	MemoryBlock* temp = outstandingAllocations;
	MemoryBlock* prev = outstandingAllocations;
	if (temp != NULL && temp->baseadd != NULL && temp->baseadd == pPtr) {// if pPtr is the 1st elem
		outstandingAllocations = temp->nextMemBlock;
		return outstandingAllocations;
	}
	while (temp->baseadd != pPtr) {// if pPtr is somewhere in the middle
		if (temp->nextMemBlock == NULL) {
			break;
			/*cout << "pPtr not found in outstanding allocations list. " << "\n";
			return outstandingAllocations;*/
		}
		prev = temp;
		temp = temp->nextMemBlock;
	}
	prev->nextMemBlock = temp->nextMemBlock;
	return outstandingAllocations;
}

bool HeapManager::_free(void* pPtr) {
	MemoryBlock* pBlock = RetrieveFromOutstandingAllocation(this->outstandingAllocations, pPtr);
	if (pBlock == NULL) {
		//cout << "OutstandingList is empty. "<<"\n";
		return true;
	}
	this->outstandingAllocations = ShrinkOutstandingAllocations(this->outstandingAllocations, pPtr);
	assert(pBlock);
	/*pBlock->nextMemBlock = this->freeList;
	this->freeList = pBlock;*/

	//put the block on the Freelist
	MemoryBlock* temp = this->freeList;
	MemoryBlock* prev = this->freeList;
	if (pBlock->baseadd < temp->baseadd) {
		pBlock->nextMemBlock = this->freeList;
		this->freeList = pBlock;
	}
	else {
		while (temp != NULL && pBlock->baseadd >= temp->baseadd) {
			prev = temp;
			temp = temp->nextMemBlock;
		}
		pBlock->nextMemBlock = temp;
		prev->nextMemBlock = pBlock;
	}
	return true;
}

void HeapManager::collect() {
	MemoryBlock* temp = FreeMemoryList;
	MemoryBlock* tempFreelist = freeList;
	while (tempFreelist != NULL && tempFreelist->nextMemBlock != NULL) {
		if ((reinterpret_cast<uintptr_t>(tempFreelist->baseadd) + tempFreelist->blocksize) == reinterpret_cast<uintptr_t>(tempFreelist->nextMemBlock->baseadd)) {
			while (temp->nextMemBlock != NULL) {
				temp = temp->nextMemBlock;
			}
			tempFreelist->blocksize += tempFreelist->nextMemBlock->blocksize;
			temp->nextMemBlock = tempFreelist->nextMemBlock;
			tempFreelist->nextMemBlock = tempFreelist->nextMemBlock->nextMemBlock;
			temp->nextMemBlock->baseadd = nullptr;
			temp->nextMemBlock->blocksize = 0;
			temp->nextMemBlock->nextMemBlock = nullptr;
			
		}
		tempFreelist = tempFreelist->nextMemBlock;
	}
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
	size_t hSize = sizeof(HeapManager);
	void* start = static_cast<char*>(this->memBlockAdd) - hSize;
	size_t totalSize = this->size + hSize;
	void* end = reinterpret_cast<void*>(static_cast<char*>(start) + totalSize);
	MemoryBlock* temp = this->freeList;
	while (temp != nullptr && temp != tail) {
		cout << "Free Block " << i << " : " << temp->baseadd << " with size "<< temp->blocksize<<endl;
		if (temp->nextMemBlock == NULL) {
			break;
		}
		temp = temp->nextMemBlock;
		i++;
	}
}
void HeapManager::ShowOutstandingAllocations() {
	int i = 0;
	MemoryBlock* temp = this->outstandingAllocations;
	while (temp != nullptr) {
		cout << "Outstanding Allocation Block " << i << " : " << temp->baseadd << " with size " << temp->blocksize << endl;
		if (temp->nextMemBlock == NULL) {
			break;
		}
		temp = temp->nextMemBlock;
		i++;
	}
}
void HeapManager::destroy() {
}


