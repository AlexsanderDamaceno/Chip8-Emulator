#include <fstream>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <stdlib.h>

using namespace std; 

#define VIDEO_WIDTH  64
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
       uint32_t  video[64*32];
       uint16_t  opcode;  
       Chip8();
       void  LoadRom(const char *Name);
       short RandByte();
       void  OP_CLS();
       void OP_Ret();
       void OP_JP();
       void OP_Call();
       void OP_SE();
       void OP_SER();
       void OP_SNE();
       void OP_LD();
       void OP_ADD();
       void OP_LDR();
       void OP_ORR();
       void OP_ANDR();
       void OP_XORR();
       void OP_ADDC();
       void OP_SUB();
       void OP_SHR();
       void OP_SHL(); 
       void SUBN();
       void OP_SNER();
       void OP_LDI();
       void OP_JPV0();
       void OP_RND();
       void OP_DRW();
       void SKP();
       void SKNP();
       void LDDelay();
       void OP_WAITKEY();
       void SetDelay();
       
};



uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};




short Chip8::RandByte(){
  srand(time(NULL));
  return rand() % 256; 
}




void Chip8::LoadRom(const char *Name){
     ifstream file(Name ,  ios::binary | ios::ate);

     if(file.is_open()){
     	streampos size = file.tellg();
     	char *buffer   = new char[size];

     	file.seekg(0 , ios::beg);
     	file.read(buffer , size);
     	file.close();
     	for(int i = 0; i < size; i++){
     		memory[START_ADDRESS + i] =  buffer[i];

     	}

     	for(int i = 0; i < FONTSET_SIZE;  i++){
     		memory[FONTSET_START_ADDRESS + i] = fontset[i];
     	}

     	delete[] buffer; 
     }
}

Chip8::Chip8(){
	 pc = START_ADDRESS;
}


void Chip8::OP_CLS(){
	memset(video , 0 , sizeof(video));

}


void Chip8::OP_Ret(){
	--sp; 
	pc = stack[sp];

}


void Chip8::OP_JP(){
	uint16_t address = opcode & 0x0FFFu;
	pc = address; 
}
 
void Chip8::OP_Call(){
	uint16_t address = opcode & 0x0FFFu;
	stack[sp] = address;
	sp++; 
	pc = address; 
}  

void Chip8::OP_SE(){
	uint8_t Vx   = (opcode & 0x0F00) >> 8; 
	uint8_t byte = opcode & 0x00FF;

	if(registers[Vx] == byte)
	{
		pc += 2; 
	} 
}


void Chip8::OP_SNE(){
	uint8_t Vx   = (opcode & 0x0F00) >> 8; 
	uint8_t byte = opcode & 0x00FF;

	if(registers[Vx] == byte)
	{
		pc += 2; 
	} 
}

void Chip8::OP_SER(){

	uint8_t Vx   = (opcode & 0x0F00) >> 8; 
	uint8_t Vy =   (opcode & 0x00F0) >> 4;

	if(registers[Vx] == registers[Vy])
	{
		pc += 2; 
	} 

}


void Chip8::OP_LD(){
	uint8_t Vx   = (opcode & 0x0F00) >> 8; 
	uint8_t byte = opcode  & 0x00FF;
	registers[Vx] = byte;  
}

void Chip8::OP_ADD(){
	uint8_t Vx   =  (opcode & 0x0F00) >> 8; 
	uint8_t byte =  opcode  & 0x00FF;
	registers[Vx] += byte;  
}


void Chip8::OP_LDR(){
	uint8_t Vx = (opcode & 0x0F00) >> 8; 
	uint8_t Vy = (opcode & 0X00F0) >> 4;
    registers[Vx] = registers[Vy];
}

void Chip8::OP_ORR(){
	uint8_t Vx = (opcode & 0x0F00) >> 8; 
	uint8_t Vy = (opcode & 0X00F0) >> 4;
    registers[Vx] |= registers[Vy];
}

void Chip8::OP_ANDR(){
	uint8_t Vx = (opcode & 0x0F00) >> 8; 
	uint8_t Vy = (opcode & 0X00F0) >> 4;
    registers[Vx] &= registers[Vy];
}


void Chip8::OP_XORR(){
	uint8_t Vx = (opcode & 0x0F00) >> 8; 
	uint8_t Vy = (opcode & 0X00F0) >> 4;
    registers[Vx] ^= registers[Vy];
}


void Chip8::OP_ADDC(){
	uint8_t Vx =  (opcode & 0x0F00) >> 8;
	uint8_t Vy =  (opcode & 0x0F00) >> 4;

	uint16_t sum = registers[Vx] + registers[Vy];

	if(sum > 255){
		registers[0x0F] = 1; 
	}else{
		registers[0x0F] = 0; 
	}

	registers[Vx] = sum & 0x00FF;
}


void Chip8::OP_SUB(){
	uint8_t Vx =  (opcode & 0x0F00) >> 8;
	uint8_t Vy =  (opcode & 0x0F00) >> 4;

	if(registers[Vx] > registers[Vy]){
		registers[0xF] = 1; 
	}else{
		registers[0xF] = 0; 
	}

	registers[Vx] -= registers[Vy]; 
}




void Chip8::OP_SHR(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	registers[0xF] = (registers[Vx] & 0x1);
	registers[Vx] >>= 1; 
}


void Chip8::SUBN(){
	
	uint8_t Vx =  (opcode & 0x0F00) >> 8;
	uint8_t Vy =  (opcode & 0x0F00) >> 4;

	if(registers[Vy] > registers[Vx]){
		registers[0xF] = 1; 
	}else{
		registers[0xF] = 0; 
	}

	registers[Vx] = registers[Vy] - registers[Vx]; 
}

void Chip8::OP_SHL(){
	 uint8_t Vx = (opcode & 0x0F00) >>  8;
	 registers[0xF] = (registers[Vx] & 0x80) >> 7; 
	 registers[Vx]  <<= 1;   
}

void Chip8::OP_SNER(){
	uint8_t Vx  = (opcode & 0x0F00) >> 8;
	uint8_t Vy  = (opcode & 0x00F0) >> 4; 

	if(registers[Vx] != registers[Vy]){
		pc += 2; 
	}  

}

void Chip8::OP_LDI(){
	
	uint16_t address =  opcode & 0x0FFF;
	index = address;
}

void Chip8::OP_JPV0(){

	uint16_t address = opcode & 0xFFF;
	pc = registers[0] + address;
}



void Chip8::OP_RND(){
   uint8_t Vx   = (opcode & 0x0F00) >> 8;
   uint8_t byte =  opcode  & 0x00FF; 
   registers[Vx] = RandByte() & byte; 
}



void Chip8::OP_DRW(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
    uint8_t height  = opcode & 0x000F;

    uint8_t xPos = registers[Vx] %  VIDEO_WIDTH; 
    uint8_t yPos = registers[Vy] %  VIDEO_HEIGHT;

    registers[0xF]  = 0; 


    for(unsigned int row = 0;  row <  height; row++){

    	uint8_t  spriteByte = memory[index + row];

    	for(unsigned int col = 0;  col < 8; ++col){
    		uint8_t  spritePixel = spriteByte & (0x80 >> col);
    		uint32_t *screenPixel = &video[(yPos + row)*VIDEO_WIDTH + (xPos + col)];

    		if(spritePixel){
    			if(*screenPixel == 0xFFFFFFFF){
    				registers[0xF] = 1;
    			}

    			*screenPixel  ^= 0xFFFFFFFF;
    		}
    	}

    }
    
}


void Chip8::SKP(){
	uint8_t  Vx  = (opcode & 0x0F00)  >> 8;
	uint8_t key  = registers[Vx];

	if(keypad[key]){
		pc += 2; 
	}  
}


void Chip8::SKNP(){
	uint8_t  Vx  = (opcode & 0x0F00)  >> 8;
	uint8_t key  = registers[Vx];

	if(!keypad[key]){
		pc += 2; 
	}  
}



void Chip8::LDDelay(){
  uint8_t Vx = (opcode & 0x0F00) >> 8; 
  registers[Vx] = delaytimer;
}



void Chip8::OP_WAITKEY(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;


	for(int i = 0; i <= 15; i++)
	{
		if(keypad[i])
		{
			registers[Vx] = i;
			return; 
		}
	}

	pc -= 2; 
	return; 
}

void Chip8::SetDelay(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	delaytimer = registers[Vx];
}





int main(){

}