#include "BitArray.h"


BitArray::BitArray(size_t numbits)
{
	T_Bytes = (numbits - 1) / BitsPerElement + 1;
	p_Bitdata = new Bitdata[T_Bytes];
	numofblocks = numbits;
}

BitArray::~BitArray()
{
	delete[] p_Bitdata;
}

void BitArray::setAll() {
	memset(p_Bitdata, ~0, T_Bytes * sizeof(Bitdata));
}

void BitArray::clearAll() {
	memset(p_Bitdata, 0, T_Bytes * sizeof(Bitdata));
}

bool BitArray::FindFirstSetBit(size_t& b_Index)
{
	auto index = 0;
	size_t size = T_Bytes;
	while (p_Bitdata[index] == Bitdata(0) && index < T_Bytes)
	{
		index++;
	}
	if (index == size) return false;

	Bitdata data = p_Bitdata[index];
	unsigned long pos;
#if defined(WIN32)
	_BitScanReverse(&pos, data);
#else
	_BitScanReverse64(&pos, data);
#endif
	size_t poss = BitsPerElement - pos - 1;


	b_Index = index * BitsPerElement + poss;
	if (b_Index > numofblocks) { return false; }
	return true;
}

bool BitArray::operator[](size_t index)
{
	Bitdata data = p_Bitdata[index / BitsPerElement];
	size_t b_Pos = index % BitsPerElement;
	return data & (static_cast<Bitdata>(1) << (BitsPerElement - b_Pos - 1));
}

void BitArray::setBit(size_t bitIndex) {
	size_t index = 0;
	size_t size = T_Bytes;
	while ((p_Bitdata[index] == 0) && (index < size))
	{
		index++;
	}
	Bitdata* data = &p_Bitdata[bitIndex / BitsPerElement];
	size_t b_Pos = bitIndex % BitsPerElement;
	*data = *data | (static_cast<Bitdata>(1) << (BitsPerElement - b_Pos - 1));
}

void BitArray::clearBit(size_t bitIndex) {
	size_t index = 0;
	size_t size = T_Bytes;
	while ((p_Bitdata[index] == 0) && (index < size))
	{
		index++;
	}
	Bitdata* data = &p_Bitdata[bitIndex / BitsPerElement];
	size_t b_Pos = bitIndex % BitsPerElement;
	*data = *data & ~(static_cast<Bitdata>(1) << (BitsPerElement - b_Pos - 1));
}