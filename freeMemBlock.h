#ifndef INCLUDED_FREEMEMBLOCK
#define INCLUDED_FREEMEMBLOCK
#include "Constants.h"

//CLASS CI:numwords,startAddress and endaddress are defined ints

class freeMemBlock{
 private:
  uint startAddress;
  uint endAddress;
  uint numWords;

 public:

  //Constructor
  freeMemBlock();

  //Constructor:
  //PRE:none
  //POST:creates object with starting address, ending address, and numwords
  freeMemBlock(uint pStartAddress, uint pEndAddress, uint pNumWords);

  //PRE:object is defined
  //POST:returns the starting addresss
  uint getStart()const;

  
  //PRE:object is defined
  //POST:returns the starting addresss
  uint getEnd()const;


  
  //PRE:object is defined
  //POST:returns the starting addresss
  uint getNumWords()const;


  //PRE:object is defined,satisfes CI
  //POST:sets the starting address to be parameter.
  void setStart(uint address);

  //PRE:object is defined,satisfies the CI
  //POST:sets the ending address to be parameter
  void setEnd(uint address);

  //PRE:object is defined, satisfies the CI
  //POST:sets the numwords of this freememBlock to be num
  void setNumWords(uint num);

  //PRE:memBlocks start address must be 1 away from this freeMemory
  //blocks end address, or vice versa for this command to run
  //correctly
  //POST:changes the start and ending address as well as the number of
  //words of this memory block in accordance with memblock, returns
  //true if the objects were able to be spliced together, false
  //otherwise
  bool splice(freeMemBlock * memBlock);
  
  
};

#endif
