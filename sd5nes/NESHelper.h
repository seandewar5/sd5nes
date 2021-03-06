#pragma once

#include <string>
#include <cassert>

#include "NESTypes.h"
#include "NESMemory.h"

/**
* Define std::make_unique for compilers
* that actually follow the C++11 standard properly...
*/
#ifndef _MSC_VER

#include <memory>

namespace std
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif

/**
* Contains helper functions.
*/
namespace NESHelper
{
	/**
	* Struct that allows enum classes to be stored in an
	* unordered_map.
	*/
	struct EnumClassHash
	{
		template<typename T>
		inline std::size_t operator()(T val) const
		{
			return static_cast<std::size_t>(val);
		}
	};

	/**
	* Gets a random bool. The chance of the returned value being true
	* is affected by trueChance. 1.0 = always true, 0.0 = always false.
	*/
	bool GetRandomBool(double trueChance = 0.5);

	/**
	* Reverses the bits in an 8-bit value and returns the result.
	*/
	inline u8 ReverseBits(u8 val)
	{
		val = (val & 0xF0) >> 4 | (val & 0xF) << 4;
		val = (val & 0xCC) >> 2 | (val & 0x33) << 2;
		val = (val & 0xAA) >> 1 | (val & 0x55) << 1;
		return val;
	}

	/**
	* Gets the value of a bit in the specified position of an 8-bit value.
	*/
	inline bool IsBitSet(u8 val, u8 pos) 
	{
		assert(pos < 8); 
		return ((val & (1 << pos)) != 0); 
	}

	/**
	* Sets the bit at the specified position of an 8-bit value.
	*/
	inline void SetRefBit(u8& val, u8 pos) 
	{ 
		assert(pos < 8); 
		val |= (1 << pos); 
	}

	/**
	* Sets the bit at the specified position of an 8-bit value.
	* Returns the result and does not modify val.
	*/
	inline u8 SetBit(u8 val, u8 pos)
	{
		SetRefBit(val, pos);
		return val;
	}

	/**
	* Clear the bit at the specified position of an 8-bit value.
	*/
	inline void ClearRefBit(u8& val, u8 pos) 
	{ 
		assert(pos < 8); 
		val &= ~(1 << pos); 
	}

	/**
	* Clear the bit at the specified position of an 8-bit value.
	* Returns the result and does not modify val.
	*/
	inline u8 ClearBit(u8 val, u8 pos)
	{
		ClearRefBit(val, pos);
		return val;
	}

	/**
	* Sets/clears the bit at the specified position of an 8-bit value.
	*/
	inline void EditRefBit(u8& val, u8 pos, bool setBit) 
	{ 
		if (setBit)
			SetRefBit(val, pos);
		else
			ClearRefBit(val, pos); 
	}

	/**
	* Sets/clears the bit at the specified position of an 8-bit value.
	* Returns the result and does not modify val.
	*/
	inline u8 EditBit(u8 val, u8 pos, bool setBit)
	{
		EditRefBit(val, pos, setBit);
		return val;
	}

	/**
	* Checks whether or not addr1 and addr2 are in the same page of memory.
	*/
	inline bool IsInSamePage(u16 addr1, u16 addr2) { return ((addr1 & 0xFF00) == (addr2 & 0xFF00)); }

	/**
	* Converts two 8-bit values to one 16-bit value.
	*/
	inline u16 ConvertTo16(u8 hi, u8 lo) { return (hi << 8) | lo; }

	/**
	* Reads 16-bits (little-endian) from a memory interface at the specified address.
	*/
	inline u16 MemoryRead16(const INESMemoryInterface& mem, u16 addr) { return ConvertTo16(mem.Read8(addr + 1), mem.Read8(addr)); }

	/**
	* Reads 16-bits (little-endian) from a memory interface while accounting for the 6502 indirect addressing bug.
	* The bug causes the high byte of addr to not increment when the low byte wraps due to the increment in addr.
	*/
	inline u16 MemoryIndirectRead16(const INESMemoryInterface& mem, u16 addr) 
	{ 
		return ConvertTo16(mem.Read8((addr & 0xFF00) | ((addr + 1) & 0xFF)), mem.Read8(addr)); 
	}
};
