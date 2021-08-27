#include "cpu.h"
#include "raylib.h"

int main(int argc, char **argv){
    cpu *chip = new cpu();
    chip->loadApp(argv[1]);
    InitWindow(1280 , 640 , "Chip8 Emu");

    SetTargetFPS(300);
    while(!WindowShouldClose()){
        chip->EmuInstruction();
        chip->keyUnpressed();
        chip->updateDisplay();
        chip->keyPressed();

    }
	
	CloseWindow();
    delete chip;
	return 0;
}
