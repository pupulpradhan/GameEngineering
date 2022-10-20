#pragma once

struct HeapManager {

};
HeapManager* CreateHeapManager(void* pHeapMemory, const size_t sizeHeap, const unsigned int numDescriptors);
void* alloc(HeapManager* pHeapManager, size_t sizeAlloc, const unsigned int alignment);
void Collect(HeapManager* pHeapManager);
bool Contains(HeapManager* pHeapManager, void* pPtr);
bool free(HeapManager* pHeapManager, void* pPtr);
bool IsAllocated(HeapManager* pHeapManager, void* pPtr);
void ShowFreeBlocks(HeapManager* pHeapManager);
void ShowOutstandingAllocations(HeapManager* pHeapManager);
void Destroy(HeapManager* pHeapManager);

