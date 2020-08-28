#include <fstream>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include "Include/Plataform.hpp"
#include "Include/Chip-8.hpp"





int main(int argc , char ** argv){

   if(argc != 4){
     cout << "error  less arguments than expected" << endl; 
     exit(-1);
   }
  

   int vid_scale   =   stoi(argv[1]);
   int cycledelay  =   stoi(argv[2]);


   Plataform platafrom("Emulator" ,  VIDEO_WIDTH*vid_scale , VIDEO_HEIGHT*vid_scale , VIDEO_WIDTH , VIDEO_HEIGHT);
   Chip8 Chip8; 
   Chip8.LoadRom(argv[3]);
   

   int video_pitch = sizeof(Chip8.video[0])*VIDEO_WIDTH;
   bool quit = false;
  
   while(!quit){
       quit = platafrom.ProcessInput(Chip8.keypad);
       Chip8.Cycle();
       platafrom.Update(Chip8.video , video_pitch);
       SDL_Delay(cycledelay);
   }
    

   return 0; 

   
}

