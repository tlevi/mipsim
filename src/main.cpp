#include "shared.h"
#include "Cpu.h"


#define COMMAND_UNKNOWN() (cerr << "Unknown command specified, ignoring\n")
#define COMMAND_SYNTAX() (warn("Incorrect syntax in command, ignoring\n"))

void cmd_reg(char const line[512], mips_t& mips);


int main(int argc, char** argv){
	char line[512];

	mips_t mips;
	mips.init();
	Cpu::setMips(mips);

	while (!cin.eof() && cin.getline(line, 512)){
		switch (line[0]){
			case 'r':
				cmd_reg(line, mips);
				break;
			case 'p':
				break;
			case 'm':
				break;
			case '.':
				break;
			default:
				COMMAND_UNKNOWN();
		};
	}

	return EXIT_SUCCESS;
};


void cmd_reg(char const line[512], mips_t& mips){
	uInt reg = 0;
	uInt val = 0;

	int result = sscanf(line, "r%2u = %8x", &reg, &val);

	if (!result || reg > 31){
		COMMAND_SYNTAX();
		return;
	}

	if (result == 2){
		mips.r[reg] = val;
		mips.r[0] = 0; // r0 is always zero
	}
	else printf("%8x\n", mips.r[reg]);

	return;
};
