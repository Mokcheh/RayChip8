#include "cpu.h"


void cpu::updateDisplay(){
    //if display[] got updated then update the screen
    if(cpu::draw){
        cpu::draw = false;
        ClearBackground(BLACK);
        BeginDrawing();
        for(int y = 0; y<32;y++){
            for(int x = 0;x<64;x++){
                if(cpu::display[x+(64*y)]){
                    DrawRectangle(x*20 , y*20 , 20 , 20 , WHITE);
                }
            }
        }
        EndDrawing();
    }
}

void cpu::keyPressed(){
    if (IsKeyDown(KEY_Q)) cpu::key[0] = 1;
    else if (IsKeyDown(KEY_TWO)) cpu::key[1] = 1;
    else if (IsKeyDown(KEY_THREE)) cpu::key[2] = 1;
    else if (IsKeyDown(KEY_FOUR)) cpu::key[3] = 1;
    else if (IsKeyDown(KEY_ONE)) cpu::key[4] = 1;
    else if (IsKeyDown(KEY_W)) cpu::key[5] = 1;
    else if (IsKeyDown(KEY_E)) cpu::key[6] = 1;
    else if (IsKeyDown(KEY_R)) cpu::key[7] = 1;
    else if (IsKeyDown(KEY_S)) cpu::key[9] = 1;
    else if (IsKeyDown(KEY_D)) cpu::key[10] = 1;
    else if (IsKeyDown(KEY_Z)) cpu::key[12] = 1;
    else if (IsKeyDown(KEY_X)) cpu::key[13] = 1;
    else if (IsKeyDown(KEY_C)) cpu::key[14] = 1;
    else if (IsKeyDown(KEY_V)) cpu::key[15] = 1;
}

void cpu::keyUnpressed(){
    if (IsKeyDown(KEY_Q)) cpu::key[0] = 0;
    else if (IsKeyDown(KEY_TWO)) cpu::key[1] = 0;
    else if (IsKeyDown(KEY_THREE)) cpu::key[2] = 0;
    else if (IsKeyDown(KEY_FOUR)) cpu::key[3] = 0;
    else if (IsKeyDown(KEY_ONE)) cpu::key[4] = 0;
    else if (IsKeyDown(KEY_W)) cpu::key[5] = 0;
    else if (IsKeyDown(KEY_E)) cpu::key[6] = 0;
    else if (IsKeyDown(KEY_R)) cpu::key[7] = 0;
    else if (IsKeyDown(KEY_S)) cpu::key[9] = 0;
    else if (IsKeyDown(KEY_D)) cpu::key[10] = 0;
    else if (IsKeyDown(KEY_Z)) cpu::key[12] = 0;
    else if (IsKeyDown(KEY_X)) cpu::key[13] = 0;
    else if (IsKeyDown(KEY_C)) cpu::key[14] = 0;
    else if (IsKeyDown(KEY_V)) cpu::key[15] = 0;
}

