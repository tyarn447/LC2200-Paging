#include "freeMemBlock.h"
#include "Constants.h"
#define ADDRESSITERATOR 4
//Constructor
freeMemBlock::freeMemBlock(){}

//Constructor:
//PRE:none
//POST:creates object with starting address, ending address, and numwords
freeMemBlock::freeMemBlock(uint pStartAddress, uint pEndAddress, uint pNumWords){
  startAddress = pStartAddress;
  endAddress = pEndAddress;
  numWords = pNumWords;
}

//PRE:object is defined
//POST:returns the starting addresss
uint freeMemBlock::getStart()const{
  return startAddress;
}


//PRE:object is defined
//POST:returns the starting addresss
uint freeMemBlock::getEnd()const{
  return endAddress;
}



//PRE:object is defined
//POST:returns the starting addresss
uint freeMemBlock::getNumWords()const{
  return numWords;
}



//PRE:object is defined,satisfes CI
//POST:sets the starting address to be parameter.
void freeMemBlock::setStart(uint address){
  startAddress = address;
}

//PRE:object is defined,satisfies the CI
//POST:sets the ending address to be parameter
void freeMemBlock::setEnd(uint address){
  endAddress = address;
}

//PRE:object is defined, satisfies the CI
//POST:sets the numwords of this freememBlock to be num
void freeMemBlock::setNumWords(uint num){
  numWords = num;
}


//PRE:memblock is a pointer to a freeMemory block
//POST:checks if memBlock can be spliced to this object and if it can
//it does it and returns true else it returns false, if true it 
//changes the start and ending address as well as the number of
//words of this memory block in accordance with memblock
bool freeMemBlock::splice(freeMemBlock * memBlock){
  bool rv = false;
  if((memBlock->getEnd() + ADDRESSITERATOR ) == startAddress){
    //ASSERT:the end of memblock is next to the start of this one
    //so change start of this to start of memblock
    setStart(memBlock->getStart());
    //ASSERT:memBlock has been spliced to this
    setNumWords(numWords + memBlock->getNumWords());
    //ASSERT:this has correct number of words now
    rv = true;
    //ASSERT:memblock has been spliced to this
  }
  if((endAddress + ADDRESSITERATOR) == memBlock->getStart()){
    //ASSERT:the end of this object is next to the start of memblock
    //so change end of this to end of memblock
    setEnd(memBlock->getEnd());
    //ASSERT:memblock has been spliced to this
    setNumWords(numWords + memBlock->getNumWords());
    //ASSERT:this has correct number of words now
    rv = true;
    //ASSERT:memblock has been spliced to this
  }
  return rv;
  //ASSERT:returns true if objects were spliced, false otherwise
}
