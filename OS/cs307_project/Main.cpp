#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include"OS.h"

using namespace std;

int main()
{
	//for step 1

	//static const int MEMORY_SIZE = 50000;
	//OS os(MEMORY_SIZE);

	//vector<string> FileNames;
	//FileNames.push_back("Processes/process1.asm");
	//FileNames.push_back("Processes/process2.asm");
	//FileNames.push_back("Processes/process3.asm");
	//FileNames.push_back("Processes/process4.asm");
	//os.LoadProcess(FileNames);

	//for step 2
	ifstream input;
	string filename = "processConfig.txt";

	input.open(filename.c_str());

	if (input.fail())
	{
		cout << "Cannot open file\n";
		system("pause");
		return -1;
	}

	int memorySize = 0;
	string line = "";

	getline(input, line);
	memorySize = atoi(line.c_str());

	OS os(memorySize);

	os.LoadProcess(input);

	os.Start();
	
	return 0;
}