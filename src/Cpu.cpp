#include "Cpu.h"
#include "Memory.h"
#include "opcodes.h"

#define OPCODE_MASK 0xfa000000
#define OPCODE_SHIFT 26

#define GET_OPCODE(op) ((op >> OPCODE_SHIFT) & 0xfa)
#define GET_IMM(op)	(op & 0xffff)
#define GET_IMMSGNEXT(op) (uInt(sInt(sShort(op & 0xffff))))
#define GET_IMMSGN(op) (sShort(op & 0xffff))

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


const void Cpu::execute(const int count){
	for (int i=0; i < count; ++i){
		if (mips->pc & 0x3) // checks pc % 4 == 0
			fatalError("PC not aligned to 4!\n");
		const uInt op = mem->get<uInt>(mips->pc);
		executeOp(op);
		mips->r[0] = 0;
		mips->pc += 4;
	}
};


const void Cpu::executeOp(const uInt op){
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


const void Cpu::executeRegOp(const uInt op){
#if DEBUGLEVEL > 2
	printf("R-op, func:%x, r%d, r%d, r%d\n", ROP_FUNC(op), OP_DST_REG(op), OP_SRC_REG(op), OP_ADD_REG(op));
#endif

	uInt* const dst = &mips->r[OP_DST_REG(op)];
	const uInt* const add = &mips->r[OP_ADD_REG(op)];
	const uInt* const src = &mips->r[OP_SRC_REG(op)];

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
		case OPFUNC_MFHI:
		case OPFUNC_MFLO:
		case OPFUNC_MULT:
		case OPFUNC_DIV:
		case OPFUNC_DIVU:
			fatalError("Unimplemented R-op\n");
			break;
		case OPFUNC_ADD:
			//TODO throw overflow exception
		case OPFUNC_ADDU:
			*dst = *src + *add;
			break;
		case OPFUNC_SUB:
			//TODO throw some exception
		case OPFUNC_SUBU:
			*dst = *src - *add;
			break;
		case OPFUNC_AND:
			*dst = *src & *add;
			break;
		case OPFUNC_OR:
			*dst = *src | *add;
			break;
		case OPFUNC_NOR:
			*dst = ~(*src | *add);
			break;
		case OPFUNC_SLT:
			*dst = (sInt(*src) < sInt(*add)) ? 1 : 0;
			break;
		case OPFUNC_SLTU:
			*dst = (*src < *add) ? 1 : 0;
			break;
		default:
			fatalError("Unknown function for R-format instruction (opcode 0)\n");
			break;
	}
};


const void Cpu::executeImmOp(const uInt op){
#if DEBUGLEVEL > 2
	printf("I-op, op:%x, r%d, r%d, imm:%x\n", GET_OPCODE(op), OP_SRC_REG(op), OP_ADD_REG(op), GET_IMM(op));
#endif

	uInt* const add = &mips->r[OP_ADD_REG(op)];
	const uInt* const src = &mips->r[OP_SRC_REG(op)];
	const uInt pmem = *src + GET_IMMSGN(op);

	switch (GET_OPCODE(op)){
		case OPCODE_BEQ:
			mips->pc += (*src == *add) ? GET_IMMSGN(op)*4 : 0;
			break;
		case OPCODE_BNE:
			mips->pc += (*src != *add) ? GET_IMMSGN(op)*4 : 0;
			break;
		case OPCODE_ADDI:
			//TODO throw overflow exception
		case OPCODE_ADDIU:
			*add = *src + GET_IMMSGN(op);
			break;
		case OPCODE_MFC:
			fatalError("Unimplemented I-op\n");
			break;
		case OPCODE_FP:
			fatalError("In executeImmOp, but found FP-op!\n");
			break;
		case OPCODE_SLTI:
			*add = (sInt(*src) < GET_IMMSGN(op)) ? 1 : 0;
			break;
		case OPCODE_SLTIU:
			*add = (*src < GET_IMMSGN(op)) ? 1 : 0;
			break;
		case OPCODE_ANDI:
			*add = *src & GET_IMM(op);
			break;
		case OPCODE_ORI:
			*add = *src | GET_IMM(op);
			break;
		case OPCODE_LUI:
			*add = GET_IMM(op);
			break;
		case OPCODE_LW:
			*add = mem->get<uInt>(pmem);
			break;
		case OPCODE_LD:
			fatalError("Unimplemented I-op\n");
			break;
		case OPCODE_LBU:
			*add = mem->get<uChar>(pmem);
			break;
		case OPCODE_LHU:
			*add = mem->get<uShort>(pmem);
			break;
		case OPCODE_SB:
			mem->set(pmem, uChar(*add));
			break;
		case OPCODE_SH:
			mem->set(pmem, uShort(*add));
			break;
		case OPCODE_LL:
			fatalError("Unimplemented I-op\n");
			break;
		case OPCODE_SW:
			mem->set(pmem, *add);
			break;
		default:
			fatalError("Unknown function for I-format instruction\n");
			break;
	}
};


const void Cpu::executeJmpOp(const uInt op){
#if DEBUGLEVEL > 2
	printf("J-op\n");
#endif

	switch (GET_OPCODE(op)){
		case OPCODE_J:
		case OPCODE_JAL:
			fatalError("Unimplemented J-op\n");
			break;
		default:
			fatalError("Unknown function for J-format instruction\n");
			break;
	}
};

