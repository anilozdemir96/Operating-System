/*
*	   Author: mbenlioglu
*  Created on: Dec 07, 2016
* Modified on: Dec 20, 2016 (changes according to step-2)
*/
#pragma once

#include<Windows.h> //for sleep

#include<string>
#include<iostream>
#include<queue>
#include<vector>
#include<thread>
#include<chrono>
#include<mutex>
#include "CPU.h"
#include "Memory.h"
#include "ProcessImage.h"
#include "Assembler.h"

using namespace std;

class OS
{
public:
	OS(int size);
	~OS();

	void LoadProcess(string processFile);
	void LoadProcess(vector<string>& processFiles);
	void LoadProcess(ifstream& processConfig);
	void Start();

protected:
	void enqueue(queue<ProcessImage>& queue, ProcessImage process);
	ProcessImage dequeue(queue<ProcessImage>& queue);

private:
	const unsigned int QUANTUM;
	bool isAProcessEnded;
	long currentTime;
	CPU* cpu;
	Memory* memory;
	queue<ProcessImage> readyQ;
	queue<ProcessImage> blockedQ;
	mutex mtx;
	thread QManager;

	//update ready and blocked queues
	void AsyncConsumeInput(); //QManager runs this function....
	void BlockProcess(); //from ready Q to blocked Q

	//memory manager thread and processInfo (combines ProcessImage and arrivalTime)
	struct ProcessInfo
	{
		ProcessImage p_image;
		int arrivalTime;
		
		ProcessInfo(ProcessImage image = ProcessImage(), int arrival = INT_MAX)
			: p_image(image), arrivalTime(arrival) {}
	};
	thread memoryManager;
	mutex memoryMtx;
	vector<ProcessInfo> inputQ;
	unsigned int index;

	//functions for memoryManager thread
	void AsyncManageMemory(); //memoryManager runs this function...
};

