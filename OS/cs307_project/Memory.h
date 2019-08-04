/*
* Modified on: Dec 07, 2016
*	   Author: mbenlioglu
*
*	- Changes according to step-1 requirements
*   - Changes according to step-2 requirements (19-12-16)
*		- hasFreeSpace is now a private function and returns a boolean result
*		- addInstrunction now returns an integer result
*/

#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <fstream>
#include "LinkedList.h"

using namespace std;

typedef enum
{
	MEMORY_EMPTY,
	MEMORY_FULL
} MemoryState;

class Memory{
	friend class CPU;

private:
	static const string OUTPUT_FILE_NAME;
	ofstream output;

	//Memory information object for memory chunks
	class Segment
	{
	public:
		int BR;
		unsigned int size;
		MemoryState state;

		Segment(const int& _BR = 0, const int& _size = 0, const MemoryState& _state = MEMORY_EMPTY)
			: BR(_BR), size(_size), state(_state) {}

		const bool operator==(const Segment& rhs) const;
		const bool operator!=(const Segment& rhs) const;

		friend ostream& operator<<(ostream&stream, const Segment s)
		{
			stream << "BR: " << s.BR << " Size: " << s.size << " State: ";
			s.state == MEMORY_EMPTY ? stream << "EMPTY" : stream << "FULL";
			return stream;
		}
	};

	char* memory;
	int memorySize;

	//linked list to keep track of empty and used memory locations.
	List<Segment> memoryList;
	ListItr<Segment> lastIndex; //keep last left location for next-fit

	bool hasFreeSpace(unsigned int size); //adjusts the lastIndex to the next fitting empty part, returns false if no space found
	void addInstructions(char* buffer, unsigned int bufferSize, int BR);

public:

	Memory(int size);
	~Memory();
	int addInstructions(char* buffer, unsigned int bufferSize); //returns BR of added location if successfully added, -1 if no space
	void removeInstructions(int bufferSize, int BR);
	char* getInstruction(int PC, int BR); 
};

#endif