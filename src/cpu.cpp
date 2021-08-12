#include "cpu.h"
#include<fstream>
#include <bitset>

template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I)<<1) {
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len,'0');
    for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}


int fsize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); //go back to where we were
    return sz;
};

cpu::cpu(){
	//reset program counter/opcode/index register
	ProgramCounter = 0x200; 
	IndexRegister = 0;
	stackptr = 0;
	opcode = 0;


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



void cpu::loadApp(const char* something){
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
					mem[i+512] = buffer[i];
				};
				std::cout << "rom loaded into memory" <<std::endl;
			}

			free(buffer);
		}
	}
	fclose(rom);

};

void cpu::EmuInstruction(){
	//fetch opcode
	opcode = mem[ProgramCounter] << 8 | mem[ProgramCounter+1];
	std::cout << opcode << std::endl;
	switch(opcode & 0xF000){
		case 0x0000:
			switch(opcode & 0x000F){
				case 0x0000:
					//clear screen
					std::cout << "00E0" << std::endl;
					for(int i=0;i<2048;i++){
						display[i] = 0;
					};
					draw = true;
					ProgramCounter +=2;
					break;
				case 0x000E:
					//returns from subroutine
					std::cout << "00EE" << std::endl;
					stackptr--;
					ProgramCounter = stack[stackptr]+2;
					break;
			}
			break;
		case 0xA000:
			VR[opcode & 0x0F00] = opcode & 0x00FF;

	}

}

void cpu::debugthing(){

	opcode = mem[ProgramCounter] << 8 | mem[ProgramCounter+1];
	std::cout << n2hexstr(opcode) << std::endl;
	ProgramCounter+=2;

}
