

#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H
#include <string>

using namespace std;
class ProcessImage
{
	friend ostream& operator<<(ostream&stream, const ProcessImage p);
public:
	string processName;
	unsigned int S0;
	unsigned int S1;
	unsigned int S2;
	unsigned int S3;
	unsigned int S4;
	unsigned int S5;
	unsigned int S6;
	unsigned int S7;
	unsigned int $0;
	unsigned int PC;
	unsigned int V;
	unsigned int IR;
	unsigned int BR;
	unsigned int LR;

	ProcessImage();
	ProcessImage(int baseRegister, int limitRegister);
	ProcessImage(string processName, int baseRegister, int limitRegister);
	ProcessImage(string processName, int limitRegister);
	ProcessImage(string processName);

};
#endif
