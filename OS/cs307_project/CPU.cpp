#include "CPU.h"


int calculateSpecificLoc(int* arr, int base, int limit)
{
	int power = limit - base;
	int total = 0;

	for (int i = base; i <= limit; i++, power--) {

		if (arr[i] == 1)
			total += (int) pow(2, power);
	}
	return total;
}

int* tobinary(int number, int size) {

	int n = number;
	int* binaryForm = new int[size];
	int index = 0;
	int power = size - 1;

	for (int i = 0; i < size; i++, power--) {

		if (number - (int) pow(2, power) >= 0) {
			number -= (int) pow(2, power);
			binaryForm[i] = 1;
		}
		else
			binaryForm[i] = 0;
	}
	return binaryForm;
}


CPU::CPU(Memory* memory = NULL)
{
	this->memory = memory;
}


void CPU::fetch()
{
	char* buffer = memory->getInstruction(this->PC, this->BR);
	unsigned int result = ((unsigned char)buffer[0] << 24) + ((unsigned char)buffer[1] << 16) + ((unsigned char)buffer[2] << 8) + (unsigned char)buffer[3];
	this->IR = result;
	this->PC += 4;
}


unsigned int* CPU::ReturnReg(int num) {
	if (num == 0)
		return &(this->PC);
	else if (num == 2)
		return &(this->V);
	else if (num == 3)
		return &(this->S0);
	else if (num == 4)
		return &(this->S1);
	else if (num == 5)
		return &(this->S2);
	else if (num == 6)
		return &(this->S3);
	else if (num == 7)
		return &(this->S4);
	else if (num == 8)
		return &(this->S5);
	else if (num == 9)
		return &(this->S6);
	else if (num == 10)
		return &(this->S7);
	else if (num == 11)
		return &(this->BR);
	else if (num == 12)
		return &(this->$0);
	else
		return NULL;

}


int CPU::decodeExecute()
{
	unsigned int *R1 = NULL;
	unsigned int *R2 = NULL;
	unsigned int *R3 = NULL;
	int r1, r2, r3, c;
	int* binaryFormat = tobinary((int)this->IR, 32); // 32 is the fix 
	int operation = calculateSpecificLoc(binaryFormat, 0, 5);

	if (operation == 0)
	{
		return 0 ;
		//Leave it to OS
	}
	else if (operation == 16 || operation == 17)
	{
		r1 = calculateSpecificLoc((int*)binaryFormat, 6, 10);
		r2 = calculateSpecificLoc((int*)binaryFormat, 11, 15);
		r3 = calculateSpecificLoc((int*)binaryFormat, 16, 20);
		R1 = ReturnReg(r1);
		R2 = ReturnReg(r2);
		R3 = ReturnReg(r3);
	}
	else if (operation == 2 || operation == 3 || operation == 7 || operation == 31)
	{
		r1 = calculateSpecificLoc((int*)binaryFormat, 6, 10);
		r2 = calculateSpecificLoc((int*)binaryFormat, 11, 15);
		R1 = ReturnReg(r1);
		R2 = ReturnReg(r2);
		c = calculateSpecificLoc((int*)binaryFormat, 16, 31);
	}
	else if (operation == 9 || operation == 10)
	{
		r1 = calculateSpecificLoc((int*)binaryFormat, 6, 10);
		r2 = calculateSpecificLoc((int*)binaryFormat, 11, 15);
		R1 = ReturnReg(r1);
		R2 = ReturnReg(r2);
	}
	else if (operation == 8)
	{
		r1 = calculateSpecificLoc((int*)binaryFormat, 6, 10);
		R1 = ReturnReg(r1);
		c = calculateSpecificLoc((int*)binaryFormat, 16, 31);
	}

	if (operation == 16) {
		*R1 = (unsigned int)((int)*R2 + (int)*R3);
	}
	else if (operation == 17) {
		*R1 = (unsigned int)((int)*R2 - (int)*R3);
	}
	else if (operation == 2) {
		*R1 = (unsigned int)((int)*R2 + (int)c);
	}
	else if (operation == 3) {
		*R1 = (unsigned int)((int)*R2 - (int)c);
	}
	else if (operation == 31) {
		*R1 = (unsigned int)(*R2 | (unsigned int) c);
	}
	else if (operation == 7) {
		if (*R1 == *R2)PC = BR + c;
	}
	else if (operation == 9)
	{
		char * buffer = memory->getInstruction(*R2, 0);
		*R1 = ((unsigned char)buffer[0] << 24) + ((unsigned char)buffer[1] << 16) + ((unsigned char)buffer[2] << 8) + (unsigned char)buffer[3];


	}
	else if(operation == 10) {
		char * buffer = new char[4];
		for (int i = 0; i < 4; i++) {
			buffer[i] = *(((const char *)&*R1) + 3 - i);
		}

		memory->addInstructions(buffer, 4, *R2);
	}
	else if (operation == 8) {
		*R1 = (unsigned int)(c << 16);
	}
	return 1;
}

int CPU::getLR()
{
	return LR;
}

int CPU::getPC()
{
	return PC;
}

int CPU::getIR()
{
	return IR;
}

int CPU::getV()
{
	return V;
}

int CPU::getBR()
{
	return BR;
}


void CPU::transferFromImage(ProcessImage pImage)
{
	this->processName = pImage.processName;
	this->S0 = pImage.S0;
	this->S1 = pImage.S1;
	this->S2 = pImage.S2;
	this->S3 = pImage.S3;
	this->S4 = pImage.S4;
	this->S5 = pImage.S5;
	this->S6 = pImage.S6;
	this->S7 = pImage.S7;
	this->$0 = pImage.$0;
	this->PC = pImage.PC;
	this->V = pImage.V;
	this->IR = pImage.IR;
	this->BR = pImage.BR;
	this->LR = pImage.LR;
}


void CPU::transferToImage(ProcessImage & pImage)
{
	pImage.processName = this->processName;
	pImage.S0 = this->S0;
	pImage.S1 = this->S1;
	pImage.S2 = this->S2;
	pImage.S3 = this->S3;
	pImage.S4 = this->S4;
	pImage.S5 = this->S5;
	pImage.S6 = this->S6;
	pImage.S7 = this->S7;
	pImage.$0 = this->$0;
	pImage.PC = this->PC;
	pImage.V  = this->V;
	pImage.IR = this->IR;
	pImage.BR = this->BR;
	pImage.LR = this->LR;
}
