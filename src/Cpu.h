#include "shared.h"


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
private:
	static mips_t* mips;

public:
	static void execute(const int count);

	static void setMips(mips_t& mips) { Cpu::mips = &mips; };
	static const mips_t* const getMips() { return Cpu::mips; };
};

