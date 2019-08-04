/*
* Created on: Dec 08, 2016
*	  Author: mbenlioglu
*
*	- Assembler.cpp splitted into header and cpp for possible linking problems
*/
#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

unsigned int getRnum(string Reg);

unsigned int Type1(unsigned int init, stringstream & currentLine);

unsigned int CharSeqToInt(string S);

unsigned int Type2(unsigned int init, stringstream & currentLine);

unsigned int Type3(unsigned int init, stringstream & currentLine);

unsigned int Type4(unsigned int init, stringstream & currentLine);

unsigned int EncodeLine(string S);

int createBinaryFile(string inputfile);

char* readBinaryFile(const int & instructionsSize, string fileName);