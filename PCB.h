#ifndef INCLUDED_PCB
#define INCLUDED_PCB
#include <fstream>
#include "Constants.h"
#include "MyString.h"
#include "ProcessPageInfo.h"
#include "LinkedList.h"
using namespace std;
//CLASS CI:This represents an individual programs PC and registers as
//well as its starting address of the program and starting address of
//the stack space, the name of the program and stack pointer for program
class PCB{
 private:
  uint PC;//represents a 
  uint registers[NUMREGISTERS];//registers[0] is set to 0 on
			       //construction
  uint progStartAddress;//starting address of program
  uint progEndAddress;//ending address of program
  uint progNumWords;//numwords of memory program takes
  MyString progName;//will hold the name of the program
  uint stackStartAddress;//starting address of stack
  uint stackEndAddress;//ending address of stack
  uint stackNumWords;//numwords of memory stack takes
  uint SP;//stack pointer of program, will be the number of total
	  //words the prog and stack have together -1 *4
  uint PID;//PID of PCB;
  uint numTimeSlices;//keeps count of the number of timeslices
  uint numPageFaults;//keeps count of the number of pageFaults
  uint pageSize;//pageSize
  LinkedList <ProcessPageInfo *> VPNList;//list holding info about VPNs
  LinkedList <ProcessPageInfo *> stackVPNList;//list holding info about stackVPNs
  fstream * openFile;//file that holds bytes
  uint cursorStart;//represents cursor value where bytes start
 public:
  //constructor
  PCB();
  
  //Constructor
  //PRE:
  //POST:creates object
  PCB(uint pNumWords, MyString pName, uint sNumWords,uint pPid,
      uint pPageSize,fstream * pOpenFile, uint cursorByteStart);


  
  //PRE:outString is a MyString object
  //POST:puts all relevant information in the outString to be
  //output to the shell.
  void giveOutString(MyString & outString);
  
  
  //PRE:object is defined and satsifies the CI
  //POST:returns the PID of this object.
  uint getPID() const;
  
  
  //PRE:object is defined and satisfies the CI
  //POST:sets the PC in this PCB to be that of the paramemter mPC
  void setPC(uint mPC);
  
  
  //PRE:object is defined and satisfis the CI
  //POST:returns the PC of this object
  uint getPC() const;
  
  //PRE:takes a uint array that should represent the registers from the
  //machine class
  //POST:gives the register from the machine class the values of the
  //registers in the PCB
  void giveRegisters(uint machineRegs[]);
  
  //PRE:takes a uint array that should represent the registers from the
  //machine class
  //POST:takes the values of the registers from the machine class and
  //puts them in their own registers
  void takeRegisters(uint machineRegs[]);
  
  //PRE:object is defined and satisfies the CI
  //POST:returns the SP of this object.
  uint getSP() const;
  
  //PRE:object is defined and satisfies the CI
  //POST:returns the number of words this program has
  uint getProgWords()const;
  
  //PRE:object is defined and satisfies the CI
  //POST:returns the number of words the stack this prog has
  uint getStackWords()const;
  
  //PRE:object is defined and satisfies the CI
  //POST:returns the starting address of program
  uint getProgStart()const;
  
  
  //PRE:object is defined and satisfies the CI
  //POST:returns the starting address of stack
  uint getStackStart()const;
  
  //PRE:object is defined and satisfies the CI
  //POST:returns the ending address of program
  uint getProgEnd()const;
  
  
  //PRE:object is defined and satisfies the CI
  //POST:returns the ending address of stack
  uint getStackEnd()const;
  
  //PRE:outString is a MyString object that will hold the information of this PCB
  //POST:gives outString the relevant information on this PCB
  void addInfo(MyString & outString);

  //PRE:object exists satisfies the CI
  //POST:iterates numTimeSlices by one
  void addTimeSlice();

  //PRE:object exists and satisfies the CI
  //POST:iterates numPageFaults by one
  void addPageFault();

  //PRE:object exists and satisfies the CI
  //POST:returns numTimeSlices
  uint getNumTimeSlices();
  
  
  //PRE:object exists and satisfies the CI
  //POST:returns numTimeSlices
  uint getNumPageFaults();

  //PRE:object satisfies the CI
  //POST:updates the ProcessPageTable
  void updateProcessPageTable(uint VPN,bool stackBool,bool used,uint PPN);


  //PRE:object satisfies the CI
//POST:returns the openFile for this object.
  fstream * getFile();

//PRE:obj satisfies the CI
//POST:returns the cursorStart for openFile
  uint getCursorStart();


  //PRE:object satisfies the CI
  //POST:returns a boolean, true if the VPN we specify is in memory in
  //either the stack or memory processpages according to the stack
  //boolean, if we return true we also return by reference the PPN
  bool isInMem(uint VPN,bool stack, uint & PPN);

  
  //destructor
  ~PCB();
  
};
#endif
