
#include "OS.h"


OS::OS(int size = 50000)
	: QUANTUM(5), currentTime(0), isAProcessEnded(false)
{
	cout << "Initilizing the OS....";
	this->memory = new Memory(size);
	this->cpu = new CPU(memory);

	this->readyQ = queue<ProcessImage>();
	this->blockedQ = queue<ProcessImage>();
	this->inputQ = vector<ProcessInfo>();
	cout << "DONE!\n";
}

OS::~OS()
{
	cout << "****************************************************************\n"
		<< "Exiting the OS! GOODBYE!\n"
		<< "****************************************************************\n";
	system("pause");
}

void OS::LoadProcess(string processFile)
{
	cout << "Creating binary file for " << processFile << " ...";
	unsigned int instructionSize = createBinaryFile(processFile);
	char* process = readBinaryFile(instructionSize, processFile.substr(0, processFile.rfind('.')) + ".bin");

	cout << "Loading process to memory...";

	mtx.lock();
	int BR = this->memory->addInstructions(process, instructionSize);

	if (BR != -1)
	{
		enqueue(readyQ, ProcessImage(processFile.substr(0, processFile.rfind('.')), BR, instructionSize));
		cout << "NO Memory! Passing...\n";
	}

	mtx.unlock();

	cout << "DONE!\n";
}

void OS::LoadProcess(vector<string>& processFiles)
{
	for (auto& s : processFiles)
		LoadProcess(s);
}

void OS::LoadProcess(ifstream & processConfig)
{
	processConfig.clear();
	processConfig.seekg(0);

	if (processConfig.bad())
	{
		cout << "Process config file is corrupted... Closing...\n";
		return;
	}

	cout << "Reading processName - arivalTime information....\n";
	string line = "";
	getline(processConfig, line); //skip the line with memory size
	line = "";

	while (getline(processConfig, line))
	{
		istringstream iss(line);

		string processName;
		int arrivalTime;

		iss >> processName >> arrivalTime;

		unsigned int instructionSize = createBinaryFile(processName + ".asm");

		memoryMtx.lock();
		inputQ.push_back(ProcessInfo(ProcessImage(processName, instructionSize), arrivalTime)); //note that queued processImage does not have a BR value
		memoryMtx.unlock();
	}
	index = 0;//index 0
	memoryManager = thread(&OS::AsyncManageMemory, this);
}

void OS::Start()
{
	cout << "****************************************************************\n"
		<< "Starting the OS with " << readyQ.size() + blockedQ.size() + inputQ.size() << " processes...\n";
	cout.flush();

	QManager = thread(&OS::AsyncConsumeInput, this);

	memoryMtx.lock();
	mtx.lock();
	while (!readyQ.empty() || !blockedQ.empty() || !inputQ.empty())
	{
		mtx.unlock();
		memoryMtx.unlock();
		if (!readyQ.empty())
		{
			cout << "Executing " << readyQ.front().processName << "...\n";
			cout.flush();
			cpu->transferFromImage(readyQ.front());
			for (size_t i = 0; i < QUANTUM; i++)
			{
				if (cpu->getLR() != cpu->getPC()) //check if process is finished
				{
					cpu->fetch();
					int returnCode = cpu->decodeExecute();
					currentTime++; //update time

					if (returnCode == 0) //if process makes a system call
					{
						cout << "Process " << readyQ.front().processName << " made a system call for ";
						if (cpu->getV() == 0) //syscall for input
						{
							cout << "input, transfering to blocked queue and waiting for input...\n";
							cout.flush();
							BlockProcess();
						}
						else //syscall for output
						{
							cout << "output. Output Value:\n";
							cout.flush();
							ProcessImage p;
							cpu->transferToImage(p);
							mtx.lock();
							dequeue(readyQ);
							cout << p.V << endl;
							cout.flush();
							enqueue(readyQ, p);
							mtx.unlock();
						}
						//process blocked, need to end quantum prematurely
						break;
					}
				}
				else //process finished
				{
					cout << "Process " << readyQ.front().processName
						<< " have been finished! Removing from the queue...\n";
					ProcessImage p;
					cpu->transferToImage(p);

					memoryMtx.lock();
					this->memory->removeInstructions(p.LR, p.BR);
					isAProcessEnded = true; //inform memory manager that a process has finished
					memoryMtx.unlock();

					cout << "Final " << p;
					cout.flush();

					mtx.lock();
					dequeue(readyQ);
					mtx.unlock();
					break;
				}

				if (i == QUANTUM - 1)
				{
					//quantum finished put the process at the end of readyQ
					cout << "Context Switch! Allocated quantum have been reached, switching to next process...\n";
					cout.flush();
					ProcessImage p;
					cpu->transferToImage(p);
					mtx.lock();
					dequeue(readyQ);
					enqueue(readyQ, p);
					mtx.unlock();
				}
			}
		}
		else
			currentTime++; //update current OS time

		//lock to check the Q's in while condition
		memoryMtx.lock();
		mtx.lock();
	}
	mtx.unlock();
	memoryMtx.unlock();

	QManager.join();
	memoryManager.join();

	cout << "Execution of all processes have finished!\n";
	cout.flush();
}

void OS::enqueue(queue<ProcessImage>& queue, ProcessImage process)
{
	queue.push(process);
}

ProcessImage OS::dequeue(queue<ProcessImage>& queue)
{
	ProcessImage p;

	if (!queue.empty())
	{
		p = queue.front();
		queue.pop();
	}

	return p;
}

void OS::AsyncConsumeInput()
{
	mtx.lock();
	while (!blockedQ.empty() || !readyQ.empty() || !inputQ.empty())
	{
		if (!blockedQ.empty())
		{
			mtx.unlock();

			int input;
			cin.clear();
			cin >> input;

			if (!this->blockedQ.empty()) //input is ignored if no process is blocking for input
			{
				mtx.lock();
				ProcessImage p = dequeue(blockedQ);

				//set input to V register
				p.V = input;

				enqueue(readyQ, p);
				mtx.unlock();
			}
		}
		else
			mtx.unlock();

		//lock to check Q's in while condition
		mtx.lock();
	}
	mtx.unlock();
}

void OS::BlockProcess()
{
	ProcessImage p;
	this->cpu->transferToImage(p);
	mtx.lock();
	dequeue(readyQ);
	enqueue(blockedQ, p);
	mtx.unlock();
}

void OS::AsyncManageMemory()
{
	memoryMtx.lock();
	while (!inputQ.empty())
	{
		if (isAProcessEnded) //A process has ended re-check all the waiting ones
		{
			isAProcessEnded = false;
			for (size_t i = 0; i < inputQ.size() && i != index;)
			{
				cout << "A process have been finished re-checking all waiting processes...\n";
				cout.flush();
				char* buffer = readBinaryFile(inputQ[i].p_image.LR, inputQ[i].p_image.processName + ".bin");
				int BR = this->memory->addInstructions(buffer, inputQ[i].p_image.LR);

				if (BR != -1) //process fits the memory
				{
					cout << "Process " << inputQ[i].p_image.processName << " fits memory. Inserting to memory...\n";
					cout.flush();
					mtx.lock();
					ProcessImage p = inputQ[i].p_image;
					p.BR = BR;
					enqueue(readyQ, p);
					mtx.unlock();
					inputQ.erase(inputQ.begin() + i);
					index--;
				}
				else
				{
					cout << "Process " << inputQ[i].p_image.processName << " doesn't fit. Waiting for opening...\n";
					cout.flush();
					i++;
				}
			}
		}
		else if (index < inputQ.size() && inputQ[index].arrivalTime <= currentTime) //Check if the time arrived for next process
		{
			char* buffer = readBinaryFile(inputQ[index].p_image.LR, inputQ[index].p_image.processName + ".bin");
			int BR = this->memory->addInstructions(buffer, inputQ[index].p_image.LR);

			if (BR != -1) //process fits the memory
			{
				cout << "Process " << inputQ[index].p_image.processName << " fits memory. Inserting to memory...\n";
				cout.flush();
				mtx.lock();
				ProcessImage p = inputQ[index].p_image;
				p.BR = BR;
				enqueue(readyQ, p);
				mtx.unlock();
				inputQ.erase(inputQ.begin() + index);
			}
			else //process doesn't fit
			{
				cout << "Process " << inputQ[index].p_image.processName << " doesn't fit. Waiting for opening...\n";
				cout.flush();
				index++;
			}
		}
		memoryMtx.unlock();

		Sleep(100);

		memoryMtx.lock();
	}
	memoryMtx.unlock();
}
