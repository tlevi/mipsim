#ifndef CPU_H_
#define CPU_H_

#include "Memory.h"
#include "mips_t.h"


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
	void executeRegOp(uInt op);
	void executeImmOp(uInt op);
	void executeJmpOp(uInt op);
};


#endif /* CPU_H_ */
