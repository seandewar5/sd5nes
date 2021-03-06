#pragma once

#include "NESCPU.h"
#include "NESPPU.h"
#include "NESMMC.h"

class INESController;

typedef std::array<INESController*, 2> NESControllerPorts;

/**
* Communications interface allowing the CPU to communicate with its
* RAM, the PPU and the MMC on the Cartridge.
*/
class NESCPUEmuComm : public INESCPUCommunicationsInterface
{
public:
	NESCPUEmuComm(NESMemCPURAM& ram, NESPPU& ppu, INESMMC& mmc, const NESControllerPorts& controllers);
	virtual ~NESCPUEmuComm();

	void Write8(u16 addr, u8 val) override;
	u8 Read8(u16 addr) const override;

private:
	static NESPPURegisterType GetPPURegister(u16 realAddr);

	NESMemCPURAM& ram_;
	NESPPU& ppu_;
	INESMMC& mmc_;
	const NESControllerPorts& controllers_;
};

