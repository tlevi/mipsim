#ifndef CPU_H_
#define CPU_H_

#include "Memory.h"
#include "mips_t.h"


class Cpu{
public:
	const void execute(const int count);

	mips_t* getMips() const { return this->mips; };
	Memory* getMem() const{ return this->mem; };

	Cpu(mips_t* const amips, Memory* const amem);

	static Cpu* create_cpu();
	static void delete_cpu(Cpu* cpu);

private:
	mips_t* const mips;
	Memory* const mem;

	const void executeOp(const uInt op);
	const void executeRegOp(const uInt op);
	const void executeImmOp(const uInt op);
	const void executeJmpOp(const uInt op);
	const void executeFpuOp(const uInt op);
};


#endif /* CPU_H_ */
