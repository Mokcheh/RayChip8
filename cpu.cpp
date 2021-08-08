#include "cpu.h"
#include<fstream>

int fsize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); //go back to where we were
    return sz;
};

cpu::cpu(){
	//clearning memory
	for(int i=0;i<4096;i++){
		mem[i] = 0;
	};
	std::cout << "memory cleared" << std::endl;

	//clearing display
	for(int i=0; i<2048;i++){
		display[i] = 0;
	};
	std::cout << "display cleared" << std::endl;

	//clearing stack/registers and keys
	for(int i=0;i<16;i++){
		stack[i] = 0;
		VR[i] = 0;
		key[i] = 0;
	};
	
	std::cout << "stack/registers/keys cleared" << std::endl;

	//loading fonts
	for(int i = 0; i<80;i++){
		mem[i] = fontset[i];
	};
	std::cout << "loaded fonts" << std::endl;

	IndexRegister = 0, stackptr = 0;
	delayTimer = 0 , soundTimer = 0;

	std::cout << "chip8 initialized" << std::endl;
	//step 1 done
};

void cpu::loadApp(char* something){
	FILE* rom;
	rom = fopen(something , "r");
	if(rom == NULL){
		std::cout << "failed to load rom" << std::endl;
	}else{
		unsigned short size = fsize(rom);
		if(size > 4096 - 512){
			std::cout << "error! : rom too big" << std::endl;

		}else{
			//allocatin a buffer for the rom
			char* buffer = (char*) malloc(sizeof(char) * size);

			//copy it to the buffer
			int yes = fread(buffer, sizeof(char), size, rom);
			if(yes != size){
				std::cout << "error: failed to copy rom to a buffer" << std::endl;
			}else{
				for(int i=0;i < size;i++){
					mem[i+512] = (uint8_t)buffer[i];
					std::cout << (uint8_t)buffer[i];
				};
				std::cout << "rom loaded into memory" <<std::endl;
			}

			free(buffer);
		}
	}
	fclose(rom);

};


void cpu::EmuInstruction(){
	//TODO: whatever

};
