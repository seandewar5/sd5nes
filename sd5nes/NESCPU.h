#pragma once

#include "NESTypes.h"

#include "NESCPUOpConstants.h"
#include "NESMemoryConstants.h"
#include "NESMemory.h"
#include "NESPPU.h"

#include <unordered_map>

/**
* Struct containing the registers used by the NES CPU.
*/
struct NESCPURegisters
{
	/* The Program Counter (PC) */
	u16 PC;

	/* The Stack Pointer (SP) */
	u8 SP;

	/* Accumulator (A) and Index Registers X and Y */
	u8 A, X, Y;

	/* Processor Status (P) union - contains the bits in same address space. */
	union
	{
		/* Processor Status (P) */
		u8 P;

		u8 C : 1; /* Carry Flag (C) */
		u8 Z : 1; /* Zero Flag (Z) */
		u8 I : 1; /* Interrupt Disable (I) */
		u8 D : 1; /* Decimal Mode (D) */
		u8 B : 1; /* Break Command (B) */
		u8 PUnused : 1; /* Unused bit - should typically always be 1. */
		u8 V : 1; /* Overflow Flag (V) */
		u8 N : 1; /* Negative Flag (N) */
	};
};

/**
* Enum containing the different interrupts used by the NES CPU.
* Sorted by interrupt priority (0 = highest, 1 = medium, 2 = lowest).
*/
enum class NESCPUInterrupt : u8
{
	/* RESET interrupt - highest priority. */
	RESET = 0,

	/* NMI interrupt - medium priority. */
	NMI = 1,

	/* IRQ/BRK interrupt - lowest priority. */
	IRQBRK = 2
};

/**
* Enum containing the different addressing modes of operands.
*/
enum class NESCPUOpAddressingMode
{
	IMMEDIATE,
	ZEROPAGE,
	ZEROPAGE_X,
	ZEROPAGE_Y,
	ABSOLUTE,
	ABSOLUTE_X,
	ABSOLUTE_Y,
	INDIRECT,
	INDIRECT_X,
	INDIRECT_Y,
	ACCUMULATOR,
	RELATIVE,
	IMPLIED,
	UNKNOWN
};

/**
* Allows static constructor mimic to register opcode mappings.
*/
struct NESCPUStaticInit
{
	NESCPUStaticInit();
};

// This needs to be forward-declared so that we can typedef NESOpFuncPointer.
class NESCPU;

// Typedef for a basic opcode executing function.
typedef bool (NESCPU::*NESOpFuncPointer)();

/**
* Struct containing opcode function mapping info.
*/
struct NESCPUOpInfo
{
	NESOpFuncPointer opFunc;
	NESCPUOpAddressingMode addrMode;
	int cycleCount;

	NESCPUOpInfo(NESOpFuncPointer opFunc, NESCPUOpAddressingMode addrMode, int cycleCount) :
		opFunc(opFunc),
		addrMode(addrMode),
		cycleCount(cycleCount)
	{ }
};

/**
* Handles emulation of the NES's CPU memory + mirroring.
*/
class NESCPUMemory : public NESMemory
{
public:
	NESCPUMemory(NESPPURegisters& ppuReg);
	~NESCPUMemory();

	bool Write8(u16 addr, u8 val) override;
	bool Read8(u16 addr, u8* outVal) const override;

private:
	NESPPURegisters& ppuReg_;
};

/**
* Handles emulation of the 6502 2A03 CPU used in the NES.
*/
class NESCPU
{
	friend struct NESCPUStaticInit;

public:
	NESCPU(NESCPUMemory& mem);
	~NESCPU();

	// Resets the CPU.
	void Reset();

	// Gets the current opcode being executed.
	u8 GetCurrentOpcode() const;

private:
	// Allows init of static stuff such as registration of opcode mapp
	static NESCPUStaticInit staticInit_;

	// Contains mapped opcode info.
	static std::unordered_map<u8, NESCPUOpInfo> opInfos_;

	// Registers an opcode mapping.
	static void RegisterOpMapping(u8 op, NESOpFuncPointer opFunc, NESCPUOpAddressingMode addrMode, int cycleCount);

	// Gets the addressing mode of the specified opcode.
	static NESCPUOpAddressingMode GetOpAddressingMode(u8 op);

	// Edits outOpSize with the size of the opcode. Returns true on success and false on failure.
	// If false, or outOpSize is null, outOpSize is not modified.
	static bool GetOpSizeFromAddressingMode(NESCPUOpAddressingMode addrMode, int* outOpSize);

	NESCPURegisters reg_;
	NESCPUMemory& mem_;

	u8 currentOp_;
	std::unordered_map<u8, NESCPUOpInfo>::const_iterator currentOpMappingIt_;
	int currentOpCycleCount_;
	bool currentOpChangedPC_;

	// (Re-)Initializes the CPU.
	void Initialize();

	// Updates the Z register. Sets to 1 if val is zero. Sets to 0 otherwise.
	inline void UpdateRegZ(u8 val) { reg_.Z = (val == 0 ? 1 : 0); }

	// Updates the N register. Sets to the value of val's 7th bit (sign bit).
	inline void UpdateRegN(u8 val) { reg_.N = ((val >> 7) & 1); }

	// Updates the PC register. Sets PC to val. currentOpChangedPC_ is set to true so PC is not automatically changed afterwards.
	inline void UpdateRegPC(u16 val) { reg_.PC = val; currentOpChangedPC_ = true; }

	// Reads the value of the next op's immediate argument depending on its addressing mode.
	// Also checks if a page boundary was crossed.
	bool ReadOpArgValue(u8* outVal, bool* outCrossedPageBoundary);

	// Writes an op's result to the intended piece of memory / register.
	bool WriteOpResult(u8 result);

	// Executes the next opcode at the PC.
	bool ExecuteNextOp();

	// Push 8-bit value onto the stack.
	bool StackPush8(u8 val);

	// Push 16-bit value onto the stack.
	bool StackPush16(u16 val);

	// Pull 8-bit value from the stack.
	bool StackPull8(u8* outVal);

	// Pull 16-bit value from the stack.
	bool StackPull16(u16* outVal);

	// Executes the current op as a branch instruction if shouldBranch is true.
	// Adds 1 to the current op's cycle count if branched to same page, 2 if branched to a different page.
	bool ExecuteOpAsBranch(bool shouldBranch, int branchSamePageCycleExtra, int branchDiffPageCycleExtra);

	// Executes an interrupt of the specified type.
	bool ExecuteInterrupt(NESCPUInterrupt interruptType);

	/****************************************/
	/****** Instruction Implementation ******/
	/****************************************/

	// Execute Add with Carry (ADC).
	bool ExecuteOpADC();

	// Execute AND with Accumulator (AND).
	bool ExecuteOpAND();

	// Execute Shift Left One Bit (Memory or Accumulator) (ASL).
	bool ExecuteOpASL();

	// Execute Branch on Carry Clear (BCC).
	bool ExecuteOpBCC();

	// Execute Branch on Carry Set (BCS).
	bool ExecuteOpBCS();

	// Execute Branch on Result Zero (BEQ).
	bool ExecuteOpBEQ();

	// Execute Test Bits in Memory with Accumulator (BIT).
	bool ExecuteOpBIT();

	// Execute Branch on Result Minus (BMI).
	bool ExecuteOpBMI();

	// Execute Branch on Result Not Zero (BNE).
	bool ExecuteOpBNE();

	// Execute Branch on Result Plus (BPL).
	bool ExecuteOpBPL();

	// Execute Force Break (BRK).
	bool ExecuteOpBRK();

	// Execute Branch on Overflow Clear (BVC).
	bool ExecuteOpBVC();

	// Execute Branch on Overflow Set (BVS).
	bool ExecuteOpBVS();

	// Execute Clear Carry Flag (CLC).
	bool ExecuteOpCLC();

	// Execute Clear Decimal Mode (CLD).
	bool ExecuteOpCLD();

	// Execute Clear Interrupt Disable Bit (CLI).
	bool ExecuteOpCLI();

	// Execute Clear Overflow Flag (CLV).
	bool ExecuteOpCLV();

	// Execute Compare Memory and Accumulator (CMP).
	bool ExecuteOpCMP();

	// Execute Compare Memory and Index X (CPX).
	bool ExecuteOpCPX();

	// Execute Compare Memory and Index Y (CPY).
	bool ExecuteOpCPY();

	// Execute Decrement Memory by One (DEC).
	bool ExecuteOpDEC();

	// Execute Decrement Index X by One (DEX).
	bool ExecuteOpDEX();

	// Execute Decrement Index Y by One (DEY).
	bool ExecuteOpDEY();

	// Execute "Exclusive-Or" Memory with Accumulator (EOR).
	bool ExecuteOpEOR();

	// Execute Increment Memory by One (INC).
	bool ExecuteOpINC();

	// Execute Increment Index X by One (INX).
	bool ExecuteOpINX();

	// Execute Increment Index Y by One (INY).
	bool ExecuteOpINY();

	// Execute Jump to New Location (JMP).
	bool ExecuteOpJMP();

	// Execute Jump to New Location Saving Return Address (JSR).
	bool ExecuteOpJSR();

	// Execute Load Accumulator with Memory (LDA).
	bool ExecuteOpLDA();

	// Execute Load Index X with Memory (LDX).
	bool ExecuteOpLDX();

	// Execute Load Index Y with Memory (LDY).
	bool ExecuteOpLDY();

	// Execute Shift Right One Bit (Memory or Accumulator) (LSR).
	bool ExecuteOpLSR();

	// Execute No Operation (Do Nothing) (NOP).
	bool ExecuteOpNOP();

	// Execute "Or" Memory with Accumulator (ORA).
	bool ExecuteOpORA();

	// Execute Push Accumulator to Stack (PHA).
	bool ExecuteOpPHA();

	// Execute Push Processor Status to Stack (PHP).
	bool ExecuteOpPHP();

	// Execute Pull Accumulator from Stack (PLA).
	bool ExecuteOpPLA();

	// Execute Pull Processor Status from Stack (PLP).
	bool ExecuteOpPLP();

	// Execute Rotate One Bit Left (ROL).
	bool ExecuteOpROL();

	// Execute Rotate One Bit Right (ROR).
	bool ExecuteOpROR();

	// Execute Return from Interrupt (RTI).
	bool ExecuteOpRTI();

	// Execute Return from Subroutine (RTS).
	bool ExecuteOpRTS();

	// Execute Subtract Memory from Accumulator with Borrow (SBC).
	bool ExecuteOpSBC();

	// Execute Set Carry Flag (SEC).
	bool ExecuteOpSEC();

	// Execute Set Decimal Mode (SED).
	bool ExecuteOpSED();

	// Execute Set Interrupt Disable Status (SEI).
	bool ExecuteOpSEI();

	// Execute Store Accumulator in Memory (STA).
	bool ExecuteOpSTA();

	// Execute Store Index X in Memory (STX).
	bool ExecuteOpSTX();

	// Execute Store Index Y in Memory (STY).
	bool ExecuteOpSTY();

	// Execute Transfer Accumulator to Index Y (TAY).
	bool ExecuteOpTAY();

	// Execute Transfer Accumulator to Index X (TAX).
	bool ExecuteOpTAX();

	// Execute Transfer Stack Pointer to Index X (TSX).
	bool ExecuteOpTSX();

	// Execute Transfer Index X to Accumulator (TXA).
	bool ExecuteOpTXA();

	// Execute Transfer Index X to Stack Pointer (TXS).
	bool ExecuteOpTXS();

	// Execute Transfer Index Y to Accumulator (TYA).
	bool ExecuteOpTYA();
};