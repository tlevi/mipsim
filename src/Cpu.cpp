#include "Cpu.h"
#include "Memory.h"


#define OPCODE_MASK 0xfa000000
#define OPCODE_SHIFT 26

#define FUNC_ADD  0x20
#define FUNC_ADDU 0x21
#define FUNC_SLL  0x00

#define ROP_FUNC (op & 0x3f)
#define ROP_SHAM ((op >> 6) & 0x1f)
#define ROP_DST_REG ((op >> 11) & 0x1f)
#define ROP_ADD_REG ((op >> 16) & 0x1f)
#define ROP_SRC_REG ((op >> 21) & 0x1f)


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
		mips->pc += 4;
	}
};


void Cpu::executeOp(uInt op){
	const uInt opcode = (op >> OPCODE_SHIFT) & 0xfa;

	switch (opcode){
		case 0:
			executeAluOp(op);
			break;
		default:
			fatalError("Unknown opcode\n");
	};
};


void Cpu::executeAluOp(uInt op){
#ifdef DEBUG
	printf("R-op, func:%x, r%d, r%d, r%d\n", ROP_FUNC, ROP_DST_REG, ROP_SRC_REG, ROP_ADD_REG);
#endif

	uInt* const regs = mips->r;
	uInt* const dst = &regs[ROP_DST_REG];
	uInt* const add = &regs[ROP_ADD_REG];
	uInt* const src = &regs[ROP_SRC_REG];

	switch (ROP_FUNC){
		case FUNC_SLL:
			*dst = *src << *add;
			break;

		case FUNC_ADD:
			//TODO throw overflow exception
		case FUNC_ADDU:
			*dst = *src + *add;
			break;

		default:
			fatalError("Unknown function for R-format instruction (opcode 0)");
	}

	regs[0] = 0;
};

