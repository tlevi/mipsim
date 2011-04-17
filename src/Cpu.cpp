#include "Cpu.h"
#include "Memory.h"

void Cpu::execute(const int count){
	//TODO
	return;
};


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
