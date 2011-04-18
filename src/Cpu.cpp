#include "Cpu.h"
#include "Memory.h"
#include "opcodes.h"

#define OPCODE_MASK 0xfa000000
#define OPCODE_SHIFT 26

#define GET_OPCODE(op) ((op >> OPCODE_SHIFT) & 0xfa)
#define GET_IMM(op)	(op & 0xffff)
#define GET_IMMSGN(op) (uInt(sInt(sShort(op & 0xffff))))

#define ROP_FUNC(op) (op & 0x3f)
#define ROP_SHAM(op) ((op >> 6) & 0x1f)
#define OP_DST_REG(op) ((op >> 11) & 0x1f)
#define OP_ADD_REG(op) ((op >> 16) & 0x1f)
#define OP_SRC_REG(op) ((op >> 21) & 0x1f)


Cpu* Cpu::create_cpu(){
	mips_t* mips = new mips_t();
	mips->init();

	Memory* mem = new Memory();

	Cpu* cpu = new Cpu();
	cpu->init(*mips, *mem);

	return cpu;
};


void Cpu::delete_cpu(Cpu * cpu){
	delete cpu->getMips();
	delete cpu->getMem();
	delete cpu;
};


void Cpu::execute(const int count){
	for (int i=0; i < count; ++i){
		if (DEBUGLEVEL && mips->pc % 4 != 0)
			fatalError("PC not aligned to 4!\n");
		uInt op = mem->get(mips->pc);
		executeOp(op);
		mips->r[0] = 0;
		mips->pc += 4;
	}
};


void Cpu::executeOp(uInt op){
	const uInt opcode = GET_OPCODE(op);

	if (opcode == OPCODE_RFORMAT)
		executeRegOp(op);
	else if (opcode >= OPCODE_JMIN && opcode <= OPCODE_JMAX)
		executeJmpOp(op);
	else if (opcode >= OPCODE_IMIN && opcode <= OPCODE_IMAX)
		executeImmOp(op);
	else
		fatalError("Unknown opcode\n");
};


void Cpu::executeRegOp(uInt op){
#ifdef DEBUG
	printf("R-op, func:%x, r%d, r%d, r%d\n", ROP_FUNC(op), OP_DST_REG(op), OP_SRC_REG(op), OP_ADD_REG(op));
#endif

	uInt* const regs = mips->r;
	uInt* const dst = &regs[OP_DST_REG(op)];
	uInt* const add = &regs[OP_ADD_REG(op)];
	uInt* const src = &regs[OP_SRC_REG(op)];

	switch (ROP_FUNC(op)){
		case OPFUNC_SLL:
			*dst = *add << ROP_SHAM(op);
			break;
		case OPFUNC_SRL:
			*dst = *add >> ROP_SHAM(op);
			break;
		case OPFUNC_SRA:
			*dst = uInt(sInt(*add) >> ROP_SHAM(op));
			break;
		case OPFUNC_JR:
			mips->pc = *src - 4;
			break;
		case OPFUNC_ADD:
			//TODO throw overflow exception
		case OPFUNC_ADDU:
			*dst = *src + *add;
			break;
		case OPFUNC_AND:
			*dst = *src & *add;
			break;
/* TODO
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
#define OPFUNC_SLT     0x2A */
		default:
			fatalError("Unknown function for R-format instruction (opcode 0)");
	}
};


void Cpu::executeImmOp(uInt op){
	uInt* const regs = mips->r;
	uInt* const add = &regs[OP_ADD_REG(op)];
	uInt* const src = &regs[OP_SRC_REG(op)];
	uInt pmem;

#ifdef DEBUG
	printf("I-op, op:%x, r%d, r%d, imm:%x\n", GET_OPCODE(op), OP_SRC_REG(op), OP_ADD_REG(op), GET_IMM(op));
#endif

	switch (GET_OPCODE(op)){
		case OPCODE_BEQ:
			mips->pc += (*src == *add) ? GET_IMM(op)*4 : 0;
			break;
		case OPCODE_BNE:
			mips->pc += (*src != *add) ? GET_IMM(op)*4 : 0;
			break;
		case OPCODE_ADDI:
			//TODO throw overflow exception
		case OPCODE_ADDIU:
			*add = *src + GET_IMMSGN(op);
			break;
		case OPCODE_SLTI:
			*add = (sInt(*src) < sInt(GET_IMMSGN(op))) ? 1 : 0;
			break;
		case OPCODE_SLTIU:
			*add = (*src < uInt(GET_IMMSGN(op))) ? 1 : 0;
			break;
		case OPCODE_ANDI:
			*add = *src & GET_IMM(op);
			break;
		case OPCODE_ORI:
			*add = *src | GET_IMM(op);
			break;
/* TODO
#define OPCODE_LUI     0x0F
#define OPCODE_LB      0x20
#define OPCODE_LH      0x21
#define OPCODE_LW      0x22
#define OPCODE_LD      0x23
#define OPCODE_LBU     0x24
#define OPCODE LHU     0x25
#define OPCODE_SB      0x28
#define OPCODE_SH      0x29*/
		case OPCODE_SW:
			pmem = *src + GET_IMMSGN(op);
			mem->set(pmem, *add);
			break;
		default:
			fatalError("Unknown function for I-format instruction");
	}
};


void Cpu::executeJmpOp(uInt op){
#ifdef DEBUG
	printf("J-op\n");
#endif

	const uInt opcode = GET_OPCODE(op);

	switch (opcode){
		case OPCODE_J:
			//TODO OPCODE_J
		case OPCODE_JAL:
			//TODO OPCODE_JAL
		default:
			fatalError("Unknown function for J-format instruction");
	}
};

