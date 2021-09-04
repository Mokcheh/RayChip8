#include "cpu.h"

int main(int argc, char **argv){
    if(argc < 2){
        std::cout << "usage : pass rom path as an argument to the executable" << std::endl;
    }
    else{
        cpu *chip = new cpu();
        chip->loadApp(argv[1]);

        InitWindow(1280 , 640 , "RayChip8 Emulator");
        SetTargetFPS(60);
        while(!WindowShouldClose()){
            for(int cycle = 0; cycle <8;cycle++){
                chip->EmuInstruction();
            }

            chip->keyUnpressed();
            chip->updateDisplay();
            chip->keyPressed();
        }

        CloseWindow();
        delete chip;

    }

	return 0;
}

