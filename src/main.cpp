#include "cpu.h"

int main(int argc, char **argv){
    cpu *chip = new cpu();
    chip->loadApp(argv[1]);

    InitWindow(1280 , 640 , "Chip8 Emu");
    chip->loadSound("../bin/bonk.wav");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        for(int cycle = 0; cycle <9;cycle++){
            chip->EmuInstruction();
        }
        chip->keyUnpressed();
        chip->updateDisplay();
        chip->playSound();
        chip->keyPressed();
    }
	
	CloseWindow();
    delete chip;
	return 0;
}

