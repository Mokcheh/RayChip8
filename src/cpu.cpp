#include "cpu.h"
#include<fstream>
 #include <chrono>
 #include <thread>
 //yes, I know ... wanna fight about it?
unsigned short Vx; 
unsigned short Vy;  
unsigned short N; //height
unsigned short pixel;
int i;


template <typename I> std::string hex(I w, size_t hex_len = sizeof(I)<<1) {
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
	//reset program counter/opcode/index register ...
	ProgramCounter = 0x200; 
	IndexRegister = 0;
	stackptr = 0;
	opcode = 0;
    delayTimer = 0 , soundTimer = 0;


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
	std::cout << hex(opcode) << std::endl;
	switch(opcode & 0xF000){
		case 0x0000:
			switch(opcode & 0x000F){
				case 0x0000:
					//clears screen
					for(int i=0;i<2048;i++){
						display[i] = 0;
					};
					draw = true;
					ProgramCounter +=2;
					break;
				case 0x000E:
					//returns from subroutine
					stackptr--;
					ProgramCounter = stack[stackptr]+2;
					break;
				default:
					std::cout << "unkown opcode" << std::endl;
			}
			break;
		case 0x1000:
			//jumps to address NNN
			ProgramCounter = opcode & 0x0FFF;
			break;
		case 0x2000:
			//calls subroutine at NNN
			stack[stackptr] = ProgramCounter; //store the current address in stack
			stackptr++;
			ProgramCounter = opcode & 0x0FFF; //jump to NNN
			break;
		case 0x3000:
			//Skips the next instruction if VX equals NN.
			if(VR[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
				ProgramCounter+=4; // skips 1 instruction
			}else{
				ProgramCounter+=2;
			}
			break;
		case 0x4000:
			//Skips the next instruction if VX does not equal NN
			if(VR[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
				ProgramCounter+=4; //skips 1 instruction
			}
			else{
				ProgramCounter+=2;
			}
			break;
		case 0x5000:
			//Skips the next instruction if VX equals VY
			if(VR[(opcode & 0x0F00) >> 8] == VR[opcode & 0x00F0 >> 4]){
				ProgramCounter+=4; // skips 1 instruction
			}else{
				ProgramCounter+=2;
			}
			break;
		case 0x6000:
			//sets VX to NN
			VR[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			ProgramCounter+=2;
			break;
		case 0x7000:
			//Adds NN to VX
			VR[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			ProgramCounter+=2;
			break;
		case 0x8000:
			switch(opcode & 0x000F){
				case 0x0000:
					//sets VX to VY
					VR[(opcode & 0x0F00) >> 8] = VR[(opcode & 0x00F0) >> 4];
					ProgramCounter+=2;
					break;
				case 0x0001:
					//sets VX to VX or VY
					VR[(opcode & 0x0F00) >> 8] = VR[(opcode & 0x0F00) >> 8] | VR[(opcode & 0x00F0) >> 4];
					ProgramCounter+=2;
					break;
				case 0x0002:
					//sets VX to VX and VY
					VR[(opcode & 0x0F00) >> 8] = VR[(opcode & 0x0F00) >> 8] & VR[(opcode & 0x00F0) >> 4];
					ProgramCounter+=2;
					break;
				case 0x0003:
					//sets VX to VX xor VY.
					VR[(opcode & 0x0F00) >> 8] = VR[(opcode & 0x0F00) >> 8] ^ VR[(opcode & 0x00F0) >> 4];
					ProgramCounter+=2;
					break;
				case 0x0004:
					//adds VY to VX. VF(V[15]) is set to 1 when there's a carry and 0 if there isn't
					if(VR[(opcode & 0x00F0) >> 4] > (0xFF - VR[(opcode & 0x0F00) >> 8])){
						//there is carry
						VR[15] = 1;
					}
					else{
						VR[15] = 0;
					}
					VR[(opcode & 0x0F00) >> 8] += VR[(opcode & 0x00F0) >> 4];
					ProgramCounter+=2;
					break;
				case 0x0005:
					// VY is substracted from VX. VF is set to 0 when there is a borrow and 1 if there isnt
					if(VR[(opcode & 0x00F0) >> 4] > VR[(opcode & 0x0F00) >> 8]){
						//there is a borrow
						VR[15] = 0;
					}else{
						//there isn't yes
						VR[15] = 1;
					}
					VR[(opcode & 0x0F00) >> 8] -= VR[(opcode & 0x00F0) >> 4];
					ProgramCounter+=2;
					break;
				case 0x0006:
					//stores the least significant bit of VX in VF and then shifts VX to the right by 1
					VR[15] = VR[(opcode & 0x0F00) >> 8] & 0x1;
					VR[(opcode & 0x0F00) >> 8] >>= 1;
					ProgramCounter+=2;
					break;
				case 0x0007:
					//sets VX to VY minus VX. VF is set to 0 when there is a borrow and 1 when there isnt
					if(VR[(opcode & 0x0F00) >> 8] > VR[(opcode & 0x00F0) >> 4]){
						//there is a borrow
						VR[15] = 0;
					}else{
						//there isn't
						VR[15] = 1;
					}
					VR[(opcode & 0x0F00) >> 8] = VR[(opcode & 0x00F0) >> 4] - VR[(opcode & 0x0F00) >> 8];
					ProgramCounter+=2;
					break;
				case 0x000E:
					//Stores the most significant bit of VX in VF and then shifts VX to the left by 1
					VR[15] = VR[(opcode & 0x0F00) >> 8] >> 7;
					VR[(opcode & 0x0F00) >> 8] <<= 1;
					ProgramCounter+=2;
					break;
				default:
					std::cout << "unkown upcode : " << hex(opcode) << std::endl;
			}
			break;
		case 0x9000:
			//skips the next instruction if VX != VY
			if(VR[(opcode & 0x0F00) >> 8] != VR[(opcode & 0x00F0) >> 4]){
				ProgramCounter+=4; //skips 1 instruction
			}
			else{
				ProgramCounter+=2;
			}
			break;
		case 0xA000:
			//sets I to address NNN
			IndexRegister = opcode & 0x0FFF;
			ProgramCounter+=2;
			break;
		case 0xB000:
			//jumps to address NNN+V0
			ProgramCounter = VR[0] + (opcode & 0x0FFF);
			break;
		case 0xC000:
			//Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN. 
			VR[(opcode & 0x0F00) >> 8] = (rand() % 255) & (opcode & 0x00FF);
			ProgramCounter+=2;
			break;
		case 0xD000:
			//0xDXYN : check opcode table for further information
			Vx = VR[(opcode & 0x0F00) >> 8]; 
			Vy = VR[(opcode & 0x00F0) >> 4];  
			N = opcode & 0x000F; //height
			//pixel; //yes, I know ... wanna fight about it?
			VR[15] = 0; //since there isn't anything flipped yet
			for(int y = 0; y < N; y++){
				pixel = mem[IndexRegister + y]; //we read yes?
				for(int x = 0; x < 8; x++){
					if((pixel & (0x80 >> x)) != 0){
						if(display[Vx + x + ((Vy + y) * 64)] == 1){
							//flipped
							VR[15] = 1;
						}
						display[Vx + x + ((Vy + y) * 64)] ^= 1;
					}
				}

			}
			draw = true;
			ProgramCounter+=2;
			break;
		case 0xE000:
			switch(opcode & 0x000F){
				case 0x000E:
					//skips the next instruction if the key stored in VX is pressed
					if(key[VR[(opcode & 0x0F00) >> 8]] != 0){
						ProgramCounter+=4;
					}
					else{
						ProgramCounter+=2;
					}
					break;
				case 0x0001:
					//skips the next instruction if the key stored in VX isn't pressed
					if(key[VR[(opcode & 0x0F00) >> 8]] == 0){
						ProgramCounter+=4;
					}
					else{
						ProgramCounter+=2;
					}
					break;
				default:
					std::cout << "unkown opcode : 0x" << hex(opcode) << std::endl;
			}
			break;
		case 0xF000:
			switch(opcode & 0x00FF){
				case 0x0007:
					VR[(opcode & 0x0F00) >> 8] = delayTimer;
					ProgramCounter+=2;
					break;
				case 0x000A:
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    ProgramCounter+=2;
                    break;
				case 0x0015:
					delayTimer = VR[(opcode & 0x0F00) >> 8];
					ProgramCounter+=2;
					break;
				case 0x0018:
					soundTimer = VR[(opcode & 0x0F00) >> 8];
					ProgramCounter+=2;
					break;
				case 0x001E:
					IndexRegister += VR[(opcode & 0x0F00) >> 8];
					ProgramCounter+=2;
					break;
				case 0x0029:
					//sets I to the location of the sprite for the character in VX
					//charachters 0-F (in hex) are represented by a 4x5 font
					IndexRegister = VR[(opcode & 0x0F00) >> 8] * 5;
					ProgramCounter+=2;
					break;
				case 0x0033:
					//FX33 : check opcode table for further info
					mem[IndexRegister] = VR[(opcode & 0x0F00) >> 8] / 100;
					mem[IndexRegister + 1] = (VR[(opcode & 0x0F00) >> 8] / 10) % 10;
					mem[IndexRegister + 2] = VR[(opcode & 0x0F00) >> 8] % 10;
					ProgramCounter+=2;
					break;
				case 0x0055:
					//FX55: stores V0 to VX in memory starting at address I
					for(i = 0; i <= (opcode & 0x0F00) >> 8; i++){
						mem[IndexRegister + i] = VR[i];
						ProgramCounter+=2;
					break;
					}
				case 0x0065:
						//FX65 fills V0 to VX with values from memory starting at address I
					for(int i = 0; i <= (opcode & 0x0F00) >> 8;i++){
						VR[i] = mem[IndexRegister+i];
					}
					ProgramCounter+=2;
					break;
				default:
					std::cout << "unkown opcode : 0x" << hex(opcode) << std::endl;
			}
			break;
		default:
			std::cout << "unkown opcode : 0x" << hex(opcode) << std::endl;
	}

	//update timers
	if(delayTimer > 0){
		delayTimer = 0;
	}
	if(soundTimer > 0){
		if(soundTimer == 1){
			//TODO : sound before resetting the timer
            playSound();
			soundTimer--;
		}
	}
}

