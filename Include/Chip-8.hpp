#ifndef Chip8_HH
#define Chip8_HH

#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <time.h>
#include <fstream>

using namespace std; 

#define VIDEO_WIDTH   64
#define VIDEO_HEIGHT  32

const unsigned int START_ADDRESS = 0x200; 
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50; 




class  Chip8{
   public: 

       uint8_t   registers[16];
       uint8_t   memory[4096];
       uint16_t  index;
       uint16_t  stack[16];
       uint16_t  pc; 
       uint8_t   sp; 
       uint8_t   delaytimer;
       uint8_t   soundtimer; 
       uint8_t   keypad[16];
       uint32_t  video[64*32]{};
       uint16_t  opcode;  
       
       short RandByte();
       void Cycle();
       void OP_00EE();
       void OP_00E0();
       void LoadRom(const char *Name);
       void OP_1nnn();
       void OP_2nnn();
       void OP_3xkk();
       void OP_4xkk();
       void OP_5xy0();
       void OP_6xkk();
       void OP_7xkk();
       void OP_8xy0();
       void OP_8xy1();
       void OP_8xy2();
       void OP_8xy3();
       void OP_8xy4();
       void OP_8xy5();
       void OP_8xy6();
       void OP_8xy7();
       void OP_8xyE();
       void OP_9xy0();
       void OP_Annn();
       void OP_Bnnn();
       void OP_Cxkk();
       void OP_Dxyn();
       void OP_Ex9E();
       void OP_ExA1();
       void OP_Fx07();
       void OP_Fx0A();
       void OP_Fx15();
       void OP_Fx18();
       void OP_Fx1E();
       void OP_Fx29();
       void OP_Fx33();
       void OP_Fx55();
       void OP_Fx65();
           
      
       void Table0()
     	  {
     	   ((*this).*(table0[opcode & 0x000F]))();
		 
	  }

	   void Table8()
   	    {
		  ((*this).*(table8[opcode & 0x000F]))();
	    }

	   void TableE()
	   {
	    	((*this).*(tableE[opcode & 0x000F]))();
	   }

	   void TableF()
	   {
		  ((*this).*(tableF[opcode & 0x00FF]))();
	   }


       void OP_NULL();
       typedef void (Chip8::*Chip8Func)();
       Chip8Func table[0x12]{&Chip8::OP_NULL};
       Chip8Func table0[0x14]{&Chip8::OP_NULL};
       Chip8Func table8[0x14]{&Chip8::OP_NULL};
       Chip8Func tableE[0x14]{&Chip8::OP_NULL};
       Chip8Func tableF[0x200]{&Chip8::OP_NULL};
       
       

       Chip8();

};





#endif

