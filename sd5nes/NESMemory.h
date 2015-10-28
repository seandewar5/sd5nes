#pragma once

#include <memory>

#include "NESTypes.h"

/**
* Represents the memory used by the NES system.
*/
class NESMemory
{
public:
	NESMemory(uleast32 size);
	~NESMemory();

	/**
	* Writes 8-bits to the NES's memory at a specified location with the specified value.
	* Memory will be mapped correctly.
	* Returns true on success, false on failure.
	*/
	bool Write8(u16 addr, u8 val);

	/**
	* Reads 8-bits from the NES's memory at a specified location and modifies outVal (if not null).
	* Memory will be mapped correctly.
	* Returns true on success, false on failure. If failure, outVal is not modified.
	*/
	bool Read8(u16 addr, u8* outVal) const;

	/**
	* Reads 16-bits from the NES's memory at a specified location and modifies outVal (if not null).
	* Memory will be mapped correctly.
	* Returns true on success, false on failure. If failure, outVal is not modified.
	*/
	bool Read16(u16 addr, u16* outVal) const;

	// Get the allocated size of this memory.
	uleast32 GetSize() const;

private:
	std::unique_ptr<u8[]> data_;
	uleast32 size_;
};

