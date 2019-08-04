/*
* Created on: Dec 08, 2016
*	  Author: aozdemir
*/
#include "ProcessImage.h"

ProcessImage::ProcessImage()
{
	this->processName = "Unnamed Process!";
	S0 = 0;
	S1 = 0;
	S2 = 0;
	S3 = 0;
	S4 = 0;
	S5 = 0;
	S6 = 0;
	S7 = 0;
	$0 = 0;
	PC = 0;
	V = 0;
	IR = 0;
	BR = 0;
	LR = 0;
}

ProcessImage::ProcessImage(int baseRegister, int limitRegister)
{
	this->processName = "Unnamed Process!";
	S0 = 0;
	S1 = 0;
	S2 = 0;
	S3 = 0;
	S4 = 0;
	S5 = 0;
	S6 = 0;
	S7 = 0;
	$0 = 0;
	PC = 0;
	V = 0;
	IR = 0;
	BR = baseRegister;
	LR = limitRegister;
}

ProcessImage::ProcessImage(string processName, int baseRegister, int limitRegister)
{
	this->processName = processName;
	S0 = 0;
	S1 = 0;
	S2 = 0;
	S3 = 0;
	S4 = 0;
	S5 = 0;
	S6 = 0;
	S7 = 0;
	$0 = 0;
	PC = 0;
	V = 0;
	IR = 0;
	BR = baseRegister;
	LR = limitRegister;
}

ProcessImage::ProcessImage(string processName, int limitRegister)
{
	this->processName = processName;
	S0 = 0;
	S1 = 0;
	S2 = 0;
	S3 = 0;
	S4 = 0;
	S5 = 0;
	S6 = 0;
	S7 = 0;
	$0 = 0;
	PC = 0;
	V = 0;
	IR = 0;
	BR = 0;
	LR = limitRegister;
}

ProcessImage::ProcessImage(string processName)
{
	this->processName = processName;
	S0 = 0;
	S1 = 0;
	S2 = 0;
	S3 = 0;
	S4 = 0;
	S5 = 0;
	S6 = 0;
	S7 = 0;
	$0 = 0;
	PC = 0;
	V = 0;
	IR = 0;
	BR = 0;
	LR = 0;
}

ostream & operator<<(ostream & stream, const ProcessImage p)
{
	stream << "Process image of " << p.processName << " :\n"
		<< "S0: " << p.S0 << "\nS1: " << p.S1 << "\nS2: " << p.S2 << "\nS3: " << p.S3
		<< "\nS4: " << p.S4 << "\nS5: " << p.S5 << "\nS6: " << p.S6 << "\nS7: " << p.S7
		<< "\n$0: " << p.$0 << "\nPC: " << p.PC << "\nV: " << p.V << "\nIR: " << p.IR
		<< "\nBR: " << p.BR << "\nLR: " << p.LR << endl;
	return stream;
}
