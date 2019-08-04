/*
* Created on: Nov 1, 2015
*	  Author: mbenlioglu
*/
#ifndef _LinkedList_H
#define _LinkedList_H

#include <iostream>

using namespace std;

//prototypes
/*****************/
template <class Object>
class List;

template <class Object>
class ListItr;

template <class Object>
class ListNode;
/*********************/

class  BadIterator {
public:
	BadIterator() {}
};

//List (with a dummy head)
template <class Object>
class List {
	template <class Object>
	friend ostream& operator<<(ostream&stream, const List<Object> l);
public:
	List();
	List(const List & rhs);
	~List();
	bool isEmpty() const;
	void makeEmpty();
	ListItr<Object> zeroth() const;
	ListItr<Object> first() const;
	void insert(const Object & x, const ListItr<Object> & p); //insert new object after p
	ListItr<Object> find(const Object & x) const;
	ListItr<Object> findPrevious(const Object & x) const;
	void remove(const Object & x);
	int GetSize();

	const List<Object> & operator=(const List<Object> & rhs);
private:
	ListNode<Object> *header;
};

//ListItr
template <class Object>
class ListItr {
public:
	ListItr();
	bool isPastEnd() const;
	bool isLast() const;
	void advance();
	const Object & retrieve() const;
	
private:
	ListNode<Object> *current;    // Current position

	ListItr(ListNode<Object> *theNode)
		: current(theNode) { }

	friend class List<Object>; // Grant access to constructor
};

//ListNode
template <class Object>
class ListNode
{
	ListNode(const Object & theElement = Object(), ListNode * n = NULL)
		: element(theElement), next(n) { }

	Object   element;
	ListNode *next;

	friend class List<Object>;
	friend class ListItr<Object>;
};

#include "LinkedList.cpp"

#endif
