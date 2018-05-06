#include "Memory.h"
#include <iostream>
#include <fstream>
#include "Constants.h"
#include <stdio.h>
#include "MyString.h"
using namespace std;

//CLASS INVARIANT: memArray is of size maxWords,stepbool and runbool are initialzed to false
Memory::Memory(){
}

//PRE:
//POST:initialzes Memory object where maxWords = pMaxWords
Memory::Memory(uint pMaxWords){
  maxWords = pMaxWords;
  memArray = new uint[maxWords];
  currGarbWord = 0;
  stepBool = false;
  runBool = false;
}

//PRE:memory array has been created, object is defined
//POST:changes each index in the memory array to 0 so it is 32 bits of 0 that will be loaded for a program.
void Memory::initialzeMemory(uint index1, uint index2){
  for(int index = index1; index < index2; index++){
    if(index < maxWords){
      memArray[index] = 0;
    }
  }
  //ASSERT:all array[index] that are being allocated for program  = 0;
}


//PRE:index is the index in the memory array that unsignedint ch
//will go to and shiftcounter represents how much the bit should be
//shifted to the left before inputting it into memory.
//POST:modifies 1 byte in memory.
void Memory::shiftAndInput(uint index, uint shiftCounter, uint ch){
  ch = ch << shiftCounter;
  //ASSERT:ch has been shifted to the left the correct amount of bits
  //24 bits if it is the first round, 16 if the second, 8 if third, none if fourth
  uint input =  memArray[index] | ch;//memarray[index] should be all 0's
  memArray[index] = input;
  //ASSERT:since we used or operator, all 1's in ch will now be 1's in
  //memArray[index] and the 0's will stay 0's in memarray Index
}




//PRE:PC is the program counter
//POST:returns the word in memory that the PC is at
uint Memory::getInstruction(uint PC){
  uint rv = memArray[PC/FOUR];
  //ASSERT:rv holds the word in memory that PC is asking for
  return rv;
}

//PRE:object is defined
//POST:sets SP in memory class to be the value specified
void Memory::setSP(uint value){
  SP = value;
}
 


//PRE:inputfile has been loaded into memory, the object is defined
//POST:returns numWords
uint Memory::getMem(){
  return maxWords;
}


//PRE:a program has been loaded into memory, num1 and num2 are valid numbers
//POST:displays to the shell the contents of memory starting at num1
//and ending at num2
void Memory::displayMemory(uint num1, uint num2, bool & done, bool & error, MyString & outString){
  //ASSERT:num1 and num2 are valid numbers
  uint counter = 0;
  uint index = num1;
 
  while(index <= num2){
    char array[MAXINPUT];
    sprintf(array,"Address:%i, Hex:%#x(%i) ",index,
	    memArray[index/FOUR],memArray[index/FOUR]);
    //ASSERT:Address+contents in hex+contents in dec are stored in array
    index += FOUR;
    counter++;
    outString = outString + array;
    if(counter == FOUR){
      //ASSERT: we have put four things on a line
      counter = 0;
      outString.addChar(NEWLINE);
    }
  }
}




//PRE:object is defined
//POST:returns stack pointer register.
uint Memory::getSP(){
  return SP;
}

//PRE:wordNum is the index of the word in the memory array that we
//want to get
//POST:returns the word of memory
uint Memory::getWord(uint wordNum){
  uint rv = memArray[wordNum];
  return rv;
}

//PRE:wordNum is the index of the word in the memory array that we are
//replacing with replacementWord
//POST:replaces the word in memArray[wordNum] with replacementWord
void Memory::replaceWord(uint wordNum, uint replacementWord){
  memArray[wordNum] = replacementWord;
}


//PRE:object is defined and satisfies the CI
//POST:returns the number of words occupied in memory.
uint Memory::getNumWordsUsed(){
  return numWordsUsed;
}

Memory::~Memory(){
  delete memArray;
}
  
