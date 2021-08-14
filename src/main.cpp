#include "cpu.h"


int main(int argc, char **argv){
	cpu chip;
	chip.loadApp(argv[1]);
	for(int i=0; i<20;i++){
		chip.EmuInstruction();
	}
	
	return 0;
}
