#include "BitArray.h"


BitArray::BitArray(size_t numbits)
{
	TotalBytes = (numbits - 1) / BitsPerElement + 1;
	p_Bitdata = new Bitdata[TotalBytes];
	numofblocks = numbits;
}

BitArray::~BitArray()
{
	delete[] p_Bitdata;
}

void BitArray::setAll() {
	memset(p_Bitdata, 1, TotalBytes * sizeof(Bitdata));
}

void BitArray::clearAll() {
	memset(p_Bitdata, 0, TotalBytes * sizeof(Bitdata));
}

bool BitArray::FindFirstSetBit(size_t& b_Index)
{
	auto index = 0;
	size_t size = TotalBytes;
	while (p_Bitdata[index] == Bitdata(0) && index < size)
	{
		index++;
	}
	if (index == size) return false;

	Bitdata data = p_Bitdata[index];
	unsigned long pos;
#if defined(WIN32)
	_BitScanForward(&pos, data);
#else
	_BitScanForward64(&pos, data);
#endif
	size_t offset = BitsPerElement - pos - 1;


	b_Index = index * BitsPerElement + offset;
	if (b_Index > numofblocks) { return false; }
	return true;
}

bool BitArray::FindFirstClearBit(size_t& b_Index) {
	auto index = 0;
	size_t size = TotalBytes;
	while (p_Bitdata[index] == Bitdata(1) && index < size)
	{
		index++;
	}
	if (index == size) return false;

	Bitdata data = p_Bitdata[index];
	unsigned long pos;
#if defined(WIN32)
	_BitScanForward(&pos, data);
#else
	_BitScanForward64(&pos, data);
#endif
	size_t offset = BitsPerElement - pos - 1;


	b_Index = index * BitsPerElement + offset;
	if (b_Index > numofblocks) { return false; }
	return true;
}
bool BitArray::isBitSet(size_t bitIndex) {
	Bitdata data = p_Bitdata[bitIndex / BitsPerElement];
	if (data == static_cast<Bitdata>(1)) { return true; }
	return false;
}

bool BitArray::operator[](size_t index)
{
	Bitdata data = p_Bitdata[index / BitsPerElement];
	size_t b_Pos = index % BitsPerElement;
	return data & (static_cast<Bitdata>(1) << (BitsPerElement - b_Pos - 1));
}

void BitArray::setBit(size_t bitIndex) {
	size_t index = 0;
	size_t size = TotalBytes;
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
	size_t size = TotalBytes;
	while ((p_Bitdata[index] == 1) && (index < size))
	{
		index++;
	}
	Bitdata* data = &p_Bitdata[bitIndex / BitsPerElement];
	size_t b_Pos = bitIndex % BitsPerElement;
	*data = *data & (static_cast<Bitdata>(0) << (BitsPerElement - b_Pos - 1));
}