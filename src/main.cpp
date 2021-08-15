#include "cpu.h"


int main(int argc, char **argv){
	cpu chip;
	chip.loadApp(argv[1]);
	for(;;){
		chip.EmuInstruction();
	}
	
	return 0;
}
