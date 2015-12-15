#include "NESCPU.h"

#include <cassert>
#include <sstream>
#include <iostream> // @TODO Debug?

#include "NESMMC.h"


NESCPUMemoryMapper::NESCPUMemoryMapper(NESMemCPURAM& ram, NESPPU& ppu, NESMMC* mmc) :
ram_(ram),
ppu_(ppu),
mmc_(mmc)
{
	assert(mmc_ != nullptr);
}


NESCPUMemoryMapper::~NESCPUMemoryMapper()
{
}


NESPPURegisterType NESCPUMemoryMapper::GetPPURegister(u16 realAddr)
{
	switch (realAddr)
	{
	case 0x2000:
		return NESPPURegisterType::PPUCTRL;

	case 0x2001:
		return NESPPURegisterType::PPUMASK;

	case 0x2002:
		return NESPPURegisterType::PPUSTATUS;

	case 0x2003:
		return NESPPURegisterType::OAMADDR;

	case 0x2004:
		return NESPPURegisterType::OAMDATA;

	case 0x2005:
		return NESPPURegisterType::PPUSCROLL;

	case 0x2006:
		return NESPPURegisterType::PPUADDR;

	case 0x2007:
		return NESPPURegisterType::PPUDATA;

	case 0x4014:
		return NESPPURegisterType::OAMDMA;

	default:
		return NESPPURegisterType::UNKNOWN;
	}
}


void NESCPUMemoryMapper::Write8(u16 addr, u8 val)
{
	if (addr < 0x2000) // RAM
		ram_.Write8(addr & 0x7FF, val);
	else if (addr < 0x4000) // PPU I/O Registers
		ppu_.WriteRegister(GetPPURegister(0x2000 + (addr & 7)), val);
	else if (addr == 0x4014) // PPU I/O OAMDATA Register
		ppu_.WriteRegister(GetPPURegister(0x4014), val);
	else if (addr < 0x4020) // pAPU I/O Registers
		return; // @TODO
	else // Use the MMC
		mmc_->Write8(addr, val);
}


u8 NESCPUMemoryMapper::Read8(u16 addr) const
{
	if (addr < 0x2000) // RAM
		return ram_.Read8(addr & 0x7FF);
	else if (addr < 0x4000) // PPU I/O Registers
		return ppu_.ReadRegister(GetPPURegister(0x2000 + (addr & 7)));
	else if (addr == 0x4014) // PPU I/O OAMDATA Register
		return ppu_.ReadRegister(GetPPURegister(0x4014));
	else if (addr < 0x4020) // pAPU I/O Registers
		return 0; // @TODO
	else // Use the MMC
		return mmc_->Read8(addr);
}


std::unordered_map<u8, NESCPUOpInfo> NESCPU::opInfos_;
NESCPUStaticInit NESCPU::staticInit_;


void NESCPU::RegisterOpMapping(const std::string& opName, u8 op, NESOpFuncPointer opFunc, NESCPUOpAddressingMode addrMode, int cycleCount)
{
	assert("Duplicate op mapping entry or unknown addressing mode!"
		&& (opInfos_.emplace(op, NESCPUOpInfo(opName, opFunc, addrMode, cycleCount)).second)
		&& (addrMode != NESCPUOpAddressingMode::UNKNOWN));
}


NESCPUStaticInit::NESCPUStaticInit()
{
	/****************************************/
	/********* Instruction Mappings *********/
	/****************************************/

	// ADC
	NESCPU::RegisterOpMapping(NES_OP_ADC_NAME, NES_OP_ADC_IMMEDIATE, &NESCPU::ExecuteOpADC, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_ADC_NAME, NES_OP_ADC_ABSOLUTE, &NESCPU::ExecuteOpADC, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_ADC_NAME, NES_OP_ADC_ABSOLUTE_X, &NESCPU::ExecuteOpADC, NESCPUOpAddressingMode::ABSOLUTE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_ADC_NAME, NES_OP_ADC_ABSOLUTE_Y, &NESCPU::ExecuteOpADC, NESCPUOpAddressingMode::ABSOLUTE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_ADC_NAME, NES_OP_ADC_ZEROPAGE, &NESCPU::ExecuteOpADC, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_ADC_NAME, NES_OP_ADC_ZEROPAGE_X, &NESCPU::ExecuteOpADC, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_ADC_NAME, NES_OP_ADC_INDIRECT_X, &NESCPU::ExecuteOpADC, NESCPUOpAddressingMode::INDIRECT_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_ADC_NAME, NES_OP_ADC_INDIRECT_Y, &NESCPU::ExecuteOpADC, NESCPUOpAddressingMode::INDIRECT_Y, 5);

	// AND
	NESCPU::RegisterOpMapping(NES_OP_AND_NAME, NES_OP_AND_IMMEDIATE, &NESCPU::ExecuteOpAND, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_AND_NAME, NES_OP_AND_ABSOLUTE, &NESCPU::ExecuteOpAND, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_AND_NAME, NES_OP_AND_ABSOLUTE_X, &NESCPU::ExecuteOpAND, NESCPUOpAddressingMode::ABSOLUTE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_AND_NAME, NES_OP_AND_ABSOLUTE_Y, &NESCPU::ExecuteOpAND, NESCPUOpAddressingMode::ABSOLUTE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_AND_NAME, NES_OP_AND_ZEROPAGE, &NESCPU::ExecuteOpAND, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_AND_NAME, NES_OP_AND_ZEROPAGE_X, &NESCPU::ExecuteOpAND, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_AND_NAME, NES_OP_AND_INDIRECT_X, &NESCPU::ExecuteOpAND, NESCPUOpAddressingMode::INDIRECT_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_AND_NAME, NES_OP_AND_INDIRECT_Y, &NESCPU::ExecuteOpAND, NESCPUOpAddressingMode::INDIRECT_Y, 5);

	// ASL
	NESCPU::RegisterOpMapping(NES_OP_ASL_NAME, NES_OP_ASL_ACCUMULATOR, &NESCPU::ExecuteOpASL, NESCPUOpAddressingMode::ACCUMULATOR, 2);
	NESCPU::RegisterOpMapping(NES_OP_ASL_NAME, NES_OP_ASL_ZEROPAGE, &NESCPU::ExecuteOpASL, NESCPUOpAddressingMode::ZEROPAGE, 5);
	NESCPU::RegisterOpMapping(NES_OP_ASL_NAME, NES_OP_ASL_ZEROPAGE_X, &NESCPU::ExecuteOpASL, NESCPUOpAddressingMode::ZEROPAGE_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_ASL_NAME, NES_OP_ASL_ABSOLUTE, &NESCPU::ExecuteOpASL, NESCPUOpAddressingMode::ABSOLUTE, 6);
	NESCPU::RegisterOpMapping(NES_OP_ASL_NAME, NES_OP_ASL_ABSOLUTE_X, &NESCPU::ExecuteOpASL, NESCPUOpAddressingMode::ABSOLUTE_X, 7);

	// BCC
	NESCPU::RegisterOpMapping(NES_OP_BCC_NAME, NES_OP_BCC_RELATIVE, &NESCPU::ExecuteOpBCC, NESCPUOpAddressingMode::RELATIVE, 2);

	// BCS
	NESCPU::RegisterOpMapping(NES_OP_BCS_NAME, NES_OP_BCS_RELATIVE, &NESCPU::ExecuteOpBCS, NESCPUOpAddressingMode::RELATIVE, 2);

	// BEQ
	NESCPU::RegisterOpMapping(NES_OP_BEQ_NAME, NES_OP_BEQ_RELATIVE, &NESCPU::ExecuteOpBEQ, NESCPUOpAddressingMode::RELATIVE, 2);

	// BIT
	NESCPU::RegisterOpMapping(NES_OP_BIT_NAME, NES_OP_BIT_ZEROPAGE, &NESCPU::ExecuteOpBIT, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_BIT_NAME, NES_OP_BIT_ABSOLUTE, &NESCPU::ExecuteOpBIT, NESCPUOpAddressingMode::ABSOLUTE, 4);

	// BMI
	NESCPU::RegisterOpMapping(NES_OP_BMI_NAME, NES_OP_BMI_RELATIVE, &NESCPU::ExecuteOpBMI, NESCPUOpAddressingMode::RELATIVE, 2);

	// BNE
	NESCPU::RegisterOpMapping(NES_OP_BNE_NAME, NES_OP_BNE_RELATIVE, &NESCPU::ExecuteOpBNE, NESCPUOpAddressingMode::RELATIVE, 2);

	// BPL
	NESCPU::RegisterOpMapping(NES_OP_BPL_NAME, NES_OP_BPL_RELATIVE, &NESCPU::ExecuteOpBPL, NESCPUOpAddressingMode::RELATIVE, 2);

	// BRK
	NESCPU::RegisterOpMapping(NES_OP_BRK_NAME, NES_OP_BRK_IMPLIED, &NESCPU::ExecuteOpBRK, NESCPUOpAddressingMode::IMPLIED_BRK, 7);

	// BVC
	NESCPU::RegisterOpMapping(NES_OP_BVC_NAME, NES_OP_BVC_RELATIVE, &NESCPU::ExecuteOpBVC, NESCPUOpAddressingMode::RELATIVE, 2);

	// BVS
	NESCPU::RegisterOpMapping(NES_OP_BVS_NAME, NES_OP_BVS_RELATIVE, &NESCPU::ExecuteOpBVS, NESCPUOpAddressingMode::RELATIVE, 2);

	// CLC
	NESCPU::RegisterOpMapping(NES_OP_CLC_NAME, NES_OP_CLC_IMPLIED, &NESCPU::ExecuteOpCLC, NESCPUOpAddressingMode::IMPLIED, 2);

	// CLD
	NESCPU::RegisterOpMapping(NES_OP_CLD_NAME, NES_OP_CLD_IMPLIED, &NESCPU::ExecuteOpCLD, NESCPUOpAddressingMode::IMPLIED, 2);

	// CLI
	NESCPU::RegisterOpMapping(NES_OP_CLI_NAME, NES_OP_CLI_IMPLIED, &NESCPU::ExecuteOpCLI, NESCPUOpAddressingMode::IMPLIED, 2);

	// CLV
	NESCPU::RegisterOpMapping(NES_OP_CLV_NAME, NES_OP_CLV_IMPLIED, &NESCPU::ExecuteOpCLV, NESCPUOpAddressingMode::IMPLIED, 2);

	// CMP
	NESCPU::RegisterOpMapping(NES_OP_CMP_NAME, NES_OP_CMP_IMMEDIATE, &NESCPU::ExecuteOpCMP, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_CMP_NAME, NES_OP_CMP_ABSOLUTE, &NESCPU::ExecuteOpCMP, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_CMP_NAME, NES_OP_CMP_ABSOLUTE_X, &NESCPU::ExecuteOpCMP, NESCPUOpAddressingMode::ABSOLUTE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_CMP_NAME, NES_OP_CMP_ABSOLUTE_Y, &NESCPU::ExecuteOpCMP, NESCPUOpAddressingMode::ABSOLUTE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_CMP_NAME, NES_OP_CMP_ZEROPAGE, &NESCPU::ExecuteOpCMP, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_CMP_NAME, NES_OP_CMP_ZEROPAGE_X, &NESCPU::ExecuteOpCMP, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_CMP_NAME, NES_OP_CMP_INDIRECT_X, &NESCPU::ExecuteOpCMP, NESCPUOpAddressingMode::INDIRECT_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_CMP_NAME, NES_OP_CMP_INDIRECT_Y, &NESCPU::ExecuteOpCMP, NESCPUOpAddressingMode::INDIRECT_Y, 5);

	// CPX
	NESCPU::RegisterOpMapping(NES_OP_CPX_NAME, NES_OP_CPX_IMMEDIATE, &NESCPU::ExecuteOpCPX, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_CPX_NAME, NES_OP_CPX_ZEROPAGE, &NESCPU::ExecuteOpCPX, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_CPX_NAME, NES_OP_CPX_ABSOLUTE, &NESCPU::ExecuteOpCPX, NESCPUOpAddressingMode::ABSOLUTE, 4);

	// CPY
	NESCPU::RegisterOpMapping(NES_OP_CPY_NAME, NES_OP_CPY_IMMEDIATE, &NESCPU::ExecuteOpCPY, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_CPY_NAME, NES_OP_CPY_ZEROPAGE, &NESCPU::ExecuteOpCPY, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_CPY_NAME, NES_OP_CPY_ABSOLUTE, &NESCPU::ExecuteOpCPY, NESCPUOpAddressingMode::ABSOLUTE, 4);

	// DEC
	NESCPU::RegisterOpMapping(NES_OP_DEC_NAME, NES_OP_DEC_ZEROPAGE, &NESCPU::ExecuteOpDEC, NESCPUOpAddressingMode::ZEROPAGE, 5);
	NESCPU::RegisterOpMapping(NES_OP_DEC_NAME, NES_OP_DEC_ZEROPAGE_X, &NESCPU::ExecuteOpDEC, NESCPUOpAddressingMode::ZEROPAGE_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_DEC_NAME, NES_OP_DEC_ABSOLUTE, &NESCPU::ExecuteOpDEC, NESCPUOpAddressingMode::ABSOLUTE, 6);
	NESCPU::RegisterOpMapping(NES_OP_DEC_NAME, NES_OP_DEC_ABSOLUTE_X, &NESCPU::ExecuteOpDEC, NESCPUOpAddressingMode::ABSOLUTE_X, 7);

	// DEX
	NESCPU::RegisterOpMapping(NES_OP_DEX_NAME, NES_OP_DEX_IMPLIED, &NESCPU::ExecuteOpDEX, NESCPUOpAddressingMode::IMPLIED, 2);

	// DEY
	NESCPU::RegisterOpMapping(NES_OP_DEY_NAME, NES_OP_DEY_IMPLIED, &NESCPU::ExecuteOpDEY, NESCPUOpAddressingMode::IMPLIED, 2);

	// EOR
	NESCPU::RegisterOpMapping(NES_OP_EOR_NAME, NES_OP_EOR_IMMEDIATE, &NESCPU::ExecuteOpEOR, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_EOR_NAME, NES_OP_EOR_ABSOLUTE, &NESCPU::ExecuteOpEOR, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_EOR_NAME, NES_OP_EOR_ABSOLUTE_X, &NESCPU::ExecuteOpEOR, NESCPUOpAddressingMode::ABSOLUTE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_EOR_NAME, NES_OP_EOR_ABSOLUTE_Y, &NESCPU::ExecuteOpEOR, NESCPUOpAddressingMode::ABSOLUTE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_EOR_NAME, NES_OP_EOR_ZEROPAGE, &NESCPU::ExecuteOpEOR, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_EOR_NAME, NES_OP_EOR_ZEROPAGE_X, &NESCPU::ExecuteOpEOR, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_EOR_NAME, NES_OP_EOR_INDIRECT_X, &NESCPU::ExecuteOpEOR, NESCPUOpAddressingMode::INDIRECT_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_EOR_NAME, NES_OP_EOR_INDIRECT_Y, &NESCPU::ExecuteOpEOR, NESCPUOpAddressingMode::INDIRECT_Y, 5);

	// INC
	NESCPU::RegisterOpMapping(NES_OP_INC_NAME, NES_OP_INC_ZEROPAGE, &NESCPU::ExecuteOpINC, NESCPUOpAddressingMode::ZEROPAGE, 5);
	NESCPU::RegisterOpMapping(NES_OP_INC_NAME, NES_OP_INC_ZEROPAGE_X, &NESCPU::ExecuteOpINC, NESCPUOpAddressingMode::ZEROPAGE_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_INC_NAME, NES_OP_INC_ABSOLUTE, &NESCPU::ExecuteOpINC, NESCPUOpAddressingMode::ABSOLUTE, 6);
	NESCPU::RegisterOpMapping(NES_OP_INC_NAME, NES_OP_INC_ABSOLUTE_X, &NESCPU::ExecuteOpINC, NESCPUOpAddressingMode::ABSOLUTE_X, 7);

	// INX
	NESCPU::RegisterOpMapping(NES_OP_INX_NAME, NES_OP_INX_IMPLIED, &NESCPU::ExecuteOpINX, NESCPUOpAddressingMode::IMPLIED, 2);

	// INY
	NESCPU::RegisterOpMapping(NES_OP_INY_NAME, NES_OP_INY_IMPLIED, &NESCPU::ExecuteOpINY, NESCPUOpAddressingMode::IMPLIED, 2);

	// JMP
	NESCPU::RegisterOpMapping(NES_OP_JMP_NAME, NES_OP_JMP_ABSOLUTE, &NESCPU::ExecuteOpJMP, NESCPUOpAddressingMode::ABSOLUTE, 3);
	NESCPU::RegisterOpMapping(NES_OP_JMP_NAME, NES_OP_JMP_INDIRECT, &NESCPU::ExecuteOpJMP, NESCPUOpAddressingMode::INDIRECT, 5);

	// JSR
	NESCPU::RegisterOpMapping(NES_OP_JSR_NAME, NES_OP_JSR_ABSOLUTE, &NESCPU::ExecuteOpJSR, NESCPUOpAddressingMode::ABSOLUTE, 6);

	// LDA
	NESCPU::RegisterOpMapping(NES_OP_LDA_NAME, NES_OP_LDA_IMMEDIATE, &NESCPU::ExecuteOpLDA, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_LDA_NAME, NES_OP_LDA_ABSOLUTE, &NESCPU::ExecuteOpLDA, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_LDA_NAME, NES_OP_LDA_ABSOLUTE_X, &NESCPU::ExecuteOpLDA, NESCPUOpAddressingMode::ABSOLUTE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_LDA_NAME, NES_OP_LDA_ABSOLUTE_Y, &NESCPU::ExecuteOpLDA, NESCPUOpAddressingMode::ABSOLUTE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_LDA_NAME, NES_OP_LDA_ZEROPAGE, &NESCPU::ExecuteOpLDA, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_LDA_NAME, NES_OP_LDA_ZEROPAGE_X, &NESCPU::ExecuteOpLDA, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_LDA_NAME, NES_OP_LDA_INDIRECT_X, &NESCPU::ExecuteOpLDA, NESCPUOpAddressingMode::INDIRECT_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_LDA_NAME, NES_OP_LDA_INDIRECT_Y, &NESCPU::ExecuteOpLDA, NESCPUOpAddressingMode::INDIRECT_Y, 5);

	// LDX
	NESCPU::RegisterOpMapping(NES_OP_LDX_NAME, NES_OP_LDX_IMMEDIATE, &NESCPU::ExecuteOpLDX, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_LDX_NAME, NES_OP_LDX_ZEROPAGE, &NESCPU::ExecuteOpLDX, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_LDX_NAME, NES_OP_LDX_ZEROPAGE_Y, &NESCPU::ExecuteOpLDX, NESCPUOpAddressingMode::ZEROPAGE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_LDX_NAME, NES_OP_LDX_ABSOLUTE, &NESCPU::ExecuteOpLDX, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_LDX_NAME, NES_OP_LDX_ABSOLUTE_Y, &NESCPU::ExecuteOpLDX, NESCPUOpAddressingMode::ABSOLUTE_Y, 4);

	// LDY
	NESCPU::RegisterOpMapping(NES_OP_LDY_NAME, NES_OP_LDY_IMMEDIATE, &NESCPU::ExecuteOpLDY, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_LDY_NAME, NES_OP_LDY_ZEROPAGE, &NESCPU::ExecuteOpLDY, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_LDY_NAME, NES_OP_LDY_ZEROPAGE_X, &NESCPU::ExecuteOpLDY, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_LDY_NAME, NES_OP_LDY_ABSOLUTE, &NESCPU::ExecuteOpLDY, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_LDY_NAME, NES_OP_LDY_ABSOLUTE_X, &NESCPU::ExecuteOpLDY, NESCPUOpAddressingMode::ABSOLUTE_X, 4);

	// LSR
	NESCPU::RegisterOpMapping(NES_OP_LSR_NAME, NES_OP_LSR_ACCUMULATOR, &NESCPU::ExecuteOpLSR, NESCPUOpAddressingMode::ACCUMULATOR, 2);
	NESCPU::RegisterOpMapping(NES_OP_LSR_NAME, NES_OP_LSR_ZEROPAGE, &NESCPU::ExecuteOpLSR, NESCPUOpAddressingMode::ZEROPAGE, 5);
	NESCPU::RegisterOpMapping(NES_OP_LSR_NAME, NES_OP_LSR_ZEROPAGE_X, &NESCPU::ExecuteOpLSR, NESCPUOpAddressingMode::ZEROPAGE_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_LSR_NAME, NES_OP_LSR_ABSOLUTE, &NESCPU::ExecuteOpLSR, NESCPUOpAddressingMode::ABSOLUTE, 6);
	NESCPU::RegisterOpMapping(NES_OP_LSR_NAME, NES_OP_LSR_ABSOLUTE_X, &NESCPU::ExecuteOpLSR, NESCPUOpAddressingMode::ABSOLUTE_X, 7);

	// NOP
	NESCPU::RegisterOpMapping(NES_OP_NOP_NAME, NES_OP_NOP_IMPLIED, &NESCPU::ExecuteOpNOP, NESCPUOpAddressingMode::IMPLIED, 2);

	// ORA
	NESCPU::RegisterOpMapping(NES_OP_ORA_NAME, NES_OP_ORA_IMMEDIATE, &NESCPU::ExecuteOpORA, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_ORA_NAME, NES_OP_ORA_ABSOLUTE, &NESCPU::ExecuteOpORA, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_ORA_NAME, NES_OP_ORA_ABSOLUTE_X, &NESCPU::ExecuteOpORA, NESCPUOpAddressingMode::ABSOLUTE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_ORA_NAME, NES_OP_ORA_ABSOLUTE_Y, &NESCPU::ExecuteOpORA, NESCPUOpAddressingMode::ABSOLUTE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_ORA_NAME, NES_OP_ORA_ZEROPAGE, &NESCPU::ExecuteOpORA, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_ORA_NAME, NES_OP_ORA_ZEROPAGE_X, &NESCPU::ExecuteOpORA, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_ORA_NAME, NES_OP_ORA_INDIRECT_X, &NESCPU::ExecuteOpORA, NESCPUOpAddressingMode::INDIRECT_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_ORA_NAME, NES_OP_ORA_INDIRECT_Y, &NESCPU::ExecuteOpORA, NESCPUOpAddressingMode::INDIRECT_Y, 5);

	// PHA
	NESCPU::RegisterOpMapping(NES_OP_PHA_NAME, NES_OP_PHA_IMPLIED, &NESCPU::ExecuteOpPHA, NESCPUOpAddressingMode::IMPLIED, 3);

	// PHP
	NESCPU::RegisterOpMapping(NES_OP_PHP_NAME, NES_OP_PHP_IMPLIED, &NESCPU::ExecuteOpPHP, NESCPUOpAddressingMode::IMPLIED, 3);

	// PLA
	NESCPU::RegisterOpMapping(NES_OP_PLA_NAME, NES_OP_PLA_IMPLIED, &NESCPU::ExecuteOpPLA, NESCPUOpAddressingMode::IMPLIED, 4);

	// PLP
	NESCPU::RegisterOpMapping(NES_OP_PLP_NAME, NES_OP_PLP_IMPLIED, &NESCPU::ExecuteOpPLP, NESCPUOpAddressingMode::IMPLIED, 4);

	// ROL
	NESCPU::RegisterOpMapping(NES_OP_ROL_NAME, NES_OP_ROL_ACCUMULATOR, &NESCPU::ExecuteOpROL, NESCPUOpAddressingMode::ACCUMULATOR, 2);
	NESCPU::RegisterOpMapping(NES_OP_ROL_NAME, NES_OP_ROL_ZEROPAGE, &NESCPU::ExecuteOpROL, NESCPUOpAddressingMode::ZEROPAGE, 5);
	NESCPU::RegisterOpMapping(NES_OP_ROL_NAME, NES_OP_ROL_ZEROPAGE_X, &NESCPU::ExecuteOpROL, NESCPUOpAddressingMode::ZEROPAGE_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_ROL_NAME, NES_OP_ROL_ABSOLUTE, &NESCPU::ExecuteOpROL, NESCPUOpAddressingMode::ABSOLUTE, 6);
	NESCPU::RegisterOpMapping(NES_OP_ROL_NAME, NES_OP_ROL_ABSOLUTE_X, &NESCPU::ExecuteOpROL, NESCPUOpAddressingMode::ABSOLUTE_X, 7);

	// ROR
	NESCPU::RegisterOpMapping(NES_OP_ROR_NAME, NES_OP_ROR_ACCUMULATOR, &NESCPU::ExecuteOpROR, NESCPUOpAddressingMode::ACCUMULATOR, 2);
	NESCPU::RegisterOpMapping(NES_OP_ROR_NAME, NES_OP_ROR_ZEROPAGE, &NESCPU::ExecuteOpROR, NESCPUOpAddressingMode::ZEROPAGE, 5);
	NESCPU::RegisterOpMapping(NES_OP_ROR_NAME, NES_OP_ROR_ZEROPAGE_X, &NESCPU::ExecuteOpROR, NESCPUOpAddressingMode::ZEROPAGE_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_ROR_NAME, NES_OP_ROR_ABSOLUTE, &NESCPU::ExecuteOpROR, NESCPUOpAddressingMode::ABSOLUTE, 6);
	NESCPU::RegisterOpMapping(NES_OP_ROR_NAME, NES_OP_ROR_ABSOLUTE_X, &NESCPU::ExecuteOpROR, NESCPUOpAddressingMode::ABSOLUTE_X, 7);

	// RTI
	NESCPU::RegisterOpMapping(NES_OP_RTI_NAME, NES_OP_RTI_IMPLIED, &NESCPU::ExecuteOpRTI, NESCPUOpAddressingMode::IMPLIED, 6);

	// RTS
	NESCPU::RegisterOpMapping(NES_OP_RTS_NAME, NES_OP_RTS_IMPLIED, &NESCPU::ExecuteOpRTS, NESCPUOpAddressingMode::IMPLIED, 6);

	// SBC
	NESCPU::RegisterOpMapping(NES_OP_SBC_NAME, NES_OP_SBC_IMMEDIATE, &NESCPU::ExecuteOpSBC, NESCPUOpAddressingMode::IMMEDIATE, 2);
	NESCPU::RegisterOpMapping(NES_OP_SBC_NAME, NES_OP_SBC_ABSOLUTE, &NESCPU::ExecuteOpSBC, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_SBC_NAME, NES_OP_SBC_ABSOLUTE_X, &NESCPU::ExecuteOpSBC, NESCPUOpAddressingMode::ABSOLUTE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_SBC_NAME, NES_OP_SBC_ABSOLUTE_Y, &NESCPU::ExecuteOpSBC, NESCPUOpAddressingMode::ABSOLUTE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_SBC_NAME, NES_OP_SBC_ZEROPAGE, &NESCPU::ExecuteOpSBC, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_SBC_NAME, NES_OP_SBC_ZEROPAGE_X, &NESCPU::ExecuteOpSBC, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_SBC_NAME, NES_OP_SBC_INDIRECT_X, &NESCPU::ExecuteOpSBC, NESCPUOpAddressingMode::INDIRECT_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_SBC_NAME, NES_OP_SBC_INDIRECT_Y, &NESCPU::ExecuteOpSBC, NESCPUOpAddressingMode::INDIRECT_Y, 5);

	// SEC
	NESCPU::RegisterOpMapping(NES_OP_SEC_NAME, NES_OP_SEC_IMPLIED, &NESCPU::ExecuteOpSEC, NESCPUOpAddressingMode::IMPLIED, 2);

	// SED
	NESCPU::RegisterOpMapping(NES_OP_SED_NAME, NES_OP_SED_IMPLIED, &NESCPU::ExecuteOpSED, NESCPUOpAddressingMode::IMPLIED, 2);

	// SEI
	NESCPU::RegisterOpMapping(NES_OP_SEI_NAME, NES_OP_SEI_IMPLIED, &NESCPU::ExecuteOpSEI, NESCPUOpAddressingMode::IMPLIED, 2);

	// STA
	NESCPU::RegisterOpMapping(NES_OP_STA_NAME, NES_OP_STA_ABSOLUTE, &NESCPU::ExecuteOpSTA, NESCPUOpAddressingMode::ABSOLUTE, 4);
	NESCPU::RegisterOpMapping(NES_OP_STA_NAME, NES_OP_STA_ABSOLUTE_X, &NESCPU::ExecuteOpSTA, NESCPUOpAddressingMode::ABSOLUTE_X, 5);
	NESCPU::RegisterOpMapping(NES_OP_STA_NAME, NES_OP_STA_ABSOLUTE_Y, &NESCPU::ExecuteOpSTA, NESCPUOpAddressingMode::ABSOLUTE_Y, 5);
	NESCPU::RegisterOpMapping(NES_OP_STA_NAME, NES_OP_STA_ZEROPAGE, &NESCPU::ExecuteOpSTA, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_STA_NAME, NES_OP_STA_ZEROPAGE_X, &NESCPU::ExecuteOpSTA, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_STA_NAME, NES_OP_STA_INDIRECT_X, &NESCPU::ExecuteOpSTA, NESCPUOpAddressingMode::INDIRECT_X, 6);
	NESCPU::RegisterOpMapping(NES_OP_STA_NAME, NES_OP_STA_INDIRECT_Y, &NESCPU::ExecuteOpSTA, NESCPUOpAddressingMode::INDIRECT_Y, 6);

	// STX
	NESCPU::RegisterOpMapping(NES_OP_STX_NAME, NES_OP_STX_ZEROPAGE, &NESCPU::ExecuteOpSTX, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_STX_NAME, NES_OP_STX_ZEROPAGE_Y, &NESCPU::ExecuteOpSTX, NESCPUOpAddressingMode::ZEROPAGE_Y, 4);
	NESCPU::RegisterOpMapping(NES_OP_STX_NAME, NES_OP_STX_ABSOLUTE, &NESCPU::ExecuteOpSTX, NESCPUOpAddressingMode::ABSOLUTE, 4);

	// STY
	NESCPU::RegisterOpMapping(NES_OP_STY_NAME, NES_OP_STY_ZEROPAGE, &NESCPU::ExecuteOpSTY, NESCPUOpAddressingMode::ZEROPAGE, 3);
	NESCPU::RegisterOpMapping(NES_OP_STY_NAME, NES_OP_STY_ZEROPAGE_X, &NESCPU::ExecuteOpSTY, NESCPUOpAddressingMode::ZEROPAGE_X, 4);
	NESCPU::RegisterOpMapping(NES_OP_STY_NAME, NES_OP_STY_ABSOLUTE, &NESCPU::ExecuteOpSTY, NESCPUOpAddressingMode::ABSOLUTE, 4);

	// TAX
	NESCPU::RegisterOpMapping(NES_OP_TAX_NAME, NES_OP_TAX_IMPLIED, &NESCPU::ExecuteOpTAX, NESCPUOpAddressingMode::IMPLIED, 2);

	// TAY
	NESCPU::RegisterOpMapping(NES_OP_TAY_NAME, NES_OP_TAY_IMPLIED, &NESCPU::ExecuteOpTAY, NESCPUOpAddressingMode::IMPLIED, 2);

	// TSX
	NESCPU::RegisterOpMapping(NES_OP_TSX_NAME, NES_OP_TSX_IMPLIED, &NESCPU::ExecuteOpTSX, NESCPUOpAddressingMode::IMPLIED, 2);

	// TXA
	NESCPU::RegisterOpMapping(NES_OP_TXA_NAME, NES_OP_TXA_IMPLIED, &NESCPU::ExecuteOpTXA, NESCPUOpAddressingMode::IMPLIED, 2);

	// TXS
	NESCPU::RegisterOpMapping(NES_OP_TXS_NAME, NES_OP_TXS_IMPLIED, &NESCPU::ExecuteOpTXS, NESCPUOpAddressingMode::IMPLIED, 2);

	// TYA
	NESCPU::RegisterOpMapping(NES_OP_TYA_NAME, NES_OP_TYA_IMPLIED, &NESCPU::ExecuteOpTYA, NESCPUOpAddressingMode::IMPLIED, 2);
}

NESCPU::NESCPU(NESCPUMemoryMapper& mem) :
mem_(mem),
intReset_(true),
intNmi_(false),
intIrq_(false),
elapsedCycles_(0)
{
}


NESCPU::~NESCPU()
{
}


void NESCPU::SetInterrupt(NESCPUInterruptType interrupt)
{
	switch (interrupt)
	{
	case NESCPUInterruptType::RESET:
		intReset_ = true;
		break;

	case NESCPUInterruptType::NMI:
		intNmi_ = true;
		break;

	case NESCPUInterruptType::IRQ:
		intIrq_ = true;
		break;
	}
}


void NESCPU::Run(unsigned int numCycles)
{
	// @TODO Need to compensate for going over the specified amount of
	// numCycles for the next Run()
	while (elapsedCycles_ < numCycles)
	{
		// Check for interrupts. Return if it's a reset.
		const auto handledInt = HandleInterrupts();
		if (handledInt == NESCPUInterruptType::RESET)
			return;

		// Execute next instruction.
		ExecuteNextOp();

		// Add 7 extra cycles if an interrupt was handled.
		elapsedCycles_ += currentOp_.opCycleCount + (handledInt != NESCPUInterruptType::NONE ? 7 : 0);
	}
}


std::string NESCPU::OpAsAsm(const std::string& opName, NESCPUOpAddressingMode addrMode, u16 val)
{
	std::ostringstream oss;
	oss << opName << " ";

	switch (addrMode)
	{
	case NESCPUOpAddressingMode::IMPLIED:
	case NESCPUOpAddressingMode::IMPLIED_BRK:
		break; // No operand in implied instructions.

	case NESCPUOpAddressingMode::ACCUMULATOR:
		oss << "A";
		break;

	case NESCPUOpAddressingMode::IMMEDIATE:
		oss << "#$" << std::hex << val;
		break;

	case NESCPUOpAddressingMode::ZEROPAGE:
		oss << "$" << std::hex << val;
		break;

	case NESCPUOpAddressingMode::ZEROPAGE_X:
		oss << "$" << std::hex << val << ",X";
		break;

	case NESCPUOpAddressingMode::ZEROPAGE_Y:
		oss << "$" << std::hex << val << ",Y";
		break;

	case NESCPUOpAddressingMode::RELATIVE:
	case NESCPUOpAddressingMode::ABSOLUTE:
		oss << "$" << std::hex << val;
		break;

	case NESCPUOpAddressingMode::ABSOLUTE_X:
		oss << "$" << std::hex << val << ",X";
		break;

	case NESCPUOpAddressingMode::ABSOLUTE_Y:
		oss << "$" << std::hex << val << ",Y";
		break;

	case NESCPUOpAddressingMode::INDIRECT:
		oss << "($" << std::hex << val << ")";
		break;

	case NESCPUOpAddressingMode::INDIRECT_X:
		oss << "($" << std::hex << val << ",X)";
		break;

	case NESCPUOpAddressingMode::INDIRECT_Y:
		oss << "($" << std::hex << val << "),Y";
		break;

	default:
		oss << "[???]";
		break;
	}

	return oss.str();
}


NESCPUOpAddressingMode NESCPU::GetOpAddressingMode(u8 op)
{
	const auto it = opInfos_.find(op);
	if (it == opInfos_.end())
		return NESCPUOpAddressingMode::UNKNOWN;

	return it->second.addrMode;
}


u16 NESCPU::GetOpSizeFromAddressingMode(NESCPUOpAddressingMode addrMode)
{
	switch (addrMode)
	{
	// 1 byte addressing modes.
	case NESCPUOpAddressingMode::ACCUMULATOR:
	case NESCPUOpAddressingMode::IMPLIED:
		return 1;

	// 2 byte addressing modes.
	case NESCPUOpAddressingMode::IMPLIED_BRK: // BRK has a padding byte.
	case NESCPUOpAddressingMode::IMMEDIATE:
	case NESCPUOpAddressingMode::RELATIVE:
	case NESCPUOpAddressingMode::ZEROPAGE:
	case NESCPUOpAddressingMode::ZEROPAGE_X:
	case NESCPUOpAddressingMode::ZEROPAGE_Y:
	case NESCPUOpAddressingMode::INDIRECT_X:
	case NESCPUOpAddressingMode::INDIRECT_Y:
		return 2;

	// 3 byte addressing modes.
	case NESCPUOpAddressingMode::ABSOLUTE:
	case NESCPUOpAddressingMode::ABSOLUTE_X:
	case NESCPUOpAddressingMode::ABSOLUTE_Y:
	case NESCPUOpAddressingMode::INDIRECT:
		return 3;

	// Unknown addressing mode.
	default:
		assert("Unknown addressing mode supplied!" && false);
		return 0;
	}
}


NESCPUInterruptType NESCPU::HandleInterrupts()
{
	auto handledInt = NESCPUInterruptType::NONE;

	// Handle interrupts while accounting for priority.
	// Only IRQ's check for the interrupt disable flag (I).
	if (intReset_)
	{
		handledInt = NESCPUInterruptType::RESET;

		UpdateRegPC(NESHelper::MemoryRead16(mem_, 0xFFFC));
		reg_.SP = 0xFF;
		intReset_ = false;
	}
	else if (intNmi_) // @TODO: Check for NMI Edge!
	{
		handledInt = NESCPUInterruptType::NMI;

		UpdateRegPC(NESHelper::MemoryRead16(mem_, 0xFFFA));
		intNmi_ = false;
	}
	else if (intIrq_ && !NESHelper::IsBitSet(reg_.P, NES_CPU_REG_P_I_BIT))
	{
		handledInt = NESCPUInterruptType::IRQ;

		UpdateRegPC(NESHelper::MemoryRead16(mem_, 0xFFFE));
		intIrq_ = false;
	}

	if (handledInt != NESCPUInterruptType::NONE)
	{
		// If it wasn't a reset, we need to push the next PC and status register (P).
		if (handledInt != NESCPUInterruptType::RESET)
		{
			StackPush16(reg_.PC + 1);
			StackPush8(reg_.P);
		}

		// Make sure the interrupt disable flag is set if we interrupted.
		NESHelper::SetBit(reg_.P, NES_CPU_REG_P_I_BIT);
	}

	return handledInt;
}


void NESCPU::ExecuteNextOp()
{
	try
	{
		// Get the next opcode.
		currentOp_ = NESCPUExecutingOpInfo(mem_.Read8(reg_.PC));
	}
	catch (const NESMemoryException&)
	{
		throw NESCPUExecutionException("Could not read the next opcode for program execution.", reg_);
	}

	// Locate the mapping for this opcode.
	const auto it = opInfos_.find(currentOp_.op);
	if (it == opInfos_.end())
	{
		std::cout << "Test status: $" << std::hex << +mem_.Read8(0x6000) << std::endl;
		std::cout << "$" << std::hex << +mem_.Read8(0x6001) << ", $" << std::hex << +mem_.Read8(0x6002) << ", $" << std::hex << +mem_.Read8(0x6003) << std::endl;

		std::ostringstream oss;
		oss << "Unknown opcode: 0x" << std::hex << +currentOp_.op;
		throw NESCPUExecutionException(oss.str(), reg_);
	}

	auto argInfo = ReadOpArgInfo(it->second.addrMode);
	currentOp_.opCycleCount = it->second.cycleCount;
	currentOp_.opChangedPC = false;

	// Execute opcode func.
	u16 val;
	if (argInfo.addrMode == NESCPUOpAddressingMode::IMMEDIATE)
		val = mem_.Read8(argInfo.argAddr);
	else if (argInfo.addrMode == NESCPUOpAddressingMode::ACCUMULATOR)
		val = 0;
	else
		val = argInfo.argAddr;

	//std::cout << "SP: $" << std::hex << +reg_.SP << ",  " <<  << std::endl;
	//std::cout << "stack: ";
	//for (u8 i = 0xFF; i >= 0 && i > reg_.SP; --i)
	//	std::cout << std::hex << +mem_.Read8(NES_CPU_STACK_START + i) << ", ";
	//std::cout << std::endl;

	if (elapsedCycles_ >= 550000)
		std::cout << "Cyc: " << elapsedCycles_ << ", Reg: " << reg_.ToString() << "\t Ins: " << OpAsAsm(it->second.opName, it->second.addrMode, val) << std::endl;

	(this->*it->second.opFunc)(argInfo);

	// Go to the next instruction.
	if (!currentOp_.opChangedPC)
		reg_.PC += GetOpSizeFromAddressingMode(it->second.addrMode);
}


NESCPUOpArgInfo NESCPU::ReadOpArgInfo(NESCPUOpAddressingMode addrMode)
{
	NESCPUOpArgInfo argInfo(addrMode);

	switch (argInfo.addrMode)
	{
	case NESCPUOpAddressingMode::ACCUMULATOR: // Instruction will need to read the register.
	case NESCPUOpAddressingMode::IMPLIED: // No operands for implied addr modes.
	case NESCPUOpAddressingMode::IMPLIED_BRK:
		break;

	case NESCPUOpAddressingMode::IMMEDIATE:
		argInfo.argAddr = reg_.PC + 1;
		break;

	case NESCPUOpAddressingMode::RELATIVE:
		// We add an extra 2 to the PC to cover the size of the rest of the instruction.
		// The offset is signed.
		argInfo.argAddr = reg_.PC + 2 + static_cast<s8>(mem_.Read8(reg_.PC + 1));
		break;

	case NESCPUOpAddressingMode::INDIRECT: // @TODO: Handle indirect addressing bug.
		argInfo.argAddr = NESHelper::MemoryRead16(mem_, reg_.PC + 1);
		break;

	case NESCPUOpAddressingMode::INDIRECT_X: // @TODO: Handle indirect addressing bug.
		argInfo.argAddr = NESHelper::MemoryRead16(mem_, (mem_.Read8(reg_.PC + 1) + reg_.X) & 0xFF);
		break;

	case NESCPUOpAddressingMode::INDIRECT_Y: // @TODO: Handle indirect addressing bug.
		argInfo.argAddr = NESHelper::MemoryRead16(mem_, mem_.Read8(reg_.PC + 1));
		argInfo.crossedPage = !NESHelper::IsInSamePage(argInfo.argAddr, argInfo.argAddr + reg_.Y);
		argInfo.argAddr += reg_.Y;
		break;

	case NESCPUOpAddressingMode::ABSOLUTE:
		argInfo.argAddr = NESHelper::MemoryRead16(mem_, reg_.PC + 1);
		break;

	case NESCPUOpAddressingMode::ABSOLUTE_X:
		argInfo.argAddr = NESHelper::MemoryRead16(mem_, reg_.PC + 1);
		argInfo.crossedPage = !NESHelper::IsInSamePage(argInfo.argAddr, argInfo.argAddr + reg_.X);
		argInfo.argAddr += reg_.X;
		break;

	case NESCPUOpAddressingMode::ABSOLUTE_Y:
		argInfo.argAddr = NESHelper::MemoryRead16(mem_, reg_.PC + 1);
		argInfo.crossedPage = !NESHelper::IsInSamePage(argInfo.argAddr, argInfo.argAddr + reg_.Y);
		argInfo.argAddr += reg_.Y;
		break;

	case NESCPUOpAddressingMode::ZEROPAGE:
		argInfo.argAddr = mem_.Read8(reg_.PC + 1);
		break;

	case NESCPUOpAddressingMode::ZEROPAGE_X:
		argInfo.argAddr = (mem_.Read8(reg_.PC + 1) + reg_.X) & 0xFF;
		break;

	case NESCPUOpAddressingMode::ZEROPAGE_Y:
		argInfo.argAddr = (mem_.Read8(reg_.PC + 1) + reg_.Y) & 0xFF;
		break;

	default:
		// Unhandled addressing mode!
		assert("Unknown addressing mode supplied!" && false);
		break;
	}

	return argInfo;
}


void NESCPU::WriteOpResult(NESCPUOpAddressingMode addrMode, u8 result)
{
	u16 addr;
	switch (addrMode)
	{
	case NESCPUOpAddressingMode::ACCUMULATOR:
		reg_.A = result; // Write to accumulator instead.
		return;

	case NESCPUOpAddressingMode::ABSOLUTE:
		addr = NESHelper::MemoryRead16(mem_, reg_.PC + 1);
		break;

	case NESCPUOpAddressingMode::ABSOLUTE_X:
		addr = NESHelper::MemoryRead16(mem_, reg_.PC + 1) + reg_.X;
		break;

	case NESCPUOpAddressingMode::ZEROPAGE:
		addr = mem_.Read8(reg_.PC + 1);
		break;

	case NESCPUOpAddressingMode::ZEROPAGE_X:
		addr = (mem_.Read8(reg_.PC + 1) + reg_.X) & 0xFF;
		break;

	default:
		// Unhandled addressing mode!
		assert("Unknown addressing mode supplied!" && false);
		return;
	}

	// Assume writing to main memory at addr.
	mem_.Write8(addr, result);
}


void NESCPU::ExecuteOpADC(NESCPUOpArgInfo& argInfo)
{
	// ADC takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);

	// A + M + C -> A, C
	// @NOTE: NES 6502 variant has no BCD mode.
	const u8 argVal = mem_.Read8(argInfo.argAddr);
	const u16 res = reg_.A + argVal + (NESHelper::IsBitSet(reg_.P, NES_CPU_REG_P_C_BIT) ? 1 : 0);

	// Check if the sign has changed due to overflow.
	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_V_BIT, ((~(reg_.A ^ argVal) & (reg_.A ^ res)) & 0x80) == 0x80);

	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, res > 0xFF);
	UpdateRegN(static_cast<u8>(res));
	reg_.A = static_cast<u8>(res);
}


void NESCPU::ExecuteOpAND(NESCPUOpArgInfo& argInfo)
{
	// AND takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);

	// A AND M -> A
	const u8 res = reg_.A & mem_.Read8(argInfo.argAddr);

	UpdateRegZ(res);
	UpdateRegN(res);
	reg_.A = res;
}


void NESCPU::ExecuteOpASL(NESCPUOpArgInfo& argInfo)
{
	// C <- [76543210] <- 0
	const u8 argVal = (argInfo.addrMode == NESCPUOpAddressingMode::ACCUMULATOR ? reg_.A : mem_.Read8(argInfo.argAddr));
	const u8 res = argVal << 1;
	WriteOpResult(argInfo.addrMode, res);

	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, (argVal & 0x80) == 0x80); // Set carry bit if bit 7 was originally 1.
	UpdateRegZ(res);
	UpdateRegN(res);
}


void NESCPU::ExecuteOpAsBranch(NESCPUOpArgInfo& argInfo, bool shouldBranch, int branchSamePageCycleExtra, int branchDiffPageCycleExtra)
{
	if (!shouldBranch)
		return;

	// Add extra cycles depending on whether or not the new PC will cross a page boundary.
	OpAddCycles(NESHelper::IsInSamePage(reg_.PC, argInfo.argAddr) ? branchSamePageCycleExtra : branchDiffPageCycleExtra);

	UpdateRegPC(argInfo.argAddr);
}


void NESCPU::ExecuteOpBIT(NESCPUOpArgInfo& argInfo)
{
	// A /\ M, M7 -> N, M6 -> V
	const u8 argVal = mem_.Read8(argInfo.argAddr);

	UpdateRegN(argVal);
	UpdateRegZ(argVal & reg_.A);
	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_V_BIT, (argVal & 0x40) == 0x40);
}


void NESCPU::ExecuteOpBRK(NESCPUOpArgInfo& argInfo)
{
	// Forced Interrupt PC + 2 toS P toS 
	StackPush16(reg_.PC + 2); // There is a padding byte after the opcode, hence the +2.
	StackPush8(reg_.P | 0x10);
	NESHelper::SetBit(reg_.P, NES_CPU_REG_P_I_BIT);

	UpdateRegPC(NESHelper::MemoryRead16(mem_, 0xFFFE));
}


void NESCPU::ExecuteOpCMP(NESCPUOpArgInfo& argInfo)
{
	// CMP takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);

	// A - M
	const u16 res = reg_.A - mem_.Read8(argInfo.argAddr);

	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, res < 0x100);
	UpdateRegN(static_cast<u8>(res));
	UpdateRegZ(res & 0xFF); // Check first 8-bits.
}


void NESCPU::ExecuteOpCPX(NESCPUOpArgInfo& argInfo)
{
	// X - M
	const u16 res = reg_.X - mem_.Read8(argInfo.argAddr);

	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, res < 0x100);
	UpdateRegN(static_cast<u8>(res));
	UpdateRegZ(res & 0xFF); // Check first 8-bits.
}


void NESCPU::ExecuteOpCPY(NESCPUOpArgInfo& argInfo)
{
	// Y - M
	const u16 res = reg_.Y - mem_.Read8(argInfo.argAddr);

	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, res < 0x100);
	UpdateRegN(static_cast<u8>(res));
	UpdateRegZ(res & 0xFF); // Check first 8-bits.
}


void NESCPU::ExecuteOpDEC(NESCPUOpArgInfo& argInfo)
{
	// M - 1 -> M
	const u8 res = mem_.Read8(argInfo.argAddr) - 1;
	WriteOpResult(argInfo.addrMode, res);

	UpdateRegN(res);
	UpdateRegZ(res);
}


void NESCPU::ExecuteOpEOR(NESCPUOpArgInfo& argInfo)
{
	// EOR takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);
	
	// A EOR M -> A
	const u8 res = reg_.A ^ mem_.Read8(argInfo.argAddr);

	UpdateRegN(res);
	UpdateRegZ(res);
	reg_.A = res;
}


void NESCPU::ExecuteOpINC(NESCPUOpArgInfo& argInfo)
{
	// M + 1 -> M
	const u8 res = mem_.Read8(argInfo.argAddr) + 1;
	WriteOpResult(argInfo.addrMode, res);

	UpdateRegN(res);
	UpdateRegZ(res);
}


void NESCPU::ExecuteOpLDA(NESCPUOpArgInfo& argInfo)
{
	// LDA takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);

	// M -> A
	const u8 argVal = mem_.Read8(argInfo.argAddr);

	UpdateRegN(argVal);
	UpdateRegZ(argVal);
	reg_.A = argVal;
}


void NESCPU::ExecuteOpLDX(NESCPUOpArgInfo& argInfo)
{
	// LDX takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);

	// M -> X
	const u8 argVal = mem_.Read8(argInfo.argAddr);

	UpdateRegN(argVal);
	UpdateRegZ(argVal);
	reg_.X = argVal;
}


void NESCPU::ExecuteOpLDY(NESCPUOpArgInfo& argInfo)
{
	// LDY takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);

	// M -> Y
	const u8 argVal = mem_.Read8(argInfo.argAddr);

	UpdateRegN(argVal);
	UpdateRegZ(argVal);
	reg_.Y = argVal;
}


void NESCPU::ExecuteOpLSR(NESCPUOpArgInfo& argInfo)
{
	// 0 -> [76543210] -> C
	const u8 argVal = (argInfo.addrMode == NESCPUOpAddressingMode::ACCUMULATOR ? reg_.A : mem_.Read8(argInfo.argAddr));
	const u8 res = argVal >> 1;
	WriteOpResult(argInfo.addrMode, res);

	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, (argVal & 1) == 1);
	UpdateRegZ(res);
	UpdateRegN(res);
}


void NESCPU::ExecuteOpORA(NESCPUOpArgInfo& argInfo)
{
	// ORA takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);

	// A OR M -> A
	const u8 res = mem_.Read8(argInfo.argAddr) | reg_.A;

	UpdateRegZ(res);
	UpdateRegN(res);
	reg_.A = res;
}


void NESCPU::ExecuteOpPLA(NESCPUOpArgInfo& argInfo)
{
	// A fromS.
	const u8 val = StackPull8();

	UpdateRegZ(val);
	UpdateRegN(val);
	reg_.A = val;
}


void NESCPU::ExecuteOpROL(NESCPUOpArgInfo& argInfo)
{
	// C <- [7654321] <- C
	const u8 argVal = (argInfo.addrMode == NESCPUOpAddressingMode::ACCUMULATOR ? reg_.A : mem_.Read8(argInfo.argAddr));
	const u16 res = (argVal << 1) | (NESHelper::IsBitSet(reg_.P, NES_CPU_REG_P_C_BIT) ? 1 : 0);
	WriteOpResult(argInfo.addrMode, static_cast<u8>(res));

	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, res > 0xFF);
	UpdateRegZ(static_cast<u8>(res));
	UpdateRegN(static_cast<u8>(res));
}


void NESCPU::ExecuteOpROR(NESCPUOpArgInfo& argInfo)
{
	// C -> [7654321] -> C
	const u8 argVal = (argInfo.addrMode == NESCPUOpAddressingMode::ACCUMULATOR ? reg_.A : mem_.Read8(argInfo.argAddr));
	const u16 res = argVal | (NESHelper::IsBitSet(reg_.P, NES_CPU_REG_P_C_BIT) ? 0x100 : 0);
	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, (res & 1) == 1);

	const u16 resShifted = res >> 1;
	WriteOpResult(argInfo.addrMode, static_cast<u8>(resShifted));

	UpdateRegZ(static_cast<u8>(resShifted));
	UpdateRegN(static_cast<u8>(resShifted));
}


void NESCPU::ExecuteOpSBC(NESCPUOpArgInfo& argInfo)
{
	// SBC takes 1 extra CPU cycle if a page boundary was crossed.
	if (argInfo.crossedPage)
		OpAddCycles(1);

	// A - M - C -> A
	const u8 argVal = mem_.Read8(argInfo.argAddr);
	const u16 res = reg_.A - argVal - 1 + (NESHelper::IsBitSet(reg_.P, NES_CPU_REG_P_C_BIT) ? 1 : 0);

	// Check if the sign has changed due to overflow.
	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_V_BIT, ((~(reg_.A ^ argVal) & (reg_.A ^ res)) & 0x80) == 0x80);

	NESHelper::EditBit(reg_.P, NES_CPU_REG_P_C_BIT, res < 0x100);
	UpdateRegN(static_cast<u8>(res));
	UpdateRegZ(static_cast<u8>(res));
	reg_.A = static_cast<u8>(res);
}