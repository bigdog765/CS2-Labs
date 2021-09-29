	//--------------------------------------------------------------------
//  Laboratory 5                                        ListLinked.cpp
//
//  *** SOLUTION: Linked list implementation of the List ADT ***
//
//  Updated 2008/03-12
//--------------------------------------------------------------------

#ifndef LISTLINKED_CPP
#define LISTLINKED_CPP

using namespace std;

#include <iostream>			// For showStructure

#include "ListLinked.h"

//--------------------------------------------------------------------
// NOTE: because ListNode is an inner class, references to ListNode do not take
// the <DataType> template qualifier. If it were a separate external class, then
// the <DataType> would be required.
// We switched from an external class in the 2nd edition to an inner class in
// order to demonstrate better OO design, as well as to get away from the ugly use of
// a private constructor in ListNode and "friend" to allow the List class to access 
// the ListNode's private data.
//--------------------------------------------------------------------

template <typename DataType>
List<DataType>::ListNode::ListNode(const DataType& nodeData,
				   ListNode* nextPtr)
// Creates a list node containing item elem and next pointer
// nextPtr.

    : dataItem(nodeData), next(nextPtr)
{
}

//--------------------------------------------------------------------

template <typename DataType>
List<DataType>::List(int ignored)

// Creates an empty list. The argument is included for compatibility
// with the array implementation (maxSize specifier) and is ignored.

    : head(0), cursor(0)
{
}

//--------------------------------------------------------------------

template <typename DataType>
List<DataType>::List(const List& other)
    : head(0), cursor(0)

// Copy constructor. Creates a list which is equivalent in content
// to the "other" list.

{
    operator=(other);
}

//--------------------------------------------------------------------

template <typename DataType>
List<DataType>& List<DataType>::operator=(const List& other)

// Overloaded assignment operator. Reinitializes the list to be 
// equivalent in content to the "other" list.
// Note: we include self-assignment protection by checking whether
// "this" object is identical to the "other" object.

{
    if( this != &other ) {
	clear();
	ListNode *otherPtr = other.head;
	ListNode *holdCursor = 0;

	while( otherPtr != 0 ) {
	    insert(otherPtr->dataItem);
	    if(otherPtr == other.cursor) {
		holdCursor = cursor;
	    }
	    otherPtr = otherPtr->next;
	}
	cursor = holdCursor;
    }

    return *this;
}

//--------------------------------------------------------------------

template <typename DataType>
List<DataType>::~List() 

// Destructor. Frees the memory used by a list.

{
    clear();
}

//--------------------------------------------------------------------

template <typename DataType>
void List<DataType>::insert(const DataType& newDataItem) throw (logic_error)

// Inserts newDataItem after the cursor. If the list is empty, then
// newDataItem is inserted as the first (and only) item in the list.
// In either case, moves the cursor to newDataItem.

{
    if ( isEmpty() ) {		// List is empty: could use "if ( cursor == 0 )"
		head = new ListNode(newDataItem, 0);
		cursor = head;
    } 
	else {
		ListNode *temp = new ListNode(newDataItem, cursor->next);
        cursor->next = temp;
        cursor = cursor->next;
    }
}

//--------------------------------------------------------------------

template <typename DataType>
void List<DataType>::remove() throw (logic_error) 

// Removes the item marked by the cursor from a list. Moves the
// cursor to the next item in the list. Assumes that the first list
// item "follows" the last list item.

{
    ListNode *p,   // Pointer to removed node
             *q;   // Pointer to prior node

    // Requires that the list is not empty
    if ( head == 0 )
        throw logic_error ("Trying to Remove from empty list.");

    if ( cursor == head )             // Remove first item. Adjust head
    {
       p = head;
       head = head->next;
       cursor = head;
    }
    else if ( cursor->next != 0 )     // Remove middle ite
    {
       p = cursor->next;
       cursor->dataItem = p->dataItem;
       cursor->next = p->next;
    }
    else                              // Remove last item. Reset cursor to head
    {
       p = cursor;
       for ( q = head ; q->next != cursor ; q = q->next )
           ;
       q->next = 0;
       cursor = head;
    }

    delete p;
}

//--------------------------------------------------------------------

template <typename DataType>
void List<DataType>::replace(const DataType& newDataItem) throw (logic_error)

// Replaces the item marked by the cursor with newDataItem and
// leaves the cursor at newDataItem.

{
    // Requires that the list is not empty
    if ( isEmpty() )
	throw logic_error("Trying to replace item in empty list\n");

    cursor->dataItem = newDataItem;
}

//--------------------------------------------------------------------

template <typename DataType>
void List<DataType>::clear() 

// Removes all the items from a list. Sets head and cursor to 0.

{
    cursor = head;
    while ( cursor != 0 )
    {
        head = head->next;
        delete cursor;
        cursor = head;
    }

    // Invariant: whether or not the while loop was entered,
    // both head and cursor == 0 at this point.
    // Could explicitly set head = cursor = 0, but not needed.
}

//--------------------------------------------------------------------

template <typename DataType>
bool List<DataType>::isEmpty() const 

// Returns true if a list is empty. Otherwise, returns false.

{
    return head == 0;
}

//--------------------------------------------------------------------

template <typename DataType>
bool List<DataType>::isFull() const 

// Returns true if a list is full. Otherwise, returns false.
//
// Note: determining whether the free store/heap would be able to
// satisfy the request to allocate another ListNode is either ugly,
// non-portable, or both. We explain the issue to our students and
// tell them that it is acceptible to simply return false. The main
// reason we include isFull() is for compatibility with the array-based
// implementation of the list. You may choose to instruct your students
// to use a different implementation.

{
    return false;		// See note above and alternative below.

    /*
    // One alternative implementation: Not really recommended, but
    // might have some pedagogic value.
    // Below is a somewhat hacked way to test if the list is full.
    // If a node can be successfully allocated than the list is not
    // full.  If the allocation fails it is implied that there is no
    // more free memory therefore the list is full.
    
    DataType testDataItem;
    ListNode *p;
    
    try
    {
        p = new ListNode(testDataItem, 0);
    }
    catch ( bad_alloc &e )
    {
        return true;
    }

    delete p;
    return false;

    // End of alternative implementation.
    */
}

//--------------------------------------------------------------------

template <typename DataType>
void List<DataType>::gotoBeginning() throw (logic_error)

// If a list is not empty, then moves the cursor to the beginning of
// the list. If list is empty, throws logic error.

{
    if (isEmpty()) {
	throw logic_error("Cannot move to beginning of empty list");
    }

    cursor = head;
}

//--------------------------------------------------------------------

template <typename DataType>
void List<DataType>::gotoEnd() throw (logic_error)

// If a list is not empty, then moves the cursor to the end of the
// list. Otherwise, throws logic_error.

{
    if (isEmpty()) {
	throw logic_error("Cannot move to end of empty list");
    }

    while (cursor->next) {	// or could use  "(cursor->next != 0)"
	cursor = cursor->next;
    }
}

//--------------------------------------------------------------------

template <typename DataType>
bool List<DataType>::gotoNext() throw (logic_error)

// If the cursor is not at the end of a list, then moves the
// cursor to the next item in the list and returns true. Otherwise, 
// leaves cursor unmoved and returns false.

{
    if (isEmpty()) {
	throw logic_error("Cannot move to next item in an empty list");
    }

    if( cursor->next != 0 ) {
	cursor = cursor->next;
	return true;
    } else {
	return false;
    }
}

//--------------------------------------------------------------------

template <typename DataType>
bool List<DataType>::gotoPrior() throw (logic_error)

// If the cursor is not at the beginning of a list, then moves the
// cursor to the preceeding item in the list and returns true.
// Otherwise, returns false.

{
    if (isEmpty()) {
	throw logic_error("Cannot move to prior item in an  empty list");
    }

    if (cursor == head) {
	return false;
    }

    ListNode* temp = cursor;
    for (cursor = head; cursor->next != temp; cursor = cursor->next)
	/* Do nothing. All necessary work done in increment step above. */ ;

    return true;
}

//--------------------------------------------------------------------

template <typename DataType>
DataType List<DataType>::getCursor() const throw (logic_error)

// Returns the item marked by the cursor. Requires that list is not empty.

{
    if (isEmpty()) {
	throw logic_error("Cannot get data at cursor in an empty list.");
    }

    return cursor->dataItem;
}

//--------------------------------------------------------------------

template <typename DataType>
void List<DataType>::showStructure() const

// Outputs the items in a list. If the list is empty, outputs
// "Empty list". This operation is intended for testing and
// debugging purposes only.

{
    if ( isEmpty() )
    {
       cout << "Empty list" << endl;
    } 
    else
    {
	for (ListNode* temp = head; temp != 0; temp = temp->next) {
	    if (temp == cursor) {
		cout << "[";
	    }

	    // Assumes that dataItem can be printed via << because
	    // is is either primitive or operator<< is overloaded.
	    cout << temp->dataItem;	

	    if (temp == cursor) {
		cout << "]";
	    }
	    cout << " ";
	}
	cout << endl;
    }
}

//--------------------------------------------------------------------
//
//                        Programming Exercises 2 and 3
//
//--------------------------------------------------------------------

// Programming exercise 2

template < typename DataType >
void List<DataType>:: moveToBeginning() throw ( logic_error )

// Removes the item marked by the cursor from a list and
// reinserts it at the beginning of the list. Moves the cursor to the
// beginning of the list.
{
   
   
}

//--------------------------------------------------------------------
// Programming exercise 3

template < typename DataType >
void List<DataType>:: insertBefore ( const DataType &newDataItem )
    throw ( logic_error )

// Inserts newDataItem before the cursor. If the list is empty, then
// newDataItem is inserted as the first (and only) item in the list.
// In either case, moves the cursor to newDataItem.
{
   
   
}


template < typename DataType >
void List<DataType>::moveForwardN(int length) throw (logic_error) { // move forward n spaces


}


template < typename DataType >
void List<DataType>::moveBackwardN(int length) throw (logic_error) { // move backward n spaces


}


#endif		// #ifndef LISTLINKED_CPP
