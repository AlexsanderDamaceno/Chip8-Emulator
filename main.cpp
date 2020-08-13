#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std; 



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
       void LoadRom(const char *Name);
       void OP_CLS();
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
       void Chip8::OP_SUB();
       
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

















int main(){

}