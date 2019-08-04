#ifndef MEMORY_CPP
#define MEMORY_CPP
#include "Memory.h"

const string Memory::OUTPUT_FILE_NAME = "MemoryConfig.txt";

//========================================================================
//Class Segment
//========================================================================
const bool Memory::Segment::operator==(const Segment & rhs) const
{
	return this->BR == rhs.BR && this->size == rhs.size && this->state == rhs.state
		? true : false;
}

const bool Memory::Segment::operator!=(const Segment & rhs) const
{
	return !(*this == rhs);
}
//========================================================================

bool Memory::hasFreeSpace(unsigned int size)
{
	ListItr<Segment> temp = lastIndex;
	do
	{
		if (!temp.isPastEnd() && !temp.isLast()) 
			temp.advance();
		else 
			temp = memoryList.first();

		if (temp.retrieve().state == MEMORY_EMPTY && temp.retrieve().size >= size)
		{
			lastIndex = temp;
			return true;
		}
	} while (temp.retrieve() != lastIndex.retrieve());

	return false;
}

void Memory::addInstructions(char * buffer, unsigned int bufferSize, int BR)
{
	for (size_t i = BR; i < bufferSize + BR; i++)
	{
		this->memory[i] = buffer[i - BR];
	}
}

Memory::Memory(int size){
	//create output file for memory configurations
	output = ofstream(OUTPUT_FILE_NAME, ios::trunc); //open and truncate file if exists
	output.close();

	memorySize = size;
	memory = new char[size];

	//initialize the memorylist
	memoryList.insert(Segment(0, memorySize, MEMORY_EMPTY), memoryList.zeroth());
	lastIndex = memoryList.first();
}

Memory::~Memory(){

	delete[] memory;
}

int Memory::addInstructions(char * buffer, unsigned int bufferSize)
{
	if (!hasFreeSpace(bufferSize)) return -1; 

	Segment oldFree = lastIndex.retrieve();

	//insert the process data to memory
	for (size_t i = oldFree.BR; i < bufferSize+oldFree.BR; i++)
	{
		this->memory[i] = buffer[i - oldFree.BR];
	}

	 //update memory status
	ListItr<Segment> newItr = lastIndex;

	//process is added to memory
	memoryList.insert(Segment(oldFree.BR, bufferSize, MEMORY_FULL), lastIndex);
	newItr.advance();
	if (bufferSize < oldFree.size)
	{
		//remaining free space (if any)
		memoryList.insert(Segment(oldFree.BR + bufferSize, oldFree.size - bufferSize, MEMORY_EMPTY), newItr);
	}
	 //remove old information about the empty space
	lastIndex = newItr;
	memoryList.remove(oldFree);

	//log the memory config to output file
	output.open(OUTPUT_FILE_NAME, ios::app); //open output file in append mode
	output << "****************************************************************\n"
		<< memoryList << "\n";
	output.close();

	return lastIndex.retrieve().BR;
}

void Memory::removeInstructions(int bufferSize, int BR)
{
	Segment toBeRemoved = Segment(BR, bufferSize, MEMORY_FULL);

	ListItr<Segment> prev = memoryList.findPrevious(toBeRemoved);

	ListItr<Segment> current = prev;
	current.advance();

	ListItr<Segment> next = current;
	if (!next.isPastEnd() && !next.isLast()) next.advance(); //try to go the next segment in the list

	if (next.retrieve() != toBeRemoved && next.retrieve().state == MEMORY_EMPTY)
		//merge if next segment is also empty
	{
		Segment s = next.retrieve();

		memoryList.insert(Segment(current.retrieve().BR, s.size + current.retrieve().size), current); // insert merged empty space to list

		//remove old next&current, both next and current now points to new segment
		ListItr<Segment> temp = current;
		current.advance();
		memoryList.remove(temp.retrieve());
		memoryList.remove(next.retrieve());
		next = current;
	}
	if (prev.retrieve() != memoryList.zeroth().retrieve() && prev.retrieve().state == MEMORY_EMPTY)
		//Merge if previous segment is also empty
	{
		Segment s = prev.retrieve();
		
		memoryList.insert(Segment(s.BR, s.size + current.retrieve().size), prev); //insert merged emty space to list

		//remove prev & current (they are replaced by new space), now both prev and current points to new segment
		ListItr<Segment> temp = prev;
		prev.advance();
		memoryList.remove(temp.retrieve());
		memoryList.remove(current.retrieve());
		current = prev;
	}
	if (current.retrieve().state == MEMORY_FULL)
		//none of the neighbours are empty (none of the above conditions are executed)
	{
		ListItr<Segment> temp = current;
		memoryList.insert(Segment(BR, bufferSize, MEMORY_EMPTY), temp);
		current.advance();
		memoryList.remove(temp.retrieve());
	}
	lastIndex = current;

	//log the memory config to output file
	output.open(OUTPUT_FILE_NAME, ios::app); //open output file in append mode
	output << "****************************************************************\n"
		<< memoryList << "\n";
	output.close();
}

char* Memory::getInstruction(int PC, int BR){
	char * instruction = new char[4];
	instruction[0]=(unsigned int)memory[PC+BR];
	instruction[1]=(unsigned int)memory[PC+BR+1];
	instruction[2]=(unsigned int)memory[PC+BR+2];
	instruction[3]=(unsigned int)memory[PC+BR+3];

	return instruction;
}

#endif