#pragma once

/******************/
/*  Official ops  */
/******************/

/* ADC */
#define NES_OP_ADC_NAME "ADC"
#define NES_OP_ADC_IMMEDIATE 0x69
#define NES_OP_ADC_ZEROPAGE 0x65
#define NES_OP_ADC_ZEROPAGE_X 0x75
#define NES_OP_ADC_ABSOLUTE 0x6D
#define NES_OP_ADC_ABSOLUTE_X 0x7D
#define NES_OP_ADC_ABSOLUTE_Y 0x79
#define NES_OP_ADC_INDIRECT_X 0x61
#define NES_OP_ADC_INDIRECT_Y 0x71

/* AND */
#define NES_OP_AND_NAME "AND"
#define NES_OP_AND_IMMEDIATE 0x29
#define NES_OP_AND_ZEROPAGE 0x25
#define NES_OP_AND_ZEROPAGE_X 0x35
#define NES_OP_AND_ABSOLUTE 0x2D
#define NES_OP_AND_ABSOLUTE_X 0x3D
#define NES_OP_AND_ABSOLUTE_Y 0x39
#define NES_OP_AND_INDIRECT_X 0x21
#define NES_OP_AND_INDIRECT_Y 0x31

/* ASL */
#define NES_OP_ASL_NAME "ASL"
#define NES_OP_ASL_ACCUMULATOR 0x0A
#define NES_OP_ASL_ZEROPAGE 0x06
#define NES_OP_ASL_ZEROPAGE_X 0x16
#define NES_OP_ASL_ABSOLUTE 0x0E
#define NES_OP_ASL_ABSOLUTE_X 0x1E

/* BCC */
#define NES_OP_BCC_NAME "BCC"
#define NES_OP_BCC_RELATIVE 0x90

/* BCS */
#define NES_OP_BCS_NAME "BCS"
#define NES_OP_BCS_RELATIVE 0xB0

/* BEQ */
#define NES_OP_BEQ_NAME "BEQ"
#define NES_OP_BEQ_RELATIVE 0xF0

/* BIT */
#define NES_OP_BIT_NAME "BIT"
#define NES_OP_BIT_ZEROPAGE 0x24
#define NES_OP_BIT_ABSOLUTE 0x2C

/* BMI */
#define NES_OP_BMI_NAME "BMI"
#define NES_OP_BMI_RELATIVE 0x30

/* BNE */
#define NES_OP_BNE_NAME "BNE"
#define NES_OP_BNE_RELATIVE 0xD0

/* BPL */
#define NES_OP_BPL_NAME "BPL"
#define NES_OP_BPL_RELATIVE 0x10

/* BRK */
#define NES_OP_BRK_NAME "BRK"
#define NES_OP_BRK_IMPLIED 0x00

/* BVC */
#define NES_OP_BVC_NAME "BVC"
#define NES_OP_BVC_RELATIVE 0x50

/* BVS */
#define NES_OP_BVS_NAME "BVS"
#define NES_OP_BVS_RELATIVE 0x70

/* CLC */
#define NES_OP_CLC_NAME "CLC"
#define NES_OP_CLC_IMPLIED 0x18

/* CLD */
#define NES_OP_CLD_NAME "CLD"
#define NES_OP_CLD_IMPLIED 0xD8

/* CLI */
#define NES_OP_CLI_NAME "CLI"
#define NES_OP_CLI_IMPLIED 0x58

/* CLV */
#define NES_OP_CLV_NAME "CLV"
#define NES_OP_CLV_IMPLIED 0xB8

/* CMP */
#define NES_OP_CMP_NAME "CMP"
#define NES_OP_CMP_IMMEDIATE 0xC9
#define NES_OP_CMP_ZEROPAGE 0xC5
#define NES_OP_CMP_ZEROPAGE_X 0xD5
#define NES_OP_CMP_ABSOLUTE 0xCD
#define NES_OP_CMP_ABSOLUTE_X 0xDD
#define NES_OP_CMP_ABSOLUTE_Y 0xD9
#define NES_OP_CMP_INDIRECT_X 0xC1
#define NES_OP_CMP_INDIRECT_Y 0xD1

/* CPX */
#define NES_OP_CPX_NAME "CPX"
#define NES_OP_CPX_IMMEDIATE 0xE0
#define NES_OP_CPX_ZEROPAGE 0xE4
#define NES_OP_CPX_ABSOLUTE 0xEC

/* CPY */
#define NES_OP_CPY_NAME "CPY"
#define NES_OP_CPY_IMMEDIATE 0xC0
#define NES_OP_CPY_ZEROPAGE 0xC4
#define NES_OP_CPY_ABSOLUTE 0xCC

/* DEC */
#define NES_OP_DEC_NAME "DEC"
#define NES_OP_DEC_ZEROPAGE 0xC6
#define NES_OP_DEC_ZEROPAGE_X 0xD6
#define NES_OP_DEC_ABSOLUTE 0xCE
#define NES_OP_DEC_ABSOLUTE_X 0xDE

/* DEX */
#define NES_OP_DEX_NAME "DEX"
#define NES_OP_DEX_IMPLIED 0xCA

/* DEY */
#define NES_OP_DEY_NAME "DEY"
#define NES_OP_DEY_IMPLIED 0x88

/* EOR */
#define NES_OP_EOR_NAME "EOR"
#define NES_OP_EOR_IMMEDIATE 0x49
#define NES_OP_EOR_ZEROPAGE 0x45
#define NES_OP_EOR_ZEROPAGE_X 0x55
#define NES_OP_EOR_ABSOLUTE 0x4D
#define NES_OP_EOR_ABSOLUTE_X 0x5D
#define NES_OP_EOR_ABSOLUTE_Y 0x59
#define NES_OP_EOR_INDIRECT_X 0x41
#define NES_OP_EOR_INDIRECT_Y 0x51

/* INC */
#define NES_OP_INC_NAME "INC"
#define NES_OP_INC_ZEROPAGE 0xE6
#define NES_OP_INC_ZEROPAGE_X 0xF6
#define NES_OP_INC_ABSOLUTE 0xEE
#define NES_OP_INC_ABSOLUTE_X 0xFE

/* INX */
#define NES_OP_INX_NAME "INX"
#define NES_OP_INX_IMPLIED 0xE8

/* INY */
#define NES_OP_INY_NAME "INY"
#define NES_OP_INY_IMPLIED 0xC8

/* JMP */
#define NES_OP_JMP_NAME "JMP"
#define NES_OP_JMP_ABSOLUTE 0x4C
#define NES_OP_JMP_INDIRECT 0x6C

/* JSR */
#define NES_OP_JSR_NAME "JSR"
#define NES_OP_JSR_ABSOLUTE 0x20

/* LDA */
#define NES_OP_LDA_NAME "LDA"
#define NES_OP_LDA_IMMEDIATE 0xA9
#define NES_OP_LDA_ZEROPAGE 0xA5
#define NES_OP_LDA_ZEROPAGE_X 0xB5
#define NES_OP_LDA_ABSOLUTE 0xAD
#define NES_OP_LDA_ABSOLUTE_X 0xBD
#define NES_OP_LDA_ABSOLUTE_Y 0xB9
#define NES_OP_LDA_INDIRECT_X 0xA1
#define NES_OP_LDA_INDIRECT_Y 0xB1

/* LDX */
#define NES_OP_LDX_NAME "LDX"
#define NES_OP_LDX_IMMEDIATE 0xA2
#define NES_OP_LDX_ZEROPAGE 0xA6
#define NES_OP_LDX_ZEROPAGE_Y 0xB6
#define NES_OP_LDX_ABSOLUTE 0xAE
#define NES_OP_LDX_ABSOLUTE_Y 0xBE

/* LDY */
#define NES_OP_LDY_NAME "LDY"
#define NES_OP_LDY_IMMEDIATE 0xA0
#define NES_OP_LDY_ZEROPAGE 0xA4
#define NES_OP_LDY_ZEROPAGE_X 0xB4
#define NES_OP_LDY_ABSOLUTE 0xAC
#define NES_OP_LDY_ABSOLUTE_X 0xBC

/* LSR */
#define NES_OP_LSR_NAME "LSR"
#define NES_OP_LSR_ACCUMULATOR 0x4A
#define NES_OP_LSR_ZEROPAGE 0x46
#define NES_OP_LSR_ZEROPAGE_X 0x56
#define NES_OP_LSR_ABSOLUTE 0x4E
#define NES_OP_LSR_ABSOLUTE_X 0x5E

/* NOP */
#define NES_OP_NOP_NAME "NOP"
#define NES_OP_NOP_IMPLIED 0xEA

/* ORA */
#define NES_OP_ORA_NAME "ORA"
#define NES_OP_ORA_IMMEDIATE 0x09
#define NES_OP_ORA_ZEROPAGE 0x05
#define NES_OP_ORA_ZEROPAGE_X 0x15
#define NES_OP_ORA_ABSOLUTE 0x0D
#define NES_OP_ORA_ABSOLUTE_X 0x1D
#define NES_OP_ORA_ABSOLUTE_Y 0x19
#define NES_OP_ORA_INDIRECT_X 0x01
#define NES_OP_ORA_INDIRECT_Y 0x11

/* PHA */
#define NES_OP_PHA_NAME "PHA"
#define NES_OP_PHA_IMPLIED 0x48

/* PHP */
#define NES_OP_PHP_NAME "PHP"
#define NES_OP_PHP_IMPLIED 0x08

/* PLA */
#define NES_OP_PLA_NAME "PLA"
#define NES_OP_PLA_IMPLIED 0x68

/* PLP */
#define NES_OP_PLP_NAME "PLP"
#define NES_OP_PLP_IMPLIED 0x28

/* ROL */
#define NES_OP_ROL_NAME "ROL"
#define NES_OP_ROL_ACCUMULATOR 0x2A
#define NES_OP_ROL_ZEROPAGE 0x26
#define NES_OP_ROL_ZEROPAGE_X 0x36
#define NES_OP_ROL_ABSOLUTE 0x2E
#define NES_OP_ROL_ABSOLUTE_X 0x3E

/* ROR */
#define NES_OP_ROR_NAME "ROR"
#define NES_OP_ROR_ACCUMULATOR 0x6A
#define NES_OP_ROR_ZEROPAGE 0x66
#define NES_OP_ROR_ZEROPAGE_X 0x76
#define NES_OP_ROR_ABSOLUTE 0x6E
#define NES_OP_ROR_ABSOLUTE_X 0x7E

/* RTI */
#define NES_OP_RTI_NAME "RTI"
#define NES_OP_RTI_IMPLIED 0x40

/* RTS */
#define NES_OP_RTS_NAME "RTS"
#define NES_OP_RTS_IMPLIED 0x60

/* SBC */
#define NES_OP_SBC_NAME "SBC"
#define NES_OP_SBC_IMMEDIATE 0xE9
#define NES_OP_SBC_ZEROPAGE 0xE5
#define NES_OP_SBC_ZEROPAGE_X 0xF5
#define NES_OP_SBC_ABSOLUTE 0xED
#define NES_OP_SBC_ABSOLUTE_X 0xFD
#define NES_OP_SBC_ABSOLUTE_Y 0xF9
#define NES_OP_SBC_INDIRECT_X 0xE1
#define NES_OP_SBC_INDIRECT_Y 0xF1

/* SEC */
#define NES_OP_SEC_NAME "SEC"
#define NES_OP_SEC_IMPLIED 0x38

/* SED */
#define NES_OP_SED_NAME "SED"
#define NES_OP_SED_IMPLIED 0xF8

/* SEI */
#define NES_OP_SEI_NAME "SEI"
#define NES_OP_SEI_IMPLIED 0x78

/* STA */
#define NES_OP_STA_NAME "STA"
#define NES_OP_STA_ZEROPAGE 0x85
#define NES_OP_STA_ZEROPAGE_X 0x95
#define NES_OP_STA_ABSOLUTE 0x8D
#define NES_OP_STA_ABSOLUTE_X 0x9D
#define NES_OP_STA_ABSOLUTE_Y 0x99
#define NES_OP_STA_INDIRECT_X 0x81
#define NES_OP_STA_INDIRECT_Y 0x91

/* STX */
#define NES_OP_STX_NAME "STX"
#define NES_OP_STX_ZEROPAGE 0x86
#define NES_OP_STX_ZEROPAGE_Y 0x96
#define NES_OP_STX_ABSOLUTE 0x8E

/* STY */
#define NES_OP_STY_NAME "STY"
#define NES_OP_STY_ZEROPAGE 0x84
#define NES_OP_STY_ZEROPAGE_X 0x94
#define NES_OP_STY_ABSOLUTE 0x8C

/* TAX */
#define NES_OP_TAX_NAME "TAX"
#define NES_OP_TAX_IMPLIED 0xAA

/* TAY */
#define NES_OP_TAY_NAME "TAY"
#define NES_OP_TAY_IMPLIED 0xA8

/* TSX */
#define NES_OP_TSX_NAME "TSX"
#define NES_OP_TSX_IMPLIED 0xBA

/* TXA */
#define NES_OP_TXA_NAME "TXA"
#define NES_OP_TXA_IMPLIED 0x8A

/* TXS */
#define NES_OP_TXS_NAME "TXS"
#define NES_OP_TXS_IMPLIED 0x9A

/* TYA */
#define NES_OP_TYA_NAME "TYA"
#define NES_OP_TYA_IMPLIED 0x98

/******************/
/* Unofficial ops */
/******************/

/* ANC */
#define NES_OP_ANC_NAME "ANC"
#define NES_OP_ANC_IMMEDIATE1 0x0B
#define NES_OP_ANC_IMMEDIATE2 0x2B

/* SAX */
#define NES_OP_SAX_NAME "SAX"
#define NES_OP_SAX_ZEROPAGE 0x87
#define NES_OP_SAX_ZEROPAGE_Y 0x97
#define NES_OP_SAX_INDIRECT_X 0x83
#define NES_OP_SAX_ABSOLUTE 0x8F

/* ARR */
#define NES_OP_ARR_NAME "ARR"
#define NES_OP_ARR_IMMEDIATE 0x6B

/* ASR */
#define NES_OP_ASR_NAME "ASR"
#define NES_OP_ASR_IMMEDIATE 0x4B

/* AHX */
#define NES_OP_AHX_NAME "AHX"
#define NES_OP_AHX_ABSOLUTE_Y 0x9F
#define NES_OP_AHX_INDIRECT_Y 0x93

/* AXS */
#define NES_OP_AXS_NAME "AXS"
#define NES_OP_AXS_IMMEDIATE 0xCB

/* DCP */
#define NES_OP_DCP_NAME "DCP"
#define NES_OP_DCP_ZEROPAGE 0xC7
#define NES_OP_DCP_ZEROPAGE_X 0xD7
#define NES_OP_DCP_ABSOLUTE 0xCF
#define NES_OP_DCP_ABSOLUTE_X 0xDF
#define NES_OP_DCP_ABSOLUTE_Y 0xDB
#define NES_OP_DCP_INDIRECT_X 0xC3
#define NES_OP_DCP_INDIRECT_Y 0xD3

/* DOP (Double NOP) */
#define NES_OP_DOP_NAME "DOP"
#define NES_OP_DOP_ZEROPAGE1 0x04
#define NES_OP_DOP_ZEROPAGE_X1 0x14
#define NES_OP_DOP_ZEROPAGE_X2 0x34
#define NES_OP_DOP_ZEROPAGE2 0x44
#define NES_OP_DOP_ZEROPAGE_X3 0x54
#define NES_OP_DOP_ZEROPAGE3 0x64
#define NES_OP_DOP_ZEROPAGE_X4 0x74
#define NES_OP_DOP_IMMEDIATE1 0x80
#define NES_OP_DOP_IMMEDIATE2 0x82
#define NES_OP_DOP_IMMEDIATE3 0x89
#define NES_OP_DOP_IMMEDIATE4 0xC2
#define NES_OP_DOP_ZEROPAGE_X5 0xD4
#define NES_OP_DOP_IMMEDIATE5 0xE2
#define NES_OP_DOP_ZEROPAGE_X6 0xF4

/* ISC */
#define NES_OP_ISC_NAME "ISC"
#define NES_OP_ISC_ZEROPAGE 0xE7
#define NES_OP_ISC_ZEROPAGE_X 0xF7
#define NES_OP_ISC_ABSOLUTE 0xEF
#define NES_OP_ISC_ABSOLUTE_X 0xFF
#define NES_OP_ISC_ABSOLUTE_Y 0xFB
#define NES_OP_ISC_INDIRECT_X 0xE3
#define NES_OP_ISC_INDIRECT_Y 0xF3

/* KIL */
#define NES_OP_KIL_NAME "KIL"
#define NES_OP_KIL_IMPLIED1 0x02
#define NES_OP_KIL_IMPLIED2 0x12
#define NES_OP_KIL_IMPLIED3 0x22
#define NES_OP_KIL_IMPLIED4 0x32
#define NES_OP_KIL_IMPLIED5 0x42
#define NES_OP_KIL_IMPLIED6 0x52
#define NES_OP_KIL_IMPLIED7 0x62
#define NES_OP_KIL_IMPLIED8 0x72
#define NES_OP_KIL_IMPLIED9 0x92
#define NES_OP_KIL_IMPLIED10 0xB2
#define NES_OP_KIL_IMPLIED11 0xD2
#define NES_OP_KIL_IMPLIED12 0xF2

/* LAS */
#define NES_OP_LAS_NAME "LAS"
#define NES_OP_LAS_ABSOLUTE_Y 0xBB

/* LAX */
#define NES_OP_LAX_NAME "LAX"
#define NES_OP_LAX_ZEROPAGE 0xA7
#define NES_OP_LAX_ZEROPAGE_Y 0xB7
#define NES_OP_LAX_ABSOLUTE 0xAF
#define NES_OP_LAX_ABSOLUTE_Y 0xBF
#define NES_OP_LAX_INDIRECT_X 0xA3
#define NES_OP_LAX_INDIRECT_Y 0xB3
#define NES_OP_LAX_IMMEDIATE 0xAB

/* NOP (Unofficial) */
#define NES_OP_NOP_U_IMPLIED1 0x1A
#define NES_OP_NOP_U_IMPLIED2 0x3A
#define NES_OP_NOP_U_IMPLIED3 0x5A
#define NES_OP_NOP_U_IMPLIED4 0x7A
#define NES_OP_NOP_U_IMPLIED5 0xDA
#define NES_OP_NOP_U_IMPLIED6 0xFA

/* RLA */
#define NES_OP_RLA_NAME "RLA"
#define NES_OP_RLA_ZEROPAGE 0x27
#define NES_OP_RLA_ZEROPAGE_X 0x37
#define NES_OP_RLA_ABSOLUTE 0x2F
#define NES_OP_RLA_ABSOLUTE_X 0x3F
#define NES_OP_RLA_ABSOLUTE_Y 0x3B
#define NES_OP_RLA_INDIRECT_X 0x23
#define NES_OP_RLA_INDIRECT_Y 0x33

/* RRA */
#define NES_OP_RRA_NAME "RRA"
#define NES_OP_RRA_ZEROPAGE 0x67
#define NES_OP_RRA_ZEROPAGE_X 0x77
#define NES_OP_RRA_ABSOLUTE 0x6F
#define NES_OP_RRA_ABSOLUTE_X 0x7F
#define NES_OP_RRA_ABSOLUTE_Y 0x7B
#define NES_OP_RRA_INDIRECT_X 0x63
#define NES_OP_RRA_INDIRECT_Y 0x73

/* SBC (Unofficial) */
#define NES_OP_SBC_U_IMMEDIATE 0xEB

/* SLO */
#define NES_OP_SLO_NAME "SLO"
#define NES_OP_SLO_ZEROPAGE 0x07
#define NES_OP_SLO_ZEROPAGE_X 0x17
#define NES_OP_SLO_ABSOLUTE 0x0F
#define NES_OP_SLO_ABSOLUTE_X 0x1F
#define NES_OP_SLO_ABSOLUTE_Y 0x1B
#define NES_OP_SLO_INDIRECT_X 0x03
#define NES_OP_SLO_INDIRECT_Y 0x13

/* SRE */
#define NES_OP_SRE_NAME "SRE"
#define NES_OP_SRE_ZEROPAGE 0x47
#define NES_OP_SRE_ZEROPAGE_X 0x57
#define NES_OP_SRE_ABSOLUTE 0x4F
#define NES_OP_SRE_ABSOLUTE_X 0x5F
#define NES_OP_SRE_ABSOLUTE_Y 0x5B
#define NES_OP_SRE_INDIRECT_X 0x43
#define NES_OP_SRE_INDIRECT_Y 0x53

/* SHX */
#define NES_OP_SHX_NAME "SHX"
#define NES_OP_SHX_ABSOLUTE_Y 0x9E

/* SHY */
#define NES_OP_SHY_NAME "SHY"
#define NES_OP_SHY_ABSOLUTE_X 0x9C

/* TOP (Triple NOP) */
#define NES_OP_TOP_NAME "TOP"
#define NES_OP_TOP_ABSOLUTE 0x0C
#define NES_OP_TOP_ABSOLUTE_X1 0x1C
#define NES_OP_TOP_ABSOLUTE_X2 0x3C
#define NES_OP_TOP_ABSOLUTE_X3 0x5C
#define NES_OP_TOP_ABSOLUTE_X4 0x7C
#define NES_OP_TOP_ABSOLUTE_X5 0xDC
#define NES_OP_TOP_ABSOLUTE_X6 0xFC

/* XAA */
#define NES_OP_XAA_NAME "XAA"
#define NES_OP_XAA_IMMEDIATE 0x8B

/* TAS */
#define NES_OP_TAS_NAME "TAS"
#define NES_OP_TAS_ABSOLUTE_Y 0x9B