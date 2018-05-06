#ifndef INCLUDED_LINKEDLIST
#define INCLUDED_LINKEDLIST
#include "Node.h"
#include <iostream>
#define uint unsigned int
using namespace std;

//Basing this list on an array, where if the numNodes is 1 the first node is at position 0
template <class T>
class LinkedList{
  //CI:head is either NULL or pointing to a node object, numNodes is either 0  or the number of nodes in the list.
 protected:
  Node <T> * head;//Pointer to the first node in the linked list.
  Node <T> * tail;//Pointer to the end of the linkedList
  uint numNodes;//number of nodes in the linked list

 public:

  //************************************
  //Start Constructors
  //************************************

  //PRE:
  //POST:initialzes the numNodes to 0 and initialzes the pointer head to NULL
  LinkedList(){
    numNodes = 0;
    head = NULL;

  };

  //PRE:pLinkedList satisfies CI
  //POST:creates a deep copy of pLinkedList to use in function.
  LinkedList(const LinkedList <T> & pLinkedList){
    uint number = pLinkedList.numNodes;
    uint index = 0;
    for(index; index < number; index++){
      addToBack(pLinkedList.getNth(index));
    }
  };

  /*
  //PREpLinkedList satisfies CI
  //POST:creates a deep copy of pLinkedList to use in function
  //if T is a pointer //ask shende how this would work
  LinkedList(const LinkedList <T *> & pLinkedList){
    int number = pLinkedList.numNodes;
    int index = 0;
    for(index; index < number; index++){
      
    }
  }
  */
  //************************************
  //End Constructors
  //************************************
  

  //************************************
  //Start Destructor
  //************************************

  //PRE:LinkedList goes out of scope
  //POST:deletes all nodes in linked list.
  ~LinkedList(){
    //iterate through the linkedlist and delete nodes accordingly, so use remove function youll have to make maybe?
    while(numNodes != 0){
      deleteFirst();
    }//ASSERT all the Nodes in the linked list have been deallocated.
  };

  //************************************
  //End Destructor
  //************************************

  
  
  //************************************
  //Start Member Functions
  //************************************

  //PRE:Node is correctly defined according to CI and holds data of type T
  //POST:returns the data in the first node
  T getHead() const {
    if(numNodes != 0){
      T returnData = head->getData();
      return returnData;
    }
    else{
      cout<<"This LinkedList is empty and has no first Node"<<endl;
    }
  };

  //PRE:
  //POST:returns the data in the last node
  T getTail() const {
    if(numNodes != 0){
      T returnData = tail->getData();
      return returnData;
    }
    else{
      cout<<"This LinkedList is empty and has no last Node"<<endl;
    }
  };
  //PRE:LinkedList satisfies CI and data is an object of type T which is defined at compile
  //POST:creates a new Node at the head of the linked list with data in it.
  void addToFront(T data){
    if(numNodes == 0){
      head = new Node<T>(data);
      tail = head;
      numNodes ++;
      //ASSERT:numNodes has been iterated and head is equal to the new Node as well as tail is.
    }
    else{
      Node <T> * previousHead = head;
      head = new Node<T>(data);
      head->setNext(previousHead);
      previousHead->setPrevious(head);
      numNodes ++;
      //ASSERT:numNodes has been iterated and head has been changed correctly
    }
  };

  //PRE:LinkedList satisfies CI and data is of type T which is definedat compile
  //POST:creates a new node at the tail of the linked list with data in it.
  void addToBack(T data){
    if(numNodes == 0){
      head = new Node<T>(data);
      tail = head;
      numNodes ++;
      //ASSERT:numNodes has been iterated and head and tail exist as pointers to the first node
    }
    else{
      Node <T> * newTail = new Node<T>(data);
      tail->setNext(newTail);//make previous tail point to newTail
      newTail->setPrevious(tail);//make newTail previous be old tail.
      tail = newTail;//make tail now equal to newTail
      numNodes ++;
      //ASSERT:the previous tail now points to a new tail.
      
    }
  };


  //PRE:n is the number of the node you want to insert a new node at,
  //while data is  the data you want in the new node
  //POST:inserts the data at index n
  void insertNth(uint n, T data){
    if(n == 0){
      //ASSERT:we are adding this data to the front of the linkedlist 
      addToFront(data);
    }
    else{
      //ASSERT:we are inserting somewhere other than the front of the
      //list
      Node <T> * newNode = new Node<T>(data);
      Node <T> * cursor = head;
      Node <T> * prevCursor;
      for(uint index = 0; index < n; index ++){
	prevCursor = cursor;
	cursor = cursor->getNext();
      }
      //ASSERT:want to place newNode inbetween cursor and prevCursor
      prevCursor->setNext(newNode);
      cursor->setPrevious(newNode);
      numNodes ++;
      //ASSERT:newNode is linked inbetween prevCursor and cursor
    }

  };
  
  //PRE:N is the number of the node you want to get data out of, N>=0 
  //POST:returns the data in the Nth node
  T getNth(uint N) const {
    if(N > numNodes){
      cout<<"The number of the Node that has been inputted does not exist"<<endl;
    }
    else{//ASSERT:N is less than or equal to NumNodes.
      Node <T> * cursor = head;
      for(uint index = 0; index < N; index++){
	cursor = cursor->getNext();//ASSERT:cursor is equal to the next Node in the list.
      }
      return cursor->getData();
    }
  };


  //PRE:
  //POST:deletes the first element in LinkedList
  void deleteFirst(){
    if(numNodes == 0){
      cout<<"There is no node to delete"<<endl;
    }
    else if(numNodes == 1){
      delete head;
      head = NULL;
      tail = NULL;
      numNodes --;
      //ASSERT:Node at head has been deleted and head and tail have been initialzed to NULL and numNodes has been deiterated
    }
    else{
      Node <T> * nextHead = head->getNext();
      Node <T> * null = NULL;
      delete head;
      head = nextHead;
      head->setPrevious(null);
      numNodes --;
      //ASSERT:Node at head has been deleted and the Node after the original head is what head now points too and numNodes has been deiterated
    }
  };

  //PRE:
  //POST:deletes the last element in LinkedList
  void deleteLast(){
    if(numNodes == 0){
      cout<<"There is no node to delete"<<endl;
    }
    else if(numNodes == 1){
      delete head;
      head = NULL;
      tail = NULL;
      numNodes --;
      //ASSERT:head has been deleted and head and tail have been initialzed to NULL and NumNodes has been deiterated
    }
    else{
      Node <T> * newTail = tail->getPrevious();
      Node <T> * null = NULL;
      delete tail;//ASSERT tail node is now deleted
      tail = newTail;//ASSERT tail is equal to the previous pointer of tail.
      tail->setNext(null);//ASSERT Set new tail next to Null
      numNodes--;
      //ASSERT:last node has been deleted and the tail is now equal to the node before that and numNodes has been deiterated
    }
  };


  //PRE:N is the number of the node to delete
  //POST:deletes the Nth node.
  void deleteNth(uint N){
    if(N > numNodes){
      cout<<"There is no Node to delete, the parameter is too large"<<endl;
    }
    else if(N == 0){//ASSERT:wants to delete first node in list
      deleteFirst();
    }
    else if(N == numNodes - 1){//Assert:wants to delete last node in list
      deleteLast();
    }
    else{
      Node <T> * cursor = head;
      Node <T> * null = NULL;
      for(uint index = 0; index < N; index ++){
	cursor = cursor->getNext();
      }//ASSERT:Cursor is at Nth node
      Node <T> * beforeNode = cursor->getPrevious();//this is equal to the previous of the current node.
      Node <T> * afterNode = cursor->getNext();//this is equal to the next of the current node

      beforeNode->setNext(afterNode);
      afterNode->setPrevious(beforeNode);
      //now the node that cursor is pointing too isnt in the linkedlist at all
      delete cursor;
      numNodes --; 
    }
  };


  //PRE:
  //POST:returns the number of nodes in the linkedlist
  uint getNumNodes() const {
    return numNodes;

  };

  
  //************************************
  //End Member Functions
  //************************************
  


  
  //************************************
  //START OVERLOADED OPERATORS
  //************************************

  //PRE:both objects satisfy CI and are of same type
  //POST:makes a deep copy of explicit equal to the implicit
  LinkedList <T> & operator = (const LinkedList <T> & pLinkedList){
    if(&pLinkedList != this){
      uint number = pLinkedList.numNodes;
      uint index = 0;
      for(index; index < number; index++){
	addToBack(pLinkedList.getNth(index));
      }
    }
    return (*this);
  };


  //PRE:both objects satisfy the CI and are of the same type
  //POST:adds explicit linkedlist to the back of the implicit and returns it as another linkedlist object.
  LinkedList <T> operator + (const LinkedList <T> & pLinkedList) const{
    LinkedList <T> temp;
    for(uint index1 = 0; index1 < numNodes; index1++){
      temp.addToBack(getNth(index1));
    }
    uint number = pLinkedList.numNodes;
    for(uint index2 = 0; index2 < number; index2++){
      temp.addToBack(pLinkedList.getNth(index2));
    }
    
    return temp;
    
  };


  //PRE:both objects satisfy the CI and are of same type
  //POST:returns true if the member data of both linkedlist are the same, false otherwise 
  bool operator == (const LinkedList <T> & pLinkedList) const {
    bool returnVal = true;
    if(numNodes == pLinkedList.numNodes){
      uint index = 0;
      while((returnVal) && (index < numNodes)){
	if(getNth(index) == pLinkedList.getNth(index)){
	  index++;
	}
	else{
	  returnVal = false;//ASSERT:node in the same index is not the same
	}
      }
    }
    else{
      returnVal = false;//ASSERT:numnodes are not the same
    }
    return returnVal;
  };

  //PRE:LinkedList satisfies the CI
  //POST:prints LinkedList data
  friend ostream & operator << (ostream & stream, const LinkedList <T> & pLinkedList){
    if(pLinkedList.numNodes == 0){
    }
    else{
      uint index = 0;
      while(index < pLinkedList.numNodes){
	stream << pLinkedList.getNth(index) << ",";
	index++;
      }//ASSERT:all LinkedList Nodes data have been printed.
    }
    cout<<endl;
    return (stream);
  };
  //************************************
  //END OVERLOADED OPERATORS
  //************************************




};
#endif
