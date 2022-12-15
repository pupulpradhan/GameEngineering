#pragma once
#include <stdint.h>

class HeapManager;

namespace HeapManagerProxy
{

	HeapManager* CreateHeapManager(void* i_pMemory, const size_t i_sizeMemory, const unsigned int i_numDescriptors);
	void			Destroy(HeapManager* i_pManager);

	void* alloc(HeapManager* i_pManager, const size_t i_size);
	void* alloc(HeapManager* i_pManager, const size_t i_size, const unsigned int i_alignment);
	bool			free(HeapManager* i_pManager, void* i_ptr);

	void			Collect(HeapManager* i_pManager);

	bool			Contains(const HeapManager* i_pManager, void* i_ptr);
	bool			IsAllocated(const HeapManager* i_pManager, void* i_ptr);

	size_t			GetLargestFreeBlock(const HeapManager* i_pManager);
	size_t			GetTotalFreeMemory(const HeapManager* i_pManager);

	void			ShowFreeBlocks(const HeapManager* i_pManager);
	void			ShowOutstandingAllocations(const HeapManager* i_pManager);

} // namespace HeapManagerProxy
