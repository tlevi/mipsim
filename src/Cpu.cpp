#include "Cpu.h"
#include "Memory.h"
#include "opcodes.h"

#define OPCODE_MASK 0xfa000000
#define OPCODE_SHIFT 26

#define GET_OPCODE(op) ((op >> OPCODE_SHIFT) & 0xfa)

#define ROP_FUNC(op) (op & 0x3f)
#define ROP_SHAM(op) ((op >> 6) & 0x1f)
#define ROP_DST_REG(op) ((op >> 11) & 0x1f)
#define ROP_ADD_REG(op) ((op >> 16) & 0x1f)
#define ROP_SRC_REG(op) ((op >> 21) & 0x1f)


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
	printf("R-op, func:%x, r%d, r%d, r%d\n", ROP_FUNC(op), ROP_DST_REG(op), ROP_SRC_REG(op), ROP_ADD_REG(op));
#endif

	uInt* const regs = mips->r;
	uInt* const dst = &regs[ROP_DST_REG(op)];
	uInt* const add = &regs[ROP_ADD_REG(op)];
	uInt* const src = &regs[ROP_SRC_REG(op)];
	uInt bit;

	switch (ROP_FUNC(op)){
		case OPFUNC_SLL:
			*add = *src << ROP_SHAM(op);
			break;
		case OPFUNC_SRL:
			*add = *src >> ROP_SHAM(op);
			break;
		case OPFUNC_SRA:
			//TODO actually test this madness
			bit = *src >> 31;
			for (int i=0; i < ROP_SHAM(op); ++i)
				bit = (bit << 1) | (bit & 1);
			*add = (*src >> ROP_SHAM(op)) | bit;
			break;
		case OPFUNC_JR:
			mips->pc = *src - 4;
			break;
		case OPFUNC_ADD:
			//TODO throw overflow exception
		case OPFUNC_ADDU:
			*dst = *src + *add;
			break;
		default:
			fatalError("Unknown function for R-format instruction (opcode 0)");
	}
};


void Cpu::executeImmOp(uInt op){
#ifdef DEBUG
	printf("I-op\n");
#endif

	const uInt opcode = GET_OPCODE(op);

	switch (opcode){
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

