#include "Include/Chip-8.hpp"


void Chip8::Cycle(){
	
	 opcode = (memory[pc] <<  8) | memory[pc+1];
	 pc += 2; 

	 ((*this).*(table[(opcode  & 0xF000)  >> 12]))();
     
	 if(delaytimer >  0){
	 	--delaytimer;
	 }


    
	 if(soundtimer > 0)
	 {
	 	--soundtimer; 
	 }

         if(soundtimer == 1){
	 	   system("echo -e '\a'");
         }




}


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
        
     // Set up function pointer table
	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	tableF[0x07] = &Chip8::OP_Fx07;
	tableF[0x0A] = &Chip8::OP_Fx0A;
	tableF[0x15] = &Chip8::OP_Fx15;
	tableF[0x18] = &Chip8::OP_Fx18;
	tableF[0x1E] = &Chip8::OP_Fx1E;
	tableF[0x29] = &Chip8::OP_Fx29;
	tableF[0x33] = &Chip8::OP_Fx33;
	tableF[0x55] = &Chip8::OP_Fx55;
	tableF[0x65] = &Chip8::OP_Fx65;
	return; 

}








void Chip8::OP_00E0(){
	memset(video , 0 , sizeof(video));

}


void Chip8::OP_00EE(){
	--sp; 
	pc = stack[sp];

}


void Chip8::OP_1nnn(){
	uint16_t address = opcode & 0x0FFF;
	pc = address; 
}
 
void Chip8::OP_2nnn(){
	uint16_t address = opcode & 0x0FFF;
	stack[sp] = pc;
	sp++; 
	pc = address; 
}  

void Chip8::OP_3xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] == byte)
	{
		pc += 2;
	}
}

void Chip8::OP_NULL(){
  printf("No opcode\n");
}

void Chip8::OP_4xkk(){
	uint8_t Vx = (opcode & 0x0F00u) >> 8;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] != byte)
	{
		pc += 2;
	}
}

void Chip8::OP_5xy0(){

	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vx] == registers[Vy])
	{
		pc += 2;
	}
}


void Chip8::OP_6xkk(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;

	registers[Vx] = byte;  
}

void Chip8::OP_7xkk(){
    
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	registers[Vx] += byte;
}


void Chip8::OP_8xy0(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] = registers[Vy];
}

void Chip8::OP_8xy1(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] |= registers[Vy];
}

void Chip8::OP_8xy2(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] &= registers[Vy];
}


void Chip8::OP_8xy3(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	registers[Vx] ^= registers[Vy];
}


void Chip8::OP_8xy4(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	uint16_t sum = registers[Vx] + registers[Vy];

	if (sum > 255U)
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] = sum & 0xFF;
}


void Chip8::OP_8xy5(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vx] > registers[Vy])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] -= registers[Vy];
}




void Chip8::OP_8xy6(){
    uint8_t Vx = (opcode & 0x0F00) >> 8;  
	registers[0xF] = (registers[Vx] & 0x1);
	registers[Vx] >>= 1;
}


void Chip8::OP_8xy7(){

	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vy] > registers[Vx])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] = registers[Vy] - registers[Vx];
	
	
}

void Chip8::OP_8xyE(){
	 uint8_t Vx = (opcode & 0x0F00) >> 8;

	
	registers[0xF] = (registers[Vx] & 0x80) >> 7;

	registers[Vx] <<= 1;
}

void Chip8::OP_9xy0(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vx] != registers[Vy])
	{
		pc += 2;
	}

}

void Chip8::OP_Annn(){
	
	uint16_t address = opcode & 0x0FFF;
	index = address;
}

void Chip8::OP_Bnnn(){

	
	uint16_t address = opcode & 0x0FFF;
	pc = registers[0] + address;
}



void Chip8::OP_Cxkk(){
    uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	registers[Vx] = RandByte() & byte;
}



void Chip8::OP_Dxyn(){
	 
	 uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	uint8_t height = opcode & 0x000F;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80 >> col); 
             
             		if((yPos + row) * VIDEO_WIDTH + (xPos + col) < 2048){
			
		      	uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			
			if (spritePixel)
			{
				
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}

				
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	   }
	}
    
}


void Chip8::OP_Ex9E(){
	
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	uint8_t key = registers[Vx];

	if (keypad[key])
	{
		pc += 2;
	}
}


void Chip8::OP_ExA1(){
	uint8_t  Vx  = (opcode & 0x0F00)  >> 8;
	uint8_t key  = registers[Vx];

	if(!keypad[key]){
		pc += 2; 
	}  
}



void Chip8::OP_Fx07()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	registers[Vx] = delaytimer;
}


void Chip8::OP_Fx0A(){
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

void Chip8::OP_Fx15(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	delaytimer = registers[Vx];
}


void Chip8::OP_Fx18(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	soundtimer = registers[Vx];
}
 

void  Chip8::OP_Fx1E(){
   uint8_t Vx = (opcode & 0x0F00) >> 8;
   index += registers[Vx];
}

void Chip8::OP_Fx29(){
  uint8_t Vx = (opcode & 0x0F00) >> 8;
  uint8_t digit = registers[Vx];
  index = FONTSET_START_ADDRESS + (5 * digit);
}

void Chip8::OP_Fx33(){
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t value = registers[Vx];

	
	memory[index + 2] = value % 10;
	value /= 10;

	
	memory[index + 1] = value % 10;
	value /= 10;

	
	memory[index] = value % 10;
}


void Chip8::OP_Fx55(){
    
   uint8_t Vx = (opcode & 0x0F00) >> 8;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		memory[index + i] = registers[i];
	}
}

void Chip8::OP_Fx65(){
  
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		registers[i] = memory[index + i];
	}
}

