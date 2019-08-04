/*
* Created on: Nov 1, 2015
*	  Author: aozdemir
*/
#include "LinkedList.h"

//List
template<class Object>
List<Object>::List()
{
	header = new ListNode<Object>;
}

template<class Object>
List<Object>::List(const List & rhs)
{
	header = new ListNode<Object>;
	*this = rhs;  // Deep Copy
}

template<class Object>
List<Object>::~List()
{
	makeEmpty();  
	delete header;  
}

template<class Object>
bool List<Object>::isEmpty() const
{
	return header->next == NULL;
}

template<class Object>
void List<Object>::makeEmpty()
{
	while (!isEmpty())
		remove(first().retrieve());
}

//returns a pointer to the dummy header of list
template<class Object>
ListItr<Object> List<Object>::zeroth() const
{
	return ListItr<Object>(header);
}

//returns a pointer to the first object of the list
template<class Object>
ListItr<Object> List<Object>::first() const
{
	return ListItr<Object>(header->next);
}

template<class Object>
void List<Object>::insert(const Object & x, const ListItr<Object>& p)
{
	if (p.current != NULL)
		p.current->next = new ListNode<Object>(x, p.current->next);

}

//returns a pointer to the element in the list, NULL if not found
template<class Object>
ListItr<Object> List<Object>::find(const Object & x) const
{
	ListNode<Object> *itr = header->next;

	while (itr != NULL && itr->element != x)
		itr = itr->next;

	return ListItr<Object>(itr);
}

//retruns a pointer to the preceding element of the searched element, returns Last element if not found
template<class Object>
ListItr<Object> List<Object>::findPrevious(const Object & x) const
{
	ListNode<Object> *itr = header;

	while ((itr->next != NULL) && itr->next->element != x )
	itr = itr->next;

	return ListItr<Object>(itr);

}

template<class Object>
void List<Object>::remove(const Object & x)
{
	ListItr<Object> p = findPrevious(x);

	if (p.current->next != NULL)
	{
		ListNode<Object> *oldNode = p.current->next;
		p.current->next = p.current->next->next;  // Bypass deleted node
		delete oldNode;
	}
}

template<class Object>
int List<Object>::GetSize()
{
	int count=0;
	ListNode<Object> *temp = header->next;
	while (temp)
	{
		count++;
		temp = temp -> next;
	}
	return count;
}

template<class Object>
const List<Object> & List<Object>::operator=(const List<Object> & rhs)
{
	if (this != &rhs)
	{
		makeEmpty();

		ListItr<Object> ritr = rhs.first();
		ListItr<Object> itr = zeroth();
		for (; !ritr.isPastEnd(); ritr.advance(), itr.advance())
			insert(ritr.retrieve(), itr);
	}
	return *this;

}


//ListItr
template<class Object>
ListItr<Object>::ListItr()
{
	current = NULL;
}

template<class Object>
bool ListItr<Object>::isPastEnd() const
{
	return current == NULL;
}

template<class Object>
bool ListItr<Object>::isLast() const
{
	return current->next == NULL;
}

template<class Object>
void ListItr<Object>::advance()
{
	if (!isPastEnd()) current = current->next;
}

template<class Object>
const Object & ListItr<Object>::retrieve() const
{
	try {
		if (isPastEnd()) throw BadIterator();
	}catch(BadIterator()){
		cout << "We have just gone off the deep end of a list, byeee"
			<< endl;
	}
	return current->element;
}

template <class Object>
ostream & operator<<(ostream & stream, const List<Object> l)
{
	if (!l.isEmpty())
	{
		ListItr<Object> itr = l.first();
		for (; !itr.isPastEnd() ; itr.advance())
		{
			stream << itr.retrieve() << endl;
		}
	}
	return stream;
}
