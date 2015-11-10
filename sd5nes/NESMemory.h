#pragma once

#include <vector>

#include "NESTypes.h"
#include "NESMemoryConstants.h"

/**
* Represents the memory used by a hardware component of the NES system.
*/
class NESMemory
{
public:
	NESMemory(uleast16 size = 0xFFFF);
	virtual ~NESMemory();

	/**
	* Sets all the allocated memory to zero.
	*/
	void ZeroMemory();

	/**
	* Copies the contents from a buffer into zero'd memory.
	*/
	void CopyFromBuffer(const std::vector<u8>& buf);

	/**
	* Writes 8-bits to the memory at a specified location with the specified value.
	* Returns true on success, false on failure.
	*/
	virtual bool Write8(u16 addr, u8 val);

	/**
	* Reads 8-bits from the memory at a specified location and modifies outVal (if not null).
	* Returns true on success, false on failure. If failure, outVal is not modified.
	*/
	virtual bool Read8(u16 addr, u8* outVal) const;

	/**
	* Reads 16-bits from the memory at a specified location and modifies outVal (if not null).
	* Returns true on success, false on failure. If failure, outVal is not modified.
	*/
	virtual bool Read16(u16 addr, u16* outVal) const;

	// Get the allocated size of this memory.
	uleast32 GetSize() const;

private:
	std::vector<u8> data_;
};

