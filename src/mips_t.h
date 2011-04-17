#ifndef MIPS_T_H_
#define MIPS_T_H_

#include "shared.h"

#define EXTRA_REGS 2


class mips_t {
public:
	uInt r[32] ALIGNED(64); // general regs
	union { // floating point regs
		float f[32] ALIGNED(64);
		double d[16] ALIGNED(64);
	};
	union {
		uInt regs[EXTRA_REGS];
		struct {
			uInt pc; // program counter
			uInt psw; // processor status word
		};
	} ALIGNED(64);


	void init(){
		// set general registers to zero
		for (int i=0; i < 32; ++i) r[i] = 0;

		// initialise other registers to zero...
		for (int i=0; i < EXTRA_REGS; ++i) regs[i] = 0;

		// initialise fp regs
		for (int i=0; i < 32; ++i) f[i] = 0.0f;
	};
};


#endif /* MIPS_T_H_ */
