#pragma once

/* Invalid / Unknown opcode. */
#define NES_OP_INVALID 0xFF

/* ADC */
#define NES_OP_ADC_IMMEDIATE 0x69
#define NES_OP_ADC_ZEROPAGE 0x65
#define NES_OP_ADC_ZEROPAGE_X 0x75
#define NES_OP_ADC_ABSOLUTE 0x6D
#define NES_OP_ADC_ABSOLUTE_X 0x7D
#define NES_OP_ADC_ABSOLUTE_Y 0x79
#define NES_OP_ADC_INDIRECT_X 0x61
#define NES_OP_ADC_INDIRECT_Y 0x71

/* AND */
#define NES_OP_AND_IMMEDIATE 0x29
#define NES_OP_AND_ZEROPAGE 0x25
#define NES_OP_AND_ZEROPAGE_X 0x35
#define NES_OP_AND_ABSOLUTE 0x2D
#define NES_OP_AND_ABSOLUTE_X 0x3D
#define NES_OP_AND_ABSOLUTE_Y 0x39
#define NES_OP_AND_INDIRECT_X 0x21
#define NES_OP_AND_INDIRECT_Y 0x31

/* ASL */
#define NES_OP_ASL_ACCUMULATOR 0x0A
#define NES_OP_ASL_ZEROPAGE 0x06
#define NES_OP_ASL_ZEROPAGE_X 0x16
#define NES_OP_ASL_ABSOLUTE 0x0E
#define NES_OP_ASL_ABSOLUTE_X 0x1E

/* BCC */
#define NES_OP_BCC_RELATIVE 0x90

/* BCS */
#define NES_OP_BCS_RELATIVE 0xB0

/* BEQ */
#define NES_OP_BEQ_RELATIVE 0xF0

/* BIT */
#define NES_OP_BIT_ZEROPAGE 0x24
#define NES_OP_BIT_ABSOLUTE 0x2C

/* BMI */
#define NES_OP_BMI_RELATIVE 0x30

/* BNE */
#define NES_OP_BNE_RELATIVE 0xD0

/* BPL */
#define NES_OP_BPL_RELATIVE 0x10

/* BRK */
#define NES_OP_BRK_IMPLIED 0x00

/* BVC */
#define NES_OP_BVC_RELATIVE 0x50

/* BVS */
#define NES_OP_BVS_RELATIVE 0x70

/* CLC */
#define NES_OP_CLC_IMPLIED 0x18

/* CLD */
#define NES_OP_CLD_IMPLIED 0xD8

/* CLI */
#define NES_OP_CLI_IMPLIED 0x58

/* CLV */
#define NES_OP_CLV_IMPLIED 0xB8

/* CMP */
#define NES_OP_CMP_IMMEDIATE 0xC9
#define NES_OP_CMP_ZEROPAGE 0xC5
#define NES_OP_CMP_ZEROPAGE_X 0xD5
#define NES_OP_CMP_ABSOLUTE 0xCD
#define NES_OP_CMP_ABSOLUTE_X 0xDD
#define NES_OP_CMP_ABSOLUTE_Y 0xD9
#define NES_OP_CMP_INDIRECT_X 0xC1
#define NES_OP_CMP_INDIRECT_Y 0xD1

/* CPX */
#define NES_OP_CPX_IMMEDIATE 0xE0
#define NES_OP_CPX_ZEROPAGE 0xE4
#define NES_OP_CPX_ABSOLUTE 0xEC

/* CPY */
#define NES_OP_CPY_IMMEDIATE 0xC0
#define NES_OP_CPY_ZEROPAGE 0xC4
#define NES_OP_CPY_ABSOLUTE 0xCC

/* DEC */
#define NES_OP_DEC_ZEROPAGE 0xC6
#define NES_OP_DEC_ZEROPAGE_X 0xD6
#define NES_OP_DEC_ABSOLUTE 0xCE
#define NES_OP_DEC_ABSOLUTE_X 0xDE

/* DEX */
#define NES_OP_DEX_IMPLIED 0xCA

/* DEY */
#define NES_OP_DEY_IMPLIED 0x88

/* EOR */
#define NES_OP_EOR_IMMEDIATE 0x49
#define NES_OP_EOR_ZEROPAGE 0x45
#define NES_OP_EOR_ZEROPAGE_X 0x55
#define NES_OP_EOR_ABSOLUTE 0x40
#define NES_OP_EOR_ABSOLUTE_X 0x50
#define NES_OP_EOR_ABSOLUTE_Y 0x59
#define NES_OP_EOR_INDIRECT_X 0x41
#define NES_OP_EOR_INDIRECT_Y 0x51

/* INC */
#define NES_OP_INC_ZEROPAGE 0xE6
#define NES_OP_INC_ZEROPAGE_X 0xF6
#define NES_OP_INC_ABSOLUTE 0xEE
#define NES_OP_INC_ABSOLUTE_X 0xFE