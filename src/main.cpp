#include "Cpu.h"

#define COMMAND_UNKNOWN() (warn("Unknown command specified, ignoring\n"))
#define COMMAND_SYNTAX() (warn("Incorrect syntax in command, ignoring\n"))


void cmd_reg(char const line[512], mips_t& mips);
void cmd_pc(char const line[512], mips_t& mips);
void cmd_mem(char const line[512], Memory& mem);
void cmd_exe(char const line[512], Cpu& cpu);


int main(int argc, char** argv){
	Cpu& cpu = *Cpu::create_cpu();
	bool end = false;
	char line[512];

	while (!end && !cin.eof() && cin.getline(line, 512)){
		switch (line[0]){
			case 'r':
				cmd_reg(line, *cpu.getMips());
				break;
			case 'p':
				cmd_pc(line, *cpu.getMips());
				break;
			case 'm':
				cmd_mem(line, *cpu.getMem());
				break;
			case '.':
				cmd_exe(line, cpu);
				break;
			case 'q':
				end = true;
				break;
			default:
				COMMAND_UNKNOWN();
		};
	}

	Cpu::delete_cpu(&cpu);
	return EXIT_SUCCESS;
};


/* "r" command, display or set general register in hex e.g,
	< r31 = deadbeef
	< r31
	> DEADBEEF */
void cmd_reg(char const line[512], mips_t& mips){
	uInt val = 0;
	uInt reg = 0xcafed00d;
	const int result = sscanf(line, "r%2u = %8x", &reg, &val);

	if (result == EOF || result < 1 || reg > 31){
		COMMAND_SYNTAX();
		return;
	}

	if (result == 2){
		mips.r[reg] = val;
		mips.r[0] = 0; // r0 is always zero
	}
	else printf("%.8X\n", mips.r[reg]);
};


/* "pc" command, display or set program counter in hex e.g,
	< pc = DEADBEEF
	< pc
	> DEADBEEF */
void cmd_pc(char const line[512], mips_t& mips){
	uInt val = 0;
	const int result = sscanf(line, "pc = %8x", &val);

	// just "pc" is acceptable too
	if (result == EOF && sscanf(line, "pc") != 0){
		COMMAND_SYNTAX();
		return;
	}

	if (result == 1) mips.pc = val;
	else printf("%.8X\n", mips.pc);
};


void cmd_mem(char const line[512], Memory& mem){
	uInt val = 0xdeadbeef;
	uInt addr = 0xc001d00d;
	const int result = sscanf(line, "m %8x = %8x", &addr, &val);

	if (result == EOF || result < 1){
		COMMAND_SYNTAX();
		return;
	}

	// address must be aligned
	if (addr & 0x3)
		fatalError("Address for m command misaligned, ignoring\n");

	if (result == 2) mem.set<uInt>(addr, val);
	else printf("%.8X\n", mem.get<uInt>(addr));
};


void cmd_exe(char const line[512], Cpu& cpu){
	uInt count = 1;

	if (sscanf(line, ". %u", &count) == EOF && sscanf(line, ".") == EOF){
		COMMAND_SYNTAX();
		return;
	}

	cpu.execute(count);
};

