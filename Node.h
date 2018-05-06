#ifndef INCLUDED_NODE
#define INCLUDED_NODE
#include<iostream>
using namespace std;

template <class T>
class Node{
  //CI:data is defined, next is defined as NULL or points to another defined Node object.
 private:
  T data; //holds the data element in this node
  Node <T> * next; //is either null or points to another Node object.
  Node <T> * previous; //is either null or points to another Node object

public:
  //************************************
  //Start Constructors
  //************************************

  //PRE:None
  //POST:Initialzes the pointer next to NULL and sets the data value to 0 which is the default when data is not given in the constructor.
  Node(){
    next = NULL;
    data = 0;//default value data will be set too if the default constructor is called
    previous = NULL;
  };


  //PRE:pData is of type T which is given at compile time.
  //POST:initiaze the pointer next to NULL and the value of data to the given pData value in the constructor parameter.
  Node(T pData){
    next = NULL;
    data = pData;
    previous = NULL;
  };


  //Pretty Sure I dont need copy constructor for node class.
  /*
  //PRE:
  //POST:makes a deep copy of the node;
  Node(const Node & pNode){
    data = pNode.data;
    next = pNode.next;
  }
  */
  //************************************
  //End Constructors
  //************************************


  //************************************
  //Start Destructor
  //************************************

    


  //************************************
  //End Destructor
  //************************************

  
  
  //************************************
  //Start Member Functions
  //************************************
  //PRE:none
  //POST:returns data
  T getData(){
    return data;
  };


  //PRE:pointer is a pointer to a Node of type T
  //POSt:makes this objects next point to another node.
  void setNext(Node <T> * pointer){
    next = pointer;
  };


  
  //PRE:pointer is a pointer to a Node of type T
  //POSt:makes this objects previous point to another node
  void setPrevious(Node <T> * pointer){
    previous = pointer;
  };


  //PRE:Node satisfies CI
  //POST:returns the pointer to the next node.
  Node <T> * getNext(){
    return next;
  };

  //PRE:Node satisfies CI
  //POST:returns the pointer to the previous node.
  Node <T> * getPrevious(){
    return previous;
  };
  //************************************
  //End Member Functions
  //************************************
  


};
#endif
