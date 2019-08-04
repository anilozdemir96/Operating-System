#include "Assembler.h"

unsigned int getRnum(string Reg)
{
	if (Reg == "PC")
		return 0;
	else if (Reg == "SP")
		return 1;
	else if (Reg == "V")
		return 2;
	else if (Reg == "S0")
		return 3;
	else if (Reg == "S1")
		return 4;
	else if (Reg == "S2")
		return 5;
	else if (Reg == "S3")
		return 6;
	else if (Reg == "S4")
		return 7;
	else if (Reg == "S5")
		return 8;
	else if (Reg == "S6")
		return 9;
	else if (Reg == "S7")
		return 10;
	else if (Reg == "BR")
		return 11;
	else if (Reg == "$0")
		return 12;
	else
	{
		return -1; // Error code
	}
}

unsigned int Type1(unsigned int init, stringstream & currentLine)
{
	string reg1, reg2, reg3;
	currentLine >> reg1 >> reg2 >> reg3;
	return (int)(init + getRnum(reg1)*pow(2, 21) + getRnum(reg2)*pow(2, 16) + getRnum(reg3)*pow(2, 11));
}

unsigned int CharSeqToInt(string S)
{
	int total = 0;
	for (int i = 15; i >= 0; i--) {
		if (S[15 - i] == '1')
			total += (int)pow(2, i);
	}
	return total;
}

unsigned int Type2(unsigned int init, stringstream & currentLine)
{
	string reg1, reg2, imm;
	currentLine >> reg1 >> reg2 >> imm;
	return (int)(init + getRnum(reg1)*pow(2, 21) + getRnum(reg2)*pow(2, 16) + CharSeqToInt(imm));
}

unsigned int Type3(unsigned int init, stringstream & currentLine)
{
	string reg1, imm;
	currentLine >> reg1 >> imm;
	return (int)(init + getRnum(reg1)*pow(2, 21) + CharSeqToInt(imm));
}

unsigned int Type4(unsigned int init, stringstream & currentLine)
{
	string reg1, reg2;
	currentLine >> reg1 >> reg2;
	return (int)(init + getRnum(reg1)*pow(2, 21) + getRnum(reg2)*pow(2, 16));
}

unsigned int EncodeLine(string S)
{
	stringstream line_stream(S);
	string op;
	line_stream >> op;
	int ins = 0;
	if (op == "ADD") {
		ins += (int)pow(2, 30);
		return Type1(ins, line_stream);
	}
	else if (op == "SUB") {
		ins += (int)pow(2, 30) + (int)pow(2, 26);
		return Type1(ins, line_stream);
	}
	else if (op == "ADDI") {
		ins += (int)pow(2, 27);
		return Type2(ins, line_stream);
	}
	else if (op == "SUBI") {
		ins += (int)pow(2, 27) + (int)pow(2, 26);
		return Type2(ins, line_stream);
	}
	else if (op == "ORI") {
		ins += (int)pow(2, 29) + (int)pow(2, 26) + (int)pow(2, 30) + (int)pow(2, 28) + (int)pow(2, 27);
		return Type2(ins, line_stream);
	}
	else if (op == "LUI") {
		ins += (int)pow(2, 29);
		return Type3(ins, line_stream);
	}
	else if (op == "BEQ")
	{
		ins += (int)pow(2, 28) + (int)pow(2, 27) + (int)pow(2, 26);
		return Type2(ins, line_stream);
	}
	else if (op == "LW") {
		ins += (int)pow(2, 29) + (int)pow(2, 26);
		return Type4(ins, line_stream);
	}
	else if (op == "SW") {
		ins += (int)pow(2, 29) + (int)pow(2, 27);
		return Type4(ins, line_stream);
	}
	else if (op == "SYSCALL")
		return 0;
	else
	{
		return -1; //Error Code
	}
}

int createBinaryFile(string inputfile)
{

	ifstream is(inputfile);
	if (is.is_open() == false) {
		cout << "Cannot open the file, exiting";
		return -1;
	}

	string output_file = inputfile.substr(0, inputfile.find("."));
	string strNum = output_file.substr(output_file.length() - 1, output_file.length());

	ofstream of, inter;
	of.open(output_file + ".bin", ios::binary | ios::out);

	string line;
	int counter = 0;
	while (getline(is, line)) {

		unsigned int instruction_encoded = EncodeLine(line);
		for (int i = 0; i < 4; i++) {
			const char * addr = ((const char *)&instruction_encoded) + 3 - i;
			of.write(addr, 1);
		}

		counter = counter + 4;
	}
	of.close();
	inter.close();
	cout << "Binary file created..." << endl;

	return counter;
}

char * readBinaryFile(const int & instructionsSize, string fileName)
{
	ifstream file;
	file.open(fileName.c_str(), ios::in | ios::binary);
	char * buffer = NULL;

	if (file.is_open())
	{
		buffer = new char[instructionsSize];
		file.read((char*)buffer, instructionsSize);
	}

	return buffer;
}
