#ifndef CPU_H_
#define CPU_H_

#include "Memory.h"


#define EXTRA_REGS 2

class mips_t {
public:
	uInt r[32] ALIGNED(64); // general regs
	union {
		uInt regs[EXTRA_REGS];
		struct {
			uInt pc; // program counter
			uInt psw; // processor status word
		};
	} ALIGNED(64);
	union { // floating point regs
		float f[32] ALIGNED(64);
		double d[16] ALIGNED(64);
	};


	void init(){
		// set general registers to zero
		for (int i=0; i < 32; ++i) r[i] = 0;

		// initialise other registers to zero...
		for (int i=0; i < EXTRA_REGS; ++i) regs[i] = 0;

		// initialise fp regs
		for (int i=0; i < 32; ++i) f[i] = 0.0f;
	};
};


class Cpu{
public:
	void execute(const int count);

	void init(mips_t& mips, Memory& mem) { this->mips = &mips; this->mem = &mem; };
	mips_t* getMips() const { return this->mips; };
	Memory* getMem() const{ return this->mem; };

	Cpu() : mips(NULL), mem(NULL) {};

	static Cpu* create_cpu();
	static void delete_cpu(Cpu* cpu);

private:
	mips_t* mips;
	Memory* mem;

	void executeOp(uInt op);
	void executeAluOp(uInt op);
};


#endif /* CPU_H_ */
