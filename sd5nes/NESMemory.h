#pragma once

#include <vector>
#include <stdexcept>
#include <cassert>

#include "NESTypes.h"
#include "NESMemoryConstants.h"

/**
* Exception thrown during memory access errors.
*/
class NESMemoryException : public std::runtime_error
{
public:
	explicit NESMemoryException(const char* msg) :
		std::runtime_error(msg)
	{ }

	explicit NESMemoryException(const std::string& msg) :
		std::runtime_error(msg)
	{ }

	virtual ~NESMemoryException()
	{ }
};

/**
* Interface that declares some basic read/write functions.
*/
class INESMemoryInterface
{
public:
	virtual void Write8(u16 addr, u8 val) = 0;
	virtual u8 Read8(u16 addr) const = 0;
	virtual u16 Read16(u16 addr) const = 0;
};

/**
* Basic structure containing information about memory maps in an NESMemoryMap.
*/
struct NESMemoryMappingInfo
{
	const u16 startAddr, size;
	NESMemory& memory;

	NESMemoryMappingInfo(NESMemory& memory, u16 startAddr, u16 size) :
		startAddr(startAddr),
		size(size),
		memory(memory)
	{
		assert("Specified size is higher than the allocated memory size being mapped!" &&
			size > memory.GetSize());
	}

	inline bool operator==(const NESMemoryMappingInfo& rhs) const
	{
		return (startAddr == rhs.startAddr && 
			size == rhs.size &&
			&memory == &rhs.memory);
	}
};

/**
* Basic structure containing information about mirroring in an NESMemoryMap.
*/
struct NESMemoryMirroringInfo
{
	const u16 startAddr, mirrorToAddr, size;

	NESMemoryMirroringInfo(u16 startAddr, u16 mirrorToAddr, u16 size) :
		startAddr(startAddr),
		mirrorToAddr(mirrorToAddr),
		size(size)
	{ }

	inline bool operator==(const NESMemoryMirroringInfo& rhs) const
	{
		return (startAddr == rhs.startAddr && 
			mirrorToAddr == rhs.mirrorToAddr &&
			size == rhs.size);
	}
};

/**
* Memory module that maps memory together.
*/
class NESMemoryMap : public INESMemoryInterface
{
public:
	NESMemoryMap();
	virtual ~NESMemoryMap();

	/**
	* Adds memory to be mapped at a specified location.
	*/
	void AddMemoryMapping(NESMemory& memory, u16 startAddr, u16 size);

	/**
	* Defines where to mirror the memory to and of what size the mirror should be.
	*/
	void AddMemoryMirror(u16 startAddr, u16 mirrorToAddr, u16 size);

	/**
	* Defines where to mirror the memory to and repeats the memory until the mirrored location
	* between mirrorToAddr and mirrorEndAddr is filled.
	*/
	void AddMemoryMirrorRange(u16 startAddr, u16 endAddr, u16 mirrorToAddr, u16 mirrorEndAddr);

	/**
	* Writes 8-bits to the memory at a specified location with the specified value.
	* Maps the memory correctly.
	* Can throw NESMemoryException.
	*/
	virtual void Write8(u16 addr, u8 val) override;

	/**
	* Reads 8-bits from the memory at a specified location with the specified value.
	* Maps the memory correctly.
	* Can throw NESMemoryException.
	*/
	virtual u8 Read8(u16 addr) const override;

	/**
	* Reads 16-bits from the memory at a specified location with the specified value.
	* Maps the memory correctly.
	* Can throw NESMemoryException.
	*/
	virtual u16 Read16(u16 addr) const override;

private:
	std::vector<NESMemoryMappingInfo> mappings_;
	std::vector<NESMemoryMirroringInfo> mirrors_;

	std::pair<NESMemory&, u16> GetMapping(u16 addr) const;
	u16 LookupMirrorAddress(u16 addr) const;
};

/**
* Represents the memory used by a hardware component of the NES system.
*/
class NESMemory : public INESMemoryInterface
{
public:
	NESMemory(uleast16 size = 0);
	~NESMemory();

	/**
	* Sets all the allocated memory to zero.
	*/
	inline void ZeroMemory() { std::fill(data_.begin(), data_.end(), 0); }

	/**
	* Copies the contents from a buffer into zero'd memory.
	*/
	void CopyFromBuffer(const std::vector<u8>& buf);

	/**
	* Writes 8-bits to the memory at a specified location with the specified value.
	*/
	void Write8(u16 addr, u8 val) override;

	/**
	* Reads 8-bits from the memory at a specified location.
	*/
	u8 Read8(u16 addr) const override;

	/**
	* Reads 16-bits from the memory at a specified location.
	*/
	u16 Read16(u16 addr) const override;

	/**
	* Gets the allocated size.
	*/
	inline uleast32 GetSize() const { return data_.size(); }

private:
	std::vector<u8> data_;
};

