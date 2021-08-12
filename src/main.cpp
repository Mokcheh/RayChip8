#include "cpu.h"


int gayass(){
	return 4;
}
int main(int argc, char **argv){
	cpu chip;
	chip.loadApp(argv[1]);
	std::cout << reinterpret_cast<void*>(gayass) << std::endl;
	return 0;
}
