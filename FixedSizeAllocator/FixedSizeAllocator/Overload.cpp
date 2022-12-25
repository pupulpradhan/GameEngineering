#include "Overload.h"

void* malloc(size_t i_size, HeapManager* hm)
{
	return hm->_alloc(i_size);
}

void free(void* i_ptr, HeapManager* hm)
{
	hm->_free(i_ptr);
}

void* operator new(size_t i_size) {
	return malloc(i_size, HeapManager::instance);
}

void operator delete(void* i_ptr) {
	free(i_ptr, HeapManager::instance);
}

void* operator new[](size_t i_size) {
	return malloc(i_size, HeapManager::instance);
}

void operator delete [](void* i_ptr) {
	free(i_ptr, HeapManager::instance);
}