#include <iostream>

struct MemoryBlock {
	*void baseadd;
	size_t blocksize;
};

void* VirtualAlloc(size_t size) {
	void* add = TheHeap.baseadd;
	TheHeap.baseadd += size;
	Theheap.blocksize -= size;

	return add;
}
void* HeapAlloc() {
}

void main() {
	struct MemoryBlock TheHeap;
	void* memadd = VirtualAlloc(64);
}