#pragma once
#include"BitArray.h"
#include <cassert>

class Allocators
{
private:
	BitArray* FreeAddressList;
	size_t DataSize, Numberofblocks;

public:
	size_t allocations = 0;
	uintptr_t start;
	Allocators(size_t size, size_t blocknum) {
		DataSize = size; Numberofblocks = blocknum; start = 0; FreeAddressList = new BitArray(blocknum); FreeAddressList->setAll();
		FreeAddressList->clearBit(0);
	}
	~Allocators();
	void destroy();
	void* alloc(size_t size);
	bool free(void* ptr);
	void showOutstandingBlocks() const;
	void showFreeBlocks() const;
};