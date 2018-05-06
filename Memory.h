#ifndef INCLUDED_MEMORY
#define INCLUDED_MEMORY
#include "Constants.h"
#include <iostream>
#include <fstream>
#include "MyString.h"
using namespace std;

class Memory{
  //CLASS INVARIANT: memArray is of size maxWords,stepbool and runbool are initialzed to false
  
 private:
  uint maxWords;
  uint freeWords;
  uint currGarbWord;
  uint numWordsUsed;//stores the number of words loaded into prog must
		    //deiterate when we kill or delete programs, and
		    //iterate when we load programs. 
  uint * memArray;
  uint SP; // The stack pointer register for memory
  uint PC; //The program counter for memory.

  bool stepBool;//initialze to false
  bool runBool;//initialze to false
  
 public:
  //constructor
  Memory();

  //constructor
  //PRE:
  //POST:
  Memory(uint pMaxWords);

  //PRE:memory array has been created, object is defined
  //POST:changes each index in the memory array to 0 so it is 32 bits of 0.
  void initialzeMemory(uint index1, uint index2);


  //PRE:index is the index in the memory array that unsignedint ch
  //will go to and shiftcounter represents how much the bit should be
  //shifted to the left before inputting it into memory.
  //POST:modifies 1 byte in memory.
  void shiftAndInput(uint index, uint shiftCounter, uint ch);
  
  //PRE:inputFile is the name of a readable input file, done and error
  //represent booleans that can be changed to represent whether the
  //function is done or not
  //POST:loads the contents of the program into memory.
  void loadProgram(ifstream & inputFile, bool & done, bool & error);

  //PRE:inputfile has been loaded into memory, the object is defined
  //POST:returns numWords
  uint getMem();

  //PRE:a program has been loaded into memory, num1 and num2 are valid numbers
  //POST:displays to the shell the contents of memory starting at num1
  //and ending at num2
  void displayMemory(uint num1, uint num2, bool & done, bool & error, MyString & outString);
  
  //PRE:object is defined
  //POST:returns stack pointer register.
  uint getSP();

  //PRE:object is defined
  //POST:sets SP in memory class to be the value specified
  void setSP(uint value);
  
  //PRE:PC is the program counter
  //POST:returns the word in memory that the PC is at
  uint getInstruction(uint PC);

  //PRE:wordNum is the index of the word in the memory array that we
  //want to get
  //POST:returns the word of memory
  uint getWord(uint wordNum);

  
  //PRE:wordNum is the index of the word in the memory array that we are
  //replacing with replacementWord
  //POST:replaces the word in memArray[wordNum] with replacementWord
  void replaceWord(uint wordNum, uint replacementWord);

  //PRE:object is defined and satisfies the CI
  //POST:returns the number of words occupied in memory.
  uint getNumWordsUsed();
  
  //destructor
  ~Memory();
};


#endif
