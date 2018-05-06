#ifndef INCLUDED_QUEUE
#define INCLUDED_QUEUE
#include "LinkedList.h"
template <class T>

//Class CI:This object is derived from a LinkedList object

class Queue : public LinkedList<T> {
 private:
  //nothing should be here cause its all in LinkedList
 public:
  //Constructor
  //PRE:
  //POST:object is created,linkedList constructor is called
  Queue(){};

  //PRE:data is of type T, object is defined and satisfies the CI
  //POST:adds an object to the back of the queue
  void enQueue(T data){
    this->addToBack(data);
  };

  //PRE:object is defined and satisfies the CI
  //POST:removes the object from the begginging of the queue,calls
  //linkedlist deletfirst
  void deQueue(){
    this->deleteFirst();
  };

  //PRE:n is a number,object is defined and satisfies the CI
  //POST:deletes the nth object in the queue
  void deQueueNth(uint n){
    this->deleteNth(n);
  };
  
};
#endif
