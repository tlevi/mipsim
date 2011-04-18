#ifndef OPCODES_H_
#define OPCODES_H_

#define OPCODE_RFORMAT 0x00

#define OPCODE_JMIN    0x02
#define OPCODE_J       0x02
#define OPCODE_JAL     0x03
#define OPCODE_JMAX    0x03

#define OPCODE_IMIN    0x04
#define OPCODE_BEQ     0x04
#define OPCODE_BNE     0x05
#define OPCODE_ADDI    0x08
#define OPCODE_ADDIU   0x09
#define OPCODE_MFC     0x10
#define OPCODE_FP      0x11
#define OPCODE_SLTI    0x0A
#define OPCODE_SLTIU   0x0B
#define OPCODE_ANDI    0x0C
#define OPCODE_ORI     0x0D
#define OPCODE_LUI     0x0F
#define OPCODE_LB      0x20
#define OPCODE_LH      0x21
#define OPCODE_LW      0x22
#define OPCODE_LD      0x23
#define OPCODE_LBU     0x24
#define OPCODE_LHU     0x25
#define OPCODE_SB      0x28
#define OPCODE_SH      0x29
#define OPCODE_LL      0x30
#define OPCODE_SW      0x2B
#define OPCODE_IMAX    0x2B

#define OPFUNC_MIN     0x00
#define OPFUNC_SLL     0x00
#define OPFUNC_SRL     0x02
#define OPFUNC_SRA     0x03
#define OPFUNC_JR      0x08
#define OPFUNC_MFHI    0x10
#define OPFUNC_MFLO    0x12
#define OPFUNC_MULT    0x18
#define OPFUNC_DIV     0x1A
#define OPFUNC_DIVU    0x1B
#define OPFUNC_ADD     0x20
#define OPFUNC_ADDU    0x21
#define OPFUNC_SUB     0x22
#define OPFUNC_SUBU    0x23
#define OPFUNC_AND     0x24
#define OPFUNC_OR      0x25
#define OPFUNC_NOR     0x27
#define OPFUNC_SLT     0x2A
#define OPFUNC_SLTU    0x2B
#define OPFUNC_MAX     0x2B

#endif /* OPCODES_H_ */
