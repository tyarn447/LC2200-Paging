#include "PCB.h"
#include "MyString.h"
#include "ProcessPageInfo.h"
#include "LinkedList.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;


  //constructor
PCB::PCB(){}

  //Constructor
  //PRE:
  //POST:creates object
PCB::PCB( uint pNumWords, MyString pName,uint sNumWords,uint pPID,
	  uint pPageSize,fstream * pOpenFile,uint pCursorStart){
  PC = 0;
  registers[0] = 0;//$zero is 0 now
  //progStartAddress = pStartAd;
  //progEndAddress = pEndAd;
  progNumWords = pNumWords;
  progName = pName;
  //stackStartAddress = sStartAd;
  //stackEndAddress = sEndAd;
  stackNumWords = sNumWords;
  PID = pPID;//set PID;
  //SP = (progNumWords + stackNumWords - 1) * FOUR;
  //registers[STACKPOINTER] = SP;
  numTimeSlices = 0;
  numPageFaults = 0;
  pageSize = pPageSize;
  uint VPNLen = 0;
  if(progNumWords%pageSize == 0){
    //ASSERT:pagesize fits perfectly with progNumWords
    VPNLen = progNumWords/pageSize;
  }
  else{
    //ASSERT:pageSize doesnt fit perfectly with progNumWords
    VPNLen = progNumWords/pageSize;
    VPNLen += 1;
  }
  //found len, now initialze VPN List
  for(uint index = 0; index < VPNLen; index++){
    VPNList.addToBack(new ProcessPageInfo(index));
  }//ASSERT:VPNList holds all possible Virtual Pages for this prog
  uint stackVPNLen = stackNumWords/pageSize;
  for(uint index = 0; index < stackVPNLen;index++){
    stackVPNList.addToBack(new ProcessPageInfo(index));
  }//ASSERT:stackVPNList holds pages for all stacks for this PCB
  openFile = pOpenFile;
  cursorStart = pCursorStart;
  //so when u want to get to a certain byte do .seekp(cursorStart + byteNum, ios::beg)
  //that will get you to bytenum bytenum+cursorStart
}


//PRE:outString is a MyString object
//POST:puts all relevant information in the outString to be
//output to the shell.
void PCB::giveOutString(MyString & outString){
  char array[GIVEOUTSTRING];
  outString = outString + progName;
  sprintf(array," PID:%i, PC:%i, ProgNumWords:%i \n Number of Timeslices:%i, Number of PageFaults:%i",PID,PC,progNumWords,numTimeSlices,numPageFaults);
  outString = outString + array;
}

//PRE:object is defined and satsifies the CI
//POST:returns the PID of this object.
uint PCB::getPID() const{
  return PID;
}

//PRE:object is defined and satisfies the CI
//POST:sets the PC in this PCB to be that of the paramemter mPC
void PCB::setPC(uint mPC){
  PC = mPC;
}

//PRE:object is defined and satisfis the CI
//POST:returns the PC of this object
uint PCB::getPC() const{
  return PC;
}

//PRE:takes a uint array that should represent the registers from the
//machine class
//POST:gives the register from the machine class the values of the
//registers in the PCB
void PCB::giveRegisters(uint machineRegs[]){
  for(int index = 0; index < NUMREGISTERS; index++){
    machineRegs[index] = registers[index];
  }
}

//PRE:takes a uint array that should represent the registers from the
//machine class
//POST:takes the values of the registers from the machine class and
//puts them in their own registers
void PCB::takeRegisters(uint machineRegs[]){
for(int index = 0; index < NUMREGISTERS; index++){
    registers[index] = machineRegs[index];
  }
}

//PRE:object is defined and satisfies the CI
//POST:returns the SP of this object.
uint PCB::getSP() const{
  return registers[STACKPOINTER];
}

//PRE:object is defined and satisfies the CI
//POST:returns the number of words this program has
uint PCB::getProgWords()const{
  return progNumWords;
}


//PRE:object is defined and satisfies the CI
//POST:returns the number of words the stack this prog has
uint PCB::getStackWords()const{
  return stackNumWords;
}


//PRE:object is defined and satisfies the CI
//POST:returns the starting address of program
uint PCB::getProgStart()const{
  //return progStartAddress;
}


//PRE:object is defined and satisfies the CI
//POST:returns the starting address of stack
uint PCB::getStackStart()const{
  //return stackStartAddress;
}


//PRE:object is defined and satisfies the CI
//POST:returns the ending address of program
uint PCB::getProgEnd()const{
  //return progEndAddress;
}


//PRE:object is defined and satisfies the CI
//POST:returns the ending address of stack
uint PCB::getStackEnd()const{
  //return stackEndAddress;
}


//PRE:outString is a MyString object that will hold the information of this PCB
//POST:gives outString the relevant information on this PCB
void PCB::addInfo(MyString & outString){
  char array[GIVEOUTSTRING];
  outString = outString + progName;
  sprintf(array," PID:%i, PC:%i, ProgNumWords:%i \n Number of Timeslices:%i, Number of PageFaults:%i\n",PID,PC,progNumWords,numTimeSlices,numPageFaults);
  outString = outString + array;
  //ASSERT: all info is held in outString
}


//PRE:object exists satisfies the CI
//POST:iterates numTimeSlices by one
void PCB::addTimeSlice(){
  numTimeSlices += 1;
}



//PRE:object exists and satisfies the CI
//POST:iterates numPageFaults by one
void PCB::addPageFault(){
  numPageFaults += 1;
}

//PRE:object exists and satisfies the CI
//POST:returns numTimeSlices
uint PCB::getNumTimeSlices(){
  return numTimeSlices;
}


//PRE:object exists and satisfies the CI
//POST:returns numTimeSlices
uint PCB::getNumPageFaults(){
  return numPageFaults;
}


//PRE:object satisfies the CI
//POST:updates the ProcessPageTable
void PCB::updateProcessPageTable(uint VPN,bool stackBool,bool used,uint PPN){
  if(stackBool){
    //ASSERT:the VPN is in stackVPNList
    uint len = stackVPNList.getNumNodes();
    bool found = false;
    uint index = 0;
    while((!found) && (index < len)){
      ProcessPageInfo * thisThing = stackVPNList.getNth(index);
      if(thisThing->getVPN() == VPN){
	//ASSERT:found right VPN
	found = true;
	//so update
	thisThing->changeInMem(used);
	thisThing->addPPN(PPN);
      }
      index ++;
    }
  }
  else{
    //ASSERT:the VPN is in VPNList
    uint len = VPNList.getNumNodes();
    bool found = false;
    uint index = 0;
    while((!found) && (index < len)){
      ProcessPageInfo * thisThing = VPNList.getNth(index);
      if(thisThing->getVPN() == VPN){
	//ASSERT:found right VPN
	found = true;
	//so update
	thisThing->changeInMem(used);
	thisThing->addPPN(PPN);
      }
      index ++;
    }
  }
}

//PRE:object satisfies the CI
//POST:returns the openFile for this object.
fstream * PCB::getFile(){
  return openFile;
}

//PRE:obj satisfies the CI
//POST:returns the cursorStart for openFile
uint PCB::getCursorStart(){
  return cursorStart;
}

//PRE:object satisfies the CI
//POST:returns a boolean, true if the VPN we specify is in memory in
//either the stack or memory processpages according to the stack
//boolean, if we return true we also return by reference the PPN
bool PCB::isInMem(uint VPN,bool stack, uint & PPN){
  bool rv = false;
  if(stack){
    //ASSERT:looking for VPN in stack process pages
    uint len = stackVPNList.getNumNodes();
    bool found = false;
    uint index = 0;
    while((index < len) && (!found)){
      //ASSERT:not out of bounds and not found yet
      ProcessPageInfo * aPage = stackVPNList.getNth(index);
      if(aPage->getVPN() == VPN){
	found = true;
	rv = aPage->inMemory();
	if(rv){
	  //ASSERT:page is in memory
	  PPN = aPage->getPPN();
	}
      }
      index ++;
    }
  }
  else{
    //ASSERT:looking for VPN in page process pages
    uint len = VPNList.getNumNodes();
    bool found = false;
    uint index = 0;
    while((index < len) && (!found)){
      //ASSERT:not out of bounds and not found yet
      ProcessPageInfo * aPage = VPNList.getNth(index);
      if(aPage->getVPN() == VPN){
	found = true;
	rv = aPage->inMemory();
	if(rv){
	  //ASSERT:page is in memory
	  PPN = aPage->getPPN();
	}
      }
      index ++;
    }
  }
  return rv;
}



  //destructor
PCB::~PCB(){
  uint len = VPNList.getNumNodes();
  for(int index = 0; index < len; index ++){
    ProcessPageInfo * deleteThis = VPNList.getNth(0);
    delete deleteThis;
    VPNList.deleteNth(0);
  }//ASSERT:all heap things in linkedlist were deleted.
  openFile->close();
}
