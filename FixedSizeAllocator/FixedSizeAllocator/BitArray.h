#pragma once
#include <cstdint>
#include "intrin.h"
#include <cstring>
class BitArray
{
#ifdef WIN32
	typedef uint32_t Bitdata;
#else
	typedef uint64_t Bitdata;
#endif
private:
	Bitdata* p_Bitdata;
	size_t numofblocks;
	const size_t BitsPerElement = sizeof(Bitdata) * 8;
	size_t TotalBytes;
public:
	BitArray(size_t numbits);
	~BitArray();
	bool FindFirstSetBit(size_t& b_Index);
	bool FindFirstClearBit(size_t& b_Index);
	bool isBitSet(size_t bitIndex);
	void setAll();
	void clearAll();
	bool operator[](size_t index);
	void setBit(size_t bitIndex);
	void clearBit(size_t bitIndex);
};