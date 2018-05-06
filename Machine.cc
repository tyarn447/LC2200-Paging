#include "Machine.h"
#include "Constants.h"
#include "Helper.h"
#include <iostream>
#include <fstream>
#include "Memory.h"
#include "LinkedList.h"
#include "freeMemBlock.h"
#include "PCB.h"
#include "Queue.h"
#include "PageTable.h"
using namespace std;

 
  //CLASS INVARIANT:mem is a pointer to a memory object, cpu holds a
  //cpu object. Max mem holds the maximum amount of memory allowed to
  //the memory object. stepFunc and runFunc tells the machine which
  //current function is still running, the two cannot be true at the
  //same time

//Constructor
//PRE:
//POST:Machine Object created
Machine::Machine(){}

//Constructor
//PRE:numWords is the amount of memory available to machine, pstack is
//the amount of space that should be given to each program for the
//stack and pMemManageType is the type of memory managem
//POST:Machine Object is created with max memory,space for each stack
//and memory management type
Machine::Machine(int numWords,int pStack, int pMemMangageType, int pTimeslice,
		 int pPagesize, int pSwapspace, int pPaging){
  maxMem = (uint)numWords;
  mem = new Memory(maxMem);//create memory object
  loadedProgram = false;
  stepFunc = false;
  stepsTaken = 0;
  registers[0] = 0; //this is for $zero
  stackNum = (uint)pStack;//set stackNum
  memManageType = (uint)pMemMangageType;//set memory management type
  timeSlice = (uint)pTimeslice;//set timeslice amount
  timeSliceCounter = 0;
  finishTimeSlice = false;
  pidCounter = 0;
  memBlockList.addToBack(new freeMemBlock(0,(maxMem - 1) * FOUR,maxMem));
  //full memory is free right now
  //PCB queue has nothing in it right now.
  pageSize = pPagesize;
  swapSpace = pSwapspace;
  paging = pPaging;
  numPages = maxMem/pageSize;
  pageTable = new PageTable(numPages);
}

//PRE:input is a char array representing what was inputted on the
//shell line
//POST:processes this line, parses it, and decides what cmd to run.
void Machine::process(char line[],bool & error, bool & input, bool & output, bool & done, MyString & outString){
  if(input){
    //ASSERT:need to input something to step or run function
    int regInput = charToInt(line);
    output = false;
    step(error,done,input,output,outString,(uint)regInput);
  }
  else if(output){
    //ASSERT:we have just came from output, so call
    int regInput = 0;//this wont be used, just a dummy
    step(error,done,input,output,outString,regInput);
  }
  else{
    //ASSERT:there is a new line to look at for new function
    char cmds [MAXCMDS][MAXINPUT];//have array to hold all words
    uint numCmds = tokenize(line, cmds);
    //ASSERT:cmds nows contains numCmds arrays in it representing cmds
    findFunc(cmds,numCmds,error,input,output,done,outString);
  }
}

//PRE:takes a char * array which is compares to other char arrays to
//see if they are the same
//POST:returns a code representing what command, if not any, to
//execute.
void Machine::findFunc(char cmds[MAXCMDS][MAXINPUT],uint numCmds, bool & error, bool & input, bool & output, bool & done, MyString & outString){
  if(compare(cmds[0],CHARLOAD)){
    //ASSERT:cmds[0] = load
    load(cmds,numCmds,error,output,done,outString);
  }
  else if(compare(cmds[0],CHARMEM)){
    //ASSERT:cmds[0] = mem
    memFunc(cmds,numCmds,error,done,output,outString);
  }
  else if(compare(cmds[0],CHARCPU)){
    //ASSERT:cmds[0] = cpu
    cpuFunc(numCmds,error,done,output,outString);
  }
  else if(compare(cmds[0],CHARSTEP)){
    //ASSERT:cmds[0] = step
    step(cmds,numCmds,error,done,input,output,outString);
  }
  //else if(compare(cmds[0],CHARRUN)){
    //ASSERT:cmds[0] = run
  // run(cmds,numCmds,error,done,input,output,outString);
  //}
  else if(compare(cmds[0],CHARCONFIG)){
    config(output,outString);  
    }
  else if(compare(cmds[0],CHARKILL)){
    killFunc(cmds,numCmds,error,output,outString);
  }
  else if(compare(cmds[0],CHARJOBS)){
    jobFunc(cmds,numCmds,error,output,outString);
  }
  else if(compare(cmds[0],CHARPAGES)){
    pagesFunc(cmds,numCmds,error,output,outString);
  }
  //else if(compare(cmds[0],CHARFREEMEM)){
  // freememFunc(cmds,numCmds,error,output,outString);
  //}
  else{
    //ASSERT:command is not valid
    error = true;
    outString = "This function call does not match any valid cmd";
  }
}


///////////////////////////////////////////////////////
//LOAD PROGRAM COMMANDS START
//////////////////////////////////////////////////////


//PRE:cmds holds the tokens from the parsed line, numcmds holds the
//amount of cmds in cmds, error and done are booleans by reference
//to tell the shell if there was an error or if the function is
//done.
//POST:performs the load function if the correct number of cmds were
//specified.
void Machine::load(char cmds[MAXCMDS][MAXINPUT],uint numCmds, bool & error,bool & output, bool & done,MyString & outString){
  setPC(0);
  //ASSERT:PC=0
  if(numCmds == NUMLOADTOKENS){
    MyString name = cmds[1];//for PCB
    //dont forget to iterate pidCounter when you add a new program and
    //it gets a PCB and u put that PID in the PCB
    //ASSERT:the correct amount of cmds were specifed on the cmd line
    uint index = 0;
    char lce[] = ".lce";
    char * inFileName = concat(cmds[1],lce);
    //in holds heap space so must be deleted when done with it
    //ASSERT:cmds[1] containst programname.lce/0
    fstream * aFile = new fstream (inFileName, ios::in | ios::out);
    name = inFileName;
    delete inFileName;
    if(aFile != NULL){
      //Assert:this inputfile is readable and exists
      loadProgram(aFile,done,error,output,name,outString);
    }
    else{
      //ASSERT:input file is unreadable
      error = true;
      outString = "Input File Is Unreadable";
    }
  }
  else{
    //ASSERT:the wrong number of shell line arguments were specified
    error = true;
    outString = "Wrong Number of Shell Line arguments";
  }
  if(!error){
    //ASSERT:program has been loaded into memory
    loadedProgram = true;
  }
}

//PRE:inputFile is the name of a readable input file, done and error
//represent booleans that can be changed to represent whether the
//function is done or not
//POST:loads the contents of the program into memory.
void Machine::loadProgram(fstream * inputFile, bool & done, bool & error,bool & output,MyString name,MyString & outString){
  uint numWords;
  (*inputFile) >> numWords;//ASSERT:numWords contains the amount of words
			//this inputfile contains
  char throwAway = inputFile->get();
  uint startOfBytes = inputFile->tellg();//represents cursor value at start of bytes
  //ASSERT:new line character is gone.
  //ASSERT:everything else in the file is a binary file.
  PCB * thisPCB = new PCB(numWords,name,stackNum,pidCounter,pageSize,inputFile,startOfBytes);
  PCBQueue.enQueue(thisPCB);
  //ASSERT:have made PCB for this program, will be at back of queue.
  uint thisPID = pidCounter;
  pidCounter ++; //iterate PID
  if(pageTable->getNumFreePages() >= MINFREEPAGES){
    //Assert:we can fit this into memory
    loadProgPage(thisPID,0,error,outString);//load page0 into memory
    loadStackPage(thisPID,0,error,outString);//loadstack0 into memory
    thisPCB->addPageFault();
    thisPCB->addPageFault();
    //because we know we must load these 2 in when loading the program
    //and load page 0 of both 
  }
  else if(pageTable->getNumFreePages() == 1){
    //Must free up 1 page
    uint firstPID; uint firstVPN; bool firstStackBool;
    bool firstRemove = pageTable->removeNonDirtyPage(firstPID,firstVPN,firstStackBool);
    if(firstRemove){
      //ASSERT:removed page from mem so we have 2 free now
      updateProcessPageTable(firstPID,firstVPN,firstStackBool,false,0);
      loadProgPage(thisPID,0,error,outString);//load page0 into memory
      loadStackPage(thisPID,0,error,outString);//loadstack0 into memory
      thisPCB->addPageFault();
      thisPCB->addPageFault();
    }
  }
  else{
    //ASSERT:we may not be able to fit this into memory
    if(pageTable->getNumFreePages() == 0){
      //Must free up 2 pages
      uint firstPID; uint firstVPN; bool firstStackBool;
      bool firstRemove = pageTable->removeNonDirtyPage(firstPID,firstVPN,firstStackBool);
      uint secondPID; uint secondVPN; bool secondStackBool;
      bool secondRemove = pageTable->removeNonDirtyPage(secondPID,secondVPN,secondStackBool);
      if(firstRemove && secondRemove){
	//ASSERT:there should be 2 free slots in mem now
	loadProgPage(thisPID,0,error,outString);
	loadStackPage(thisPID,0,error,outString);
	thisPCB->addPageFault();
	thisPCB->addPageFault();
	updateProcessPageTable(firstPID,firstVPN,firstStackBool,false,0);
	updateProcessPageTable(secondPID,secondVPN,secondStackBool,false,0);
      }
      else{
	//this is where you do stuff for swap area depending on first
	//and second bool you will have to get rid of 1 or 2 pgs that
	//are dirty into swap area.
	
	if(firstRemove){
	  //ASSERT:removed 1 page from memory
	  updateProcessPageTable(firstPID,firstVPN,firstStackBool,false,0);
	  error = true;
	  outString = outString + "Insufficient Memory for this program";
	  kill(thisPID,error,output,outString);
	  //dont kill when u using swap space need to do other shit here.
	}
	if(secondRemove){
	  //ASSERT:removed 1 page from memory
	  updateProcessPageTable(secondPID,secondVPN,secondStackBool,false,0);
	  error = true;
	  outString = outString + "Insufficient Memory for this program";
	  kill(thisPID,error,output,outString);
	}
	error = true;
	outString = outString + "Insufficient Memory for This program";
	kill(thisPID,error,output,outString);
      } 
    }
  }
}


//PRE:PID represents the PID of the program we are loading into
//memory, and VPN represents the virtual page number that we want to
//load into memory.This PID and VPN should not already be present in
//memory
//POST:loads the memory that is on the VPN page from the program into
//memory and makes a pageInfo object to put into the pagetable to
//represent that it is there.
void Machine::loadProgPage(uint PID, uint VPN,bool & error,MyString & outString){
  uint numFreePages = pageTable->getNumFreePages();
  if(numFreePages == 0){
    uint rPID;
    uint rVPN;
    bool rStackBool;//all will represent values of removed things from
		    //certain PID
    //ASSERT:there is no space in memory
    //so remove first non dirty page from Queue.
    bool removed = pageTable->removeNonDirtyPage(rPID,rVPN,rStackBool);
    if(!removed){
      //ASSERT:no more memory in this function
      //This is where we would use swapspace function but do this later
      error = true;
      outString = outString + "Insufficient Memory";
    }
    else{
      //ASSERT:successfully removed a page from usedPages so there is
      //a page open
      updateProcessPageTable(rPID,rVPN,rStackBool,false,0);
      
      uint usePPN = pageTable->addToUsedPages(VPN,PID);
      updateProcessPageTable(PID,VPN,false,true,usePPN);
      //ASSERT:processPageTable has been updated to reflect that this
      //page is in memory
      //since we can put thing in pagetable we should have no problems
      //loading into memory
      loadIntoMem(PID,VPN,false,usePPN);
    }
  }
  else{
    //ASSERT:there is a freepage ready for us
    uint usePPN = pageTable->addToUsedPages(VPN,PID);
    updateProcessPageTable(PID,VPN,false,true,usePPN);
    //ASSERT:processPageTable has been updated to reflect that this
    //page is in memory
    //since we can put thing in pagetable we should have no problems
    //loading into memory
    loadIntoMem(PID,VPN,false,usePPN);
  }
}

//PRE:PID represents the PID of the program we are loading into memory
//and VPN represents the virtual page number for stack that we want to
//load into memory, This PID and VPN should not be aleady present in
//memory for stack.
void Machine::loadStackPage(uint PID, uint VPN,bool & error,MyString & outString){
  uint numFreePages = pageTable->getNumFreePages();
  if(numFreePages == 0){
    uint rPID;
    uint rVPN;
    bool rStackBool;//all will represent values of removed things from
		    //certain PID
    //ASSERT:there is no space in memory
    //so remove first non dirty page from Queue.
    bool removed = pageTable->removeNonDirtyPage(rPID,rVPN,rStackBool);
    if(!removed){
      //ASSERT:no more memory in this function
      //This is where we would use swapspace function but do this later
      error = true;
      outString = outString + "Insufficient Memory";
    }
    else{
      //ASSERT:successfully removed a page from usedPages so there is
      //a page open
      updateProcessPageTable(rPID,rVPN,rStackBool,false,0);
      uint usePPN = pageTable->stackAddToUsedPages(VPN,PID);
      updateProcessPageTable(PID,VPN,true,true,usePPN);
      //ASSERT:processPageTable has been updated to reflect that this
      //page is in memory
      //since we can put thing in pagetable we should have no problems
      //loading into memory
      loadIntoMem(PID,VPN,true,usePPN);
    }
  }
  else{
    //ASSERT:there is a freepage ready for us
    uint usePPN = pageTable->stackAddToUsedPages(VPN,PID);
    updateProcessPageTable(PID,VPN,true,true,usePPN);
    //ASSERT:processPageTable has been updated to reflect that this
    //stack is in memory
    //since we can put thing in pagetable we should have no problems
    //loading into memory
    loadIntoMem(PID,VPN,true,usePPN);
  }
}

//PRE:we have loaded VPN and PID into pageTable with no problems, also
//assuming that this particular page is not in swap area.
//POST:loads VPN from process with PID=PID into memory that is
//referenced by PPN
//POST:loads VPN from PID into the PPN(physical Page) of memory
void Machine::loadIntoMem(uint PID, uint VPN, bool isStack, uint PPN){
  uint startAddress = PPN * pageSize;//start address in memory
  if(!isStack){
    //ASSERT:this is not a stack
    PCB * thisPCB;
    uint len = PCBQueue.getNumNodes();
    bool found = false;
    uint PCBIndex = 0;
    while((PCBIndex < len) && (!found)){
      PCB * aPCB = PCBQueue.getNth(PCBIndex);
      if(aPCB->getPID() == PID){
	found = true;
	thisPCB = aPCB;
      }
      PCBIndex ++;
    }//ASSERT:thisPCB holds PCB that has PID == PID
    mem->initialzeMemory(startAddress,startAddress + pageSize);
    //Assert:Memory at the point we are loading in is all 0
    loadIntoMemHelper(VPN,PPN,thisPCB);
   
  }
  else{
    //ASSERT:this is a stack so just initialze memory to 0 there
    mem->initialzeMemory(startAddress,startAddress + pageSize);
    //ASSERT:Memory at PPN has been made all 0's so it is a stack.
  }
}


//PRE:PCB* thisPCB is a pointer to a PCB,VPN is a uint that references
//a virtual page number, PPN is a uint that references Physical Page
//Number.
//POST:loads VPN from PCB into memory at PPN location
void Machine::loadIntoMemHelper(uint VPN,uint PPN,PCB * thisPCB){
  uint startAddress = PPN * pageSize;//start address in memory
  //ASSERT:we are loading into memory so it isnt in memory.
  fstream * aFile = thisPCB->getFile();
  uint cursorStart = thisPCB->getCursorStart();
  uint byteStart = VPN * pageSize * FOUR;
  aFile->seekp(cursorStart + byteStart, ios::beg);
  //ASSERT:cursor is at first byte it needs to get into memory. 
  unsigned char ch;
  bool done = false;
  uint shiftCounter = SHIFT0;
  uint index = 0;//start at this word
    while((index < pageSize) && (!done)){//boolean for if we run out
      //memory to read in middle
      //of page
      ch = aFile->get();
      if(!aFile->eof()){
	//ASSERT:we are not at the end of file.
	shiftCounter -= SHIFTMINUS;
	mem->shiftAndInput(startAddress + index, shiftCounter,(uint)ch);
	if(shiftCounter == SHIFT4){
	  //ASSERT:we have filled up memarray[index] with 4 bytes, so
	  //move on to next index and reset shiftCounter
	  shiftCounter = SHIFT0;
	  index ++;
	}
      }
      else{
	//ASSERT:reached end of file in middle of page
	done = true;
      }
    }
}

//PRE:object satisfies the CI
//POST:updates the ProcessPageTable of PCB with PID==PID
void Machine::updateProcessPageTable(uint PID, uint VPN, bool stackBool, bool used,uint PPN){
  uint len = PCBQueue.getNumNodes();
  bool found = false;
  uint index = 0;
  while((!found) && (index < len)){
    PCB * thisPCB = PCBQueue.getNth(index);
    if(thisPCB->getPID() == PID){
      //ASSERT:we found the correct PID
      found = true;
      thisPCB->updateProcessPageTable(VPN,stackBool,used,PPN);
      //ASSERT;we have updated pageTable in PCB
    }
    index ++;
  }
}

//PRE:numwords represents number of words we need for the program,
//pStart,pEnd,sStart and sEnd all represent addresses for program
//start/end and stack start/end
//POST:returns true if there is memory avaiable for the program and
//stack, it also put the addresses of that memory into the parameters
//passed by reference
bool Machine::findMem(uint numWords,uint & pStart,uint & pEnd,uint & sStart,uint & sEnd){
  bool rv = false;
  if(memManageType == 0){
    //ASSERT:we are using first fit
    bool progBool = false;
    bool stackBool = false;
    progBool = firstFit(numWords, pStart,pEnd);
    stackBool = firstFit(stackNum,sStart,sEnd);
    if((progBool) & (stackBool)){
      //ASSERT:we have found memory locations for both stack and
      //program
      rv = true;
    }
    else if((progBool) && (!stackBool)){
      //ASSERT:both are not being used so must return memory
      //return prog stuff cause it was found and taken out of mem
      freeMemBlock * returnBlock = new freeMemBlock(pStart,pEnd,numWords);
      addToFreeMem(returnBlock);//returning taken block to freemem
    }
    else if((!progBool) && (stackBool)){
      //ASSERT:both are not being used so must return memory
      //return stack stuff cause it was found and taken out of mem
      freeMemBlock * returnBlock = new freeMemBlock(sStart,sEnd,stackNum);
      addToFreeMem(returnBlock);//returning taken block from freemem
    }
    else{
      //neither were true so dont need to do return any memory
    }
  }
  else{
    //ASSERT:we are using best fit
    bool progBool = false;
    bool stackBool = false;
    progBool = bestFit(numWords, pStart,pEnd);
    stackBool = bestFit(stackNum,sStart,sEnd);
    if((progBool) & (stackBool)){
      //ASSERT:we have found memory locations for both stack and
      //program
      rv = true;
    }
    else if((progBool) && (!stackBool)){
      //ASSERT:both are not being used so must return memory
      //return prog stuff cause it was found and taken out of mem
      freeMemBlock * returnBlock = new freeMemBlock(pStart,pEnd,numWords);
      addToFreeMem(returnBlock);//returning taken block to freemem
    }
    else if((!progBool) && (stackBool)){
      //ASSERT:both are not being used so must return memory
      //return stack stuff cause it was found and taken out of mem
      freeMemBlock * returnBlock = new freeMemBlock(sStart,sEnd,stackNum);
      addToFreeMem(returnBlock);//returning taken block from freemem
    }
    else{
      //neither were true so dont need to do return any memory
    }
  }
  return rv;
}

//PRE:numwords represents the number of words needed from memory,
//start and end represent the addresses that will be gotten if the
//program finds memory big enough for numWords
//POST:returns true if there is memory large enough for numWords and
//puts addresses in start and end accordingly, false otherwise, finds
//the first memory large enough to hold numWords and uses that, also
//change memBlockList if we take some memory from it
bool Machine::firstFit(uint numWords, uint & start, uint & end){
  //use first fit here
  uint numNodes = memBlockList.getNumNodes();
  bool found = false;
  int index = 0;
  while((index < numNodes) && (!found)){
    //ASSERT:we have not found a suitable fit for numWords of memory
    //and have not gone through all nodes yet
    freeMemBlock * aBlock = memBlockList.getNth(index);
    uint lenABlock = aBlock->getNumWords();
    if(lenABlock >= numWords){
      //ASSERT:we have found a block of memory able to hold our prog
      //or stack
      found = true;
      uint startABlock = aBlock->getStart();
      start = startABlock;
      end = startABlock + ((numWords - 1) * FOUR);
      aBlock->setStart(end + FOUR);//set new start of memory block
      uint newABlockWords = lenABlock - numWords;
      if(newABlockWords == 0){
	//ASSERT:aBlock has no more memory space so delete
	memBlockList.deleteNth(index);
	delete aBlock;
      }
      else{
	//ASSERT:aBlock still holds some free memory
	aBlock->setNumWords(newABlockWords);
      }
    }
    else{
      //ASSERT:did not find memory
      index++;
    }
  }
  return found;
}


//PRE:numwords represents the number of words needed from memory,
//start and end represent the addresses that will be gotten if the
//program finds memory big enough for numWords
//POST:returns true if there is memory large enough for numWords and
//puts addresses in start and end accordingly, false otherwise, finds
//the best closest amount of memory to numWords and uses that, also
//changes memBlockList if we take memory out of it
bool Machine::bestFit(uint numWords, uint & start, uint & end){
  uint numNodes = memBlockList.getNumNodes();
  bool found = false;
  uint bestFitIndex;
  uint bestFitNumWords; 
  int index = 0;
  while(index < numNodes){
    //ASSERT:we have not found a suitable fit for numWords of memory
    //and have not gone through all nodes yet
    freeMemBlock * aBlock = memBlockList.getNth(index);
    uint lenABlock = aBlock->getNumWords();
    if(lenABlock >= numWords && (!found)){
      //ASSERT:we have found a block of memory able to hold our prog
      //or stack
      found = true;
      bestFitIndex = index;
      bestFitNumWords = lenABlock;
    }
    else if(lenABlock >= numWords){
      //ASSERT:we have already found a first fit so check and see
      //which fit is better
      if(lenABlock < bestFitNumWords){
	//ASSERT:This is a better fit
	bestFitIndex = index;
      }
    }
    else{
      //We did not find a possible fit
    }
    index++;//must do this always cause we need to go through all
	    //possibilites 
  }
  if(found){
    //ASSERT:we have found the best fit
    freeMemBlock * bestFit = memBlockList.getNth(bestFitIndex);
    uint startBestFit = bestFit->getStart();
    start = startBestFit;
    end = startBestFit + ((numWords - 1) * FOUR);
    bestFit->setStart(end + FOUR);//set new start of memory block
    uint newMemBlockWords = bestFit->getNumWords() - numWords;
    if(newMemBlockWords == 0){
      //ASSERT:aBlock has no more memory space so delete
      memBlockList.deleteNth(index - 1);
      delete bestFit;
    }
    else{
	//ASSERT:aBlock still holds some free memory
      bestFit->setNumWords(newMemBlockWords);
    }
  }
  return found;
}
  

///////////////////////////////////////////////////////
//LOAD PROGRAM COMMANDS END
//////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//MEM PROGRAM COMMANDS START
//////////////////////////////////////////////////////

//PRE:cmds holds the tokens from the parsed, numcmds holds the
//amount of cmds in the cmds array. error and done are booleans by
//reference to tell the shell if there was an error or if the
//function completed correctly.
//POST:calls the appropriate function from the memory class.
void Machine::memFunc(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error, bool & done,bool & output, MyString & outString){
  if(loadedProgram){
    //ASSERT:a program has been loaded into memory
    if(numCmds == NUMMEMTOKENS){
      MyString newString;
      outString = newString;//reset outString
      //ASSERT:The user input the correct amount of tokens, 2 numbers
      uint num1 = charToInt(cmds[1]);
      uint num2 = charToInt(cmds[2]);
      if(num1 <= (FOUR *mem->getMem()) && num2 <= (FOUR * mem->getMem())){
	//ASSERT:num1/num2 is not more than our max memory 
	
	if((num1 % FOUR == 0) && (num2 % FOUR ==0)){
	  //ASSERT:numbers are valid
	  mem->displayMemory(num1,num2,done,error,outString);
	  //ASSERT:output is held in outstring
	  output = true;
	}
      }
      else{
	error = true;
	outString = "The memory trying to be shown is partly or completely out of bounds";
      }
    }
    else if(numCmds == NUMMEMTOKENS - 1){
      //ASSERT:The user input the correct amount of tokens, 1 number
      uint num1 = charToInt(cmds[1]);
      if(num1 <= (FOUR * mem->getMem())){
	//ASSERT:num1 is less than our max memory
	uint num2 = mem->getMem() * FOUR;
	if((num1 % FOUR ==0)){
	  //ASSERT:numbers are valid
	  mem->displayMemory(num1,num2,done,error,outString);
	  //ASSERT:output is held in outString
	  output = true;
	}
      }
      else{
	error = true;
	outString = "The memory trying to be shown is partly or completely out of bounds";
      }
    }
    else if(numCmds == 1){
      uint num1 = 0;
      uint num2 = mem->getMem() * FOUR;
      mem->displayMemory(num1,num2,done,error,outString);
      //ASSERT:output is held in outstring
      output = true;
    }
    else{
      //ASSERT:The user did not input the correct amount of tokens.
      error = true;
      outString = "Wrong Number of shell line arguments";
    }
  }
  else{
    //ASSERT:a program has not been loaded into memory
    error = true;
    outString = "There is no program loaded into memory";
  }
}



//PRE:object is defined and satisfies the CI, memBlock is a block of
//free memory that was just freed and is being given back to the free
//memory list
//POST:adds memBlock to the freeMemList, if memBlock is next to
//another free block of memory, the two will be spliced together, if
//not it will be added to the front of the memory list.
void Machine::addToFreeMem(freeMemBlock * memBlock){
  uint length = memBlockList.getNumNodes();
  bool spliced = false;
  int index = 0;
  while((index < length) && (!spliced)){
    //ASSERT:we have not checked all the memory block in memblocklist
    //and the object has not been spliced to another object yet.
    freeMemBlock * thisMemBlock = memBlockList.getNth(index);
    //ASSERT:thisMemBlock holds a memBlock we havent checked yet
    spliced = thisMemBlock->splice(memBlock);
    
    //spliced is bool representing if the memory blocks were spliced
    //together or not 
    if(spliced){
      
      //ASSERT:we spliced the memory block, check block after/before the
      //block we spliced and see if they connect now cause of new
      //spliced block.
      delete memBlock;//needs to be deleted now cause not putting it
		      //anywhere cause it was spliced
      bool afterSpliced = false;//only possible for one of these to be
				//true
      bool beforeSpliced = false;
      if(index + 1 < length){
	
	freeMemBlock * tryMemBlock = memBlockList.getNth(index + 1);
	afterSpliced = tryMemBlock->splice(thisMemBlock);
      }
      if(index - 1 > 0){
	freeMemBlock * tryMemBlock = memBlockList.getNth(index - 1);
	beforeSpliced = tryMemBlock->splice(thisMemBlock);
      }
      if(afterSpliced || beforeSpliced){
	//ASSERT:we spliced the block we previously spliced to another
	//block before or after it, so now we are finally done, so now
	//dequeue that block and delete it.
	memBlockList.deleteNth(index);//delete because we spliced it
				      //into another block
	delete thisMemBlock;//just deleted it from list so delete it
			    //from stack.
      }
    }
    else{
      //ASSERT:we did not splice the memory block
      index++;
    }
  }
  if(!spliced){
    //ASSERT:memBlock could not be spliced to another memoryblock in
    //memBlockList so we must add it to the list
    //memBlockList.addToBack(memBlock);
    uint startAd = memBlock->getStart();
    uint numNodes = memBlockList.getNumNodes();
    uint index = 0;
    bool found = false;
    while((index < numNodes) && (!found)){
      //ASSERT:we have not found the place to place it and have not
      //gone past the number of nodes
      freeMemBlock * aBlock = memBlockList.getNth(index);
      uint thisBlockStart = aBlock->getStart();
      if(startAd < thisBlockStart){
	found = true;
	memBlockList.insertNth(index,memBlock);
      }
      else{
	index++;
      } 
    }
    if(!found){
      //ASSERT:we got to last node and still did not find a place to
      //place it so we place at back cause it must have largest start
      //address
      
      memBlockList.addToBack(memBlock);
    }
    
    //ASSERT:memory is sorted least to most incase we use first fit
  }
}



//PRE:object is defined and satisfies the CI
//POST:sorts the memory blocks in memBlocklist using quicksort
void Machine::sortMemory(){
  uint numNodes = memBlockList.getNumNodes();
  freeMemBlock * array[numNodes];//holds freememBlocks
  for(int index = 0; index < numNodes; index++){
    array[index] = memBlockList.getNth(0);
    //we are getting the 0th node everytime because we are deleting
    //after
    memBlockList.deleteFirst();
    
  }
  quickSort(array,0,numNodes);
  //ASSERT:freeMemBlocks are now sorted in array
  //so add them back to the memBlockList
  for(int index = 0; index < numNodes; index++){
    memBlockList.addToBack(array[index]);
    //ASSERT:array[index] has been added to the back of memblocklist
    //so at the end they should be sorted in memBlockList
  }
  
}
///////////////////////////////////////////////////////
//MEM PROGRAM COMMANDS END
//////////////////////////////////////////////////////



///////////////////////////////////////////////////////
//PAGES PROGRAM COMMANDS START
//////////////////////////////////////////////////////

//PRE:cmds represents the commands on the input line, numCmds is the
//number of commands on the command line, error and output are
//booleans that the shell will read, outString will be the string
//output to the shell
//POST:checks to see which pages function to use and executes the
//correct one
void Machine::pagesFunc(char cmds[MAXCMDS][MAXINPUT],uint numCmds,bool & error,bool & output, MyString & outString){
  if(numCmds == 1){
    //ASSERT:print all pages
    pageTable->giveOutString(outString);
    output = true;
  }
  else if(numCmds == NUMPAGESTOKENS){
    //ASSERT:print only pages with PID==cmds[2];
    uint PID = charToInt(cmds[1]);
    pageTable->giveOutString(PID,outString);
    output = true; 
  }
  else{
    error = true;
    outString = "Incorrect amount of tokens on command line";
  }
  
}




///////////////////////////////////////////////////////
//PAGES PROGRAM COMMANDS END
//////////////////////////////////////////////////////








///////////////////////////////////////////////////////
//JOB COMMANDS START
//////////////////////////////////////////////////////

//PRE:jobs was typed into shell, cmds are the tokens from the cmd
//line, numcdms is number of tokens,error output and outString are
//booleans/MyStrings to tell the shell to output stuff and what to
//output
//POST:calls a function that puts all information from PCBQueue into outstring
void Machine::jobFunc(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error,bool & output, MyString & outString){
  if(numCmds == NUMJOBTOKENS){
    //ASSERT:there are the correct number of tokens on shell line
    uint numNodes = PCBQueue.getNumNodes();
    if(numNodes != 0){
      for(int index = 0; index < numNodes; index++){
	PCB * currPCB = PCBQueue.getNth(index);
	currPCB->addInfo(outString);
      }
     
    }
    else{
       outString = outString + "There is nothing in the PCBQueue";
     }
    //ASSERT:all info has been added to outString
    output = true;
  }
  else{
    //ASSERT:wrong number of tokens on cmd line
    error = true;
    outString = "The wrong amount of arguments were specified for jobs command";
  }
}



///////////////////////////////////////////////////////
//JOB COMMANDS END
//////////////////////////////////////////////////////





///////////////////////////////////////////////////////
//FREEMEM COMMANDS START
//////////////////////////////////////////////////////

//PRE:freemem was typed into shell, cmds are the tokens from the cmd
//line, numcdms is number of tokens,error output and outString are
//booleans/MyStrings to tell the shell to output stuff and what to
//output
//POST:calls a function that puts all information from memBlockList into outString
void Machine::freememFunc(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error,bool & output, MyString & outString){
  if(numCmds == NUMFREEMEMTOKENS){
    //ASSERT:there are the correct number of tokens on shell line
    uint numNodes = memBlockList.getNumNodes();
    char array1[MAXINPUT];
    sprintf(array1,"Number of Free Memory Blocks:%i\n",numNodes);
    outString = outString + array1; 
    for(int index = 0; index < numNodes; index++){
      freeMemBlock * currMemBlock = memBlockList.getNth(index);
      uint currStartAddress = currMemBlock->getStart();
      uint currNumWords = currMemBlock->getNumWords();
      char array2[MAXINPUT];
      sprintf(array2,"Starting Address:%i Number of Words:%i\n",currStartAddress,currNumWords);
      outString = outString + array2;
    }
    //ASSERT:all info has been added to outString
    output = true;
  }
  else{
    //ASSERT:wrong number of tokens on cmd line
    error = true;
    outString = "The wrong amount of arguments were specified for jobs command";
  }
  
}
  

///////////////////////////////////////////////////////
//FREEMEM COMMANDS END
//////////////////////////////////////////////////////



///////////////////////////////////////////////////////
//KILL COMMANDS START
//////////////////////////////////////////////////////


//PRE:kill was typed into shell, cmds are the tokens from the cmd
//line, numcdms is number of tokens,error output and outString are
//booleans/MyStrings to tell the shell to output stuff and what to
//output
//POST:calls the kill command on a specific PID if the correct amount
//of cmds were on command line, returns an error message otherwise
void Machine::killFunc(char cmds[MAXCMDS][MAXINPUT],uint numCmds, bool & error,bool & output, MyString & outString){
  if(numCmds == NUMKILLTOKENS){
    //ASSERT:there are the correct number of commands on command
    //line
    uint pid = (uint)charToInt(cmds[1]);
    //ASSERT:we have the correct PID
    kill(pid, error,output,outString);
  }
  else{
    //ASSERT:wrong number of commands on cmd line
    error = true;
    output = true;
    outString = outString + "Too many arguments or too few on command line for kill command";
  }
}


  
//PRE:pid is the pid we are looking for to take out of PCBQueue,
//bool error and output will tell the shell what to output and
//outString holds that output
void Machine::kill(uint pid, bool & error,bool & output,MyString outString){
  uint numNodes = PCBQueue.getNumNodes();
  bool found = false;
  uint index = 0;
  PCB * deleteThis;
  
  while((index < numNodes) && (!found)){
    //ASSERT:we have not looked at all PID's and have not found PID
    PCB * aPCB = PCBQueue.getNth(index);
    if(aPCB->getPID() == pid){
      //ASSERT:we have found the pid
      found = true;
      deleteThis = aPCB;
      
      PCBQueue.deQueueNth(index);
      
    }
    else{
      //ASSERT:we have not found the pid
      index++;
    }
  }
  
  if(!found){
    //ASSERT:we never found nor deleted a pid
    error = true;
    output = true;
    outString = outString + "There is no such PID in PCBQueue";
  }
  else{
    //ASSERT:we found the PID in PCBQueue, now delete it and reallocate memory.
    // uint pStartAddress = deleteThis->getProgStart();
    // uint pEndAddress = deleteThis->getProgEnd();
    // uint pWords = deleteThis->getProgWords();
    // uint sStartAddress = deleteThis->getStackStart();
    // uint sEndAddress = deleteThis->getStackEnd();
    // uint sWords = deleteThis->getStackWords();
   
    // //create memory blocks for the stack and prog to add back to freemem
    // freeMemBlock * progMem = new freeMemBlock(pStartAddress,pEndAddress,pWords);
    // freeMemBlock * stackMem = new freeMemBlock(sStartAddress,sEndAddress,sWords);
    // bool spliced = progMem->splice(stackMem);
    // if(!spliced){
    //   //ASSERT:they were not spliced together
    //   addToFreeMem(progMem);
    //   addToFreeMem(stackMem);
    // }
    // else{
    //   //ASSET:they were spliced together
    //   addToFreeMem(progMem);
    // }

    pageTable->deletePID(deleteThis->getPID());
    //ASSERT:All instances of PID has been deleted from pageTable.
    delete deleteThis;//object on stack has been deleted
  }
}
  

///////////////////////////////////////////////////////
//KILL COMMANDS START
//////////////////////////////////////////////////////




///////////////////////////////////////////////////////
//CPU PROGRAM CMDS START
//////////////////////////////////////////////////////


//PRE:numCmds is an int to tell how many commands were specified on
//the command line, error and done and output all tell the shell
//that there is an output on the command line ready, outstring is
//what holds the output.
//POST:Puts the contents of the registers and PC in outstring as a
//string and tells the shell to output this string.
void Machine::cpuFunc(uint numCmds, bool & error, bool & done, bool & output, MyString & outString){
  if(numCmds == 1){
    //ASSERT:there was only 1 cmd, cpu
    char pidArray[MAXINPUT];
    if(PCBQueue.getNumNodes() > 0){
      //There is a pcb in the queue
      uint currPID = PCBQueue.getNth(0)->getPID();
      sprintf(pidArray,"PID:%i ",currPID);
      outString = outString + pidArray;
    }
    else{
      outString = outString + "There is no PCB in the Queue \n";
    }
    displayContents(outString);
    //ASSERT:outString contains the contence of CPU
    output = true;
  }
  else{
    //ASSERT:There was somethings specified after cpu command
    //resulting in an error
    error = true;
    outString = "There cannot be things specified after cpu command";
  }
}


//PRE:outString is a Mystring object by reference that holds strings
//POST:the contents of the CPU, such as the PC and registers are
//held in outString.
void Machine::displayContents(MyString & outString){
  char PCArray[MAXINPUT];
  sprintf(PCArray," PC:%#x(%i), ",PC,PC);
  outString = outString + PCArray;
  outString.addChar(NEWLINE);
  uint counter = 0;
  //ASSSERT:contents of PC have been added to outstring
  for(int index = 0; index < NUMREGISTERS; index++){
    counter++;
    outString = outString + regNames[index];
    //ASSERT:we have added register name to outString
    char array[MAXINPUT];
    sprintf(array," :%#x(%i), ",registers[index],registers[index]);
    outString = outString + array;
    //ASSERT:we have added register contents to line after register name
    if(counter == FOUR){
      //ASSERT:four registers have been put on a line so put newline
      counter = 0;
      outString.addChar(NEWLINE);
    }
  }
  outString.addChar(EOS);
  //ASSERT:outString contains contents of PC and all registers
}

///////////////////////////////////////////////////////
//CPU PROGRAM COMMANDS END
//////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//STEP/RUN COMMANDS START
/////////////////////////////////////////////////////

//PRE:num cmds is how many things are in the cmds array, error and
//done and booleans that will tell the shell things, outstring is
//where any output to he shell is held
//POST:gets commands from memory and executes them
void Machine::step(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error, bool & done,bool & input,bool & output, MyString & outString){
  if(numCmds == NUMSTEPTOKENS){
    if(PCBQueue.getNumNodes() == 0){
      error = true;
      outString = outString + "There is nothing in the PCBQueue to step through";
    }
    else{
      finishTimeSlice = false;
      copyPCBToCPU();//copies all relevant information in the PCB to the CPU
      uint numSteps = charToInt(cmds[1]);
      //ASSERT:numSteps holds the number of steps the user wants us to go in the loaded program
      stepFunc = true;
      cpuStep(numSteps,mem,error,input,done,output,outString);
      if(!input){
	//ASSERT:we are done with step, not asking for input..
	stepFunc = false;
      }
      //if output is true it will stay true
    }
  }
  else{
    error = true;
    outString = "Wrong number of tokens for Step Command, Usage: step n where n represents a number ";
  }
}


//PRE:num cmds is how many things are in the cmds array, error and
//done and booleans that will tell the shell things, outstring is
//where any output to the shell is held, this is only called when
//something was just outputted to the shell or input was gotten from
//the shell.
//POST:calls step function with correct parameters depending on
//circumstances such as which booleans are true or false.
void Machine::step(bool & error, bool & done,bool & input,bool & output, MyString & outString, uint regInput){
  if(output){
    //ASSERT:we just outputted something, so go back to step
    output = false;
    //ASSERT:we are done outputting so change to false
    cpuStep(mem,error,input,done,output,outString);
  }
  else{
    //ASSERT:We just inputted something
    input = false;
    stepFunc = false;
    inStep(mem,error,input,done,output,outString,regInput);
  }
  //ASSERT: do all usual checks aftereach step function call
   if(input == false){
     //ASSERT:we are done with step, not asking for input..
     stepFunc = false;
   }
   //if output is true, it will stay true

}




//PRE:num cmds is how many things are in the cmds array, error and
//done and booleans that will tell the shell things, outstring is
//where any output to he shell is held
//POST:calls step on the maximum amount of steps we can take so it
//essentially runs until it hits a halt statement.
void Machine::run(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error, bool & done,bool & input,bool & output, MyString & outString){
  if(numCmds == NUMRUNTOKENS){
    copyPCBToCPU();//copies all relevant information in the PCB to the CPU
    int numSteps = mem->getMem();//returns the max words, or max steps
				 //possible...
    //ASSERT:we will run for the maximum amount of memory if PC is 0
    //or until stepsTaken > maxMem
    cpuStep(numSteps,mem,error,input,done,output,outString);
    //Assert step has been called with the max amount of steps possible,
  }
  else{
    error = true;
    outString = "Wrong Number of tokens for run Commmand, Usage: run";
  }
}



//PRE:numSteps represents the amount of commands in memory we should
//execute, mem is a reference to the memory object, error input and
//done are all boolean references to give to the shell
//POST:executes numsteps commands in memory or until halt command
void Machine::cpuStep(uint numSteps,Memory * mem,bool & error,bool & input,bool & done,bool & output, MyString & outString){
  stepsNeeded = numSteps;
  while((stepsTaken < stepsNeeded) && (!input) && (!output) && (!error)){
    if(PCBQueue.getNumNodes() == 0){
      //ASSERT:there is nothing in the PCBQueue to be stepped through
      stepsTaken = stepsNeeded;
      output = true;
      outString = outString + "There is nothing more in the PCBQueue";
    }
    else{
      MyString newString;
      outString = newString;
      //when step or run is called we copy contents of pcb to cpu there
      PCB * currPCB = PCBQueue.getNth(0);//get current PCB
      finishTimeSlice = false;
      output = false;
      done = true;
      input = false;
      bool haltbool = false;
      //****designate PID for outString
      uint currPID = currPCB->getPID();
      char PIDArray[MAXINPUT];
      sprintf(PIDArray,"PID:%i ",currPID);
      outString = outString + PIDArray;
      //***designate PID for outString
      while((timeSliceCounter < timeSlice) && (!haltbool) && (!input) && (!output) &&
	    (!error)){
	//ASSERT:we have not reached stepsNeeded, we have not hit a halt
	//instruction, we have not hit an input instruction, PC/FOUR
	//less than maxSteps cause as long as it is we still have more
	//memory to go, we have not hit an output instruction.
	//find out if VPN where PC is is in memory or not
	uint progNumWords = currPCB->getProgWords();
	uint largestPC = (progNumWords - 1) * FOUR;
	bool stack;
	uint lookingForWord = PC/FOUR;//represents the word we want
	if(lookingForWord < progNumWords){
	  //ASSERT:we are looking for VPN in prog memory
	  stack = false;
	}
	else{
	  //ASSERT:we are looking for VPN in stack memory
	  stack = true;
	  lookingForWord = lookingForWord - (progNumWords - 1);
	  //now represents the word in stack we want
	}
	uint pageNum = findWordVPN(lookingForWord,pageSize);
	uint PPN;
	bool inMemory = currPCB->isInMem(pageNum,stack,PPN);
	//PPN is by ref so if bool is true we already have PPN,
	//do next,
	if(!inMemory){
	  //ASSERT:pageNum was not in memory so we must load it in
	  //so pagefault
	  currPCB->addPageFault();
	  if(stack){
	    //ASSERT:we are loading in a stack page
	      loadStackPage(currPID,pageNum,error,outString);
	  }
	  else{
	    //ASSERT:we are loading in a program page
	      loadProgPage(currPID,pageNum,error,outString);
	  }
	  //ASSERT:this page should be in memory now
	  inMemory = currPCB->isInMem(pageNum,stack,PPN);
	  //theoretically inMemory should now be true and PPN should
	  //hold the physical frame that this page is located unless
	  //there was an error in loadStackPage or loadProgPage
	}
	else{
	  //ASSERT:it is already in memory, we are using it
	  if(paging == 1){
	    //ASSERT:paging type is LRU
	    pageTable->LRU(currPID,pageNum,stack);
	  }
	}
	//ASSERT:have correct PPN and program is loaded into
	//memory,unless error was true in which case instructions will
	//not execute and prog will be killed.

	//once you find out if  in memory
	//if it isnt loadIntoMem, need to get PPN somehow either way
	//get correct address
	//uint instruction =//getInstruction with address gotten from
			  //VPN, think you might have to do something
			  //with mod depending on the lookingForWord.
	if((!error) && (inMemory)){
	  //ASSERT:there has not been an error with loading things in
	  //to memory so we have ppn and its loaded into mem
	  uint pageStart = PPN * pageSize;
	  uint address = ((lookingForWord % pageSize) + pageStart) * FOUR;
	  uint instruction = mem->getInstruction(address);
	  //for if it halts cause if its in case get cross
	  //initialization error everywhere.
	  char array[GIVEOUTSTRING];
	  uint numSlices = currPCB->getNumTimeSlices();
	  uint numFaults = currPCB->getNumPageFaults();
	  PC += FOUR;//iterate PC by four because we took a step and got instruction
	  uint opcode = getOpcode(instruction);
	  switch((opcodeType)opcode){
	  case ADDCODE:
	    ADD(instruction);
	    break;
	  case NANDCODE:
	    NAND(instruction);
	    break;
	  case ADDICODE:
	    ADDI(instruction);
	    break;
	  case LWCODE:
	    LW(instruction,mem,error,outString);
	    break;
	  case SWCODE:
	    SW(instruction,mem,error,outString);
	    break;
	  case BEQCODE:
	    BEQ(instruction);
	    break;
	  case JALRCODE:
	    JALR(instruction);
	    break;
	  case HALTCODE:
	    HALT();
	    haltbool = true;
	    output  = true;
	    sprintf(array,"Program has Halted,TimeSlices:%i, PageFaults:%i",numSlices + 1,numFaults);
	    outString = outString + array; 
	    break;
	  case INCODE:
	    input = true;
	    //currInInstruction = instruction;
	    outString = outString + " ?";
	    done = false;
	    break;
	  case OUTCODE:
	    OUT(instruction, output, outString);
	    done = false;
	    break;
	  case LACODE:
	    LA(instruction);
	    break;
	  case BGTCODE:
	    BGT(instruction);
	    break;
	  }
	}
	timeSliceCounter++;//ASSERT:we have taken a step
	if(error){
	  //ASSERT:there was an error
	  kill(currPCB->getPID(),error,output,outString);
	  char killArray[GIVEOUTSTRING];
	  uint numSlicesKill = currPCB->getNumTimeSlices();
	  uint numFaultsKill = currPCB->getNumPageFaults();
	  sprintf(killArray,"Program has had an error,TimeSlices:%i, PageFaults:%i",numSlicesKill + 1,numFaultsKill);
	  outString = outString + killArray; 	}
      }
      if((timeSliceCounter >= timeSlice) && (!input) && (!output) || (haltbool)){
	//ASSERT:we are not outputting or inputting anything, we are
	//actually done with this iteration of step so reset
	//timeSliceCounter and iterate stepsTaken.
	finishTimeSlice = true;
	currPCB->addTimeSlice();
	//ASSERT:we have added a timeslice to this PCB
	//because the timeslice finished
	timeSliceCounter = 0;
	stepsTaken++;
	done = false;
	if(!haltbool){
	  //ASSERT:we have not halted the program yet so have not taken
	  //the PCB out of the queue so copy cpu stuff to PCB in front
	  //if we have halted the pcb in the front is a new one
	  copyCPUToPCB();
	  //Also want to rotate PCBQueue
	  rotatePCBQueue();
	}
	//ASSERT:we have rotated or removed previous PCB from front
	if(PCBQueue.getNumNodes() > 0){
	  copyPCBToCPU();
	 }
      }
    }
  }
  if(stepsTaken >= stepsNeeded){
    //ASSERT:we are finished with step so we can get out of loop in
    //helper file by setting done to true
    done = true;
    stepsTaken = 0;
  }
}


//PRE:object is defined and satisfies the CI, there is something in PCBQueue
//POST:rotate PCBQueue by one
//slot
void Machine::rotatePCBQueue(){
  //ASSERT:There is atleast 1 node in PCBQueue
  PCB * rotate = PCBQueue.getNth(0);
  PCBQueue.deQueue();
  PCBQueue.addToBack(rotate);
}


//PRE:we are in the stepping or running a program
//POST:copies all relevant information in the PCB to the cpu.
void Machine::copyPCBToCPU(){
  PCB * aPCB = PCBQueue.getNth(0);
  aPCB->giveRegisters(registers);
  PC = aPCB->getPC();
  
}

//PRE:we have finished step and run and have not halted a program
//POST:copies all relevant information in the CPU to the PCB
void Machine::copyCPUToPCB(){
  PCB * aPCB = PCBQueue.getNth(0);
  aPCB->setPC(PC);
  aPCB->takeRegisters(registers);
}

//PRE:this step will be called after outputting the something to the
//shell so you dont have to reinput stepsneeded in the machine class,
//it will do it here
//POST:calls step function with the correct amount of steps needed
void Machine::cpuStep(Memory * mem,bool & error,bool & input,bool & done,bool & output, MyString & outString){
  cpuStep(stepsNeeded, mem, error,input,done,output,outString);
  
}



//PRE:step or run had asked for input, this is the continuation of this step
//POST:continues to execute commands specified by the stepsNeeded member data
void Machine::inStep(Memory * mem, bool & error, bool & input, bool & done,bool & output, MyString & outString, uint regInput){
  //Due to the nature of instep the instruction we are looking for
  //must already be in memory somewhere.
  input = false;
  output = false;
  PCB * currPCB = PCBQueue.getNth(0);
  uint inPC = PC - FOUR;
  uint lookingForWord = inPC/FOUR;//represents the word we want
  uint progNumWords = currPCB->getProgWords();
  bool stack;
  if(lookingForWord < progNumWords){
    //ASSERT:we are looking for VPN in prog memory
    stack = false;
  }
  else{
    //ASSERT:we are looking for VPN in stack memory
    stack = true;
    lookingForWord = lookingForWord - (progNumWords - 1);
    //now represents the word in stack we want, from 0 to
    //whenever stack ends, we minused 1 after because
    //progNumWords will always be 1 more than the words of prog
    //cause words of prog go 0-n-1 and progNumWords = n cause it
    //assumes 1-n
  }
  uint pageNum = findWordVPN(lookingForWord,pageSize);
  uint PPN;
  bool inMemory = currPCB->isInMem(pageNum,stack,PPN);

  uint pageStart = PPN * pageSize;
  uint address = ((lookingForWord % pageSize) + pageStart) * FOUR;
  uint instruction = mem->getInstruction(address);
  //will always be true as it is in memory
  IN(instruction,regInput);
  cpuStep(stepsNeeded,mem,error,input,done,output,outString);
}




///////////////////////////////////////////////////////
//STEP/RUN COMMANDS END
//////////////////////////////////////////////////////





///////////////////////////////////////////////////////
//CONFIG COMMAND
//////////////////////////////////////////////////////

//PRE:output is a boolean while outString is a MyString object by
//reference
//POST:adds the amount of memory in the machine, the number of words
//of memory allocated to stack for each program, and the type of
//memory management to the outString
void Machine::config(bool & output, MyString & outString){
  output = true;
  char array1[MAXINPUT];
  sprintf(array1,"Memory:%i \nStack:%i \ntimeSlice:%i \nPageSize:%i \nSwapSpace:%i\n",maxMem,stackNum,timeSlice,pageSize,swapSpace);
  outString = outString + array1;
  if(memManageType == 0){
    //ASSERT:memManageType is first fit
    outString = outString + "mem-management:First Fit\n";
  }
  else{
    //ASSERT:memManageType is best fit
    outString = outString + "mem-management:Best Fit\n";
  }
  if(paging == 0){
    outString = outString + "paging:FIFO-First In First Out";
  }
  else{
    outString = outString + "paging:LRU=Least Recently Used";
  }
}

///////////////////////////////////////////////////////
//CONFIG COMMAND
//////////////////////////////////////////////////////






///////////////////////////////////////////////////////
//OPCODE INSTRUCTIONS START
//////////////////////////////////////////////////////



//PRE:instruction is a word of memory that holds an instruction for
//this action to do 
//POST:adds the contents of 2 registers specified by the instruction
//into a third register.
void Machine::ADD(uint instruction){
  uint reg1 = getReg1(instruction);
  uint reg2 = getReg2(instruction);
  uint reg3 = getReg3(instruction);
  registers[reg1] = registers[reg2] + registers[reg3];
  //ASSERT:the contents of registers[reg2]+registers[reg3] are held in registers[reg1]
}

    
//PRE:instruction is a word of memory that holds an instruction for
//this action to do 
//POST:performs the bitwise operator nand on the bits of 2 registers
//specified by the instruction and puts the output into another register
void Machine::NAND(uint instruction){
  uint reg1 = getReg1(instruction);
  uint reg2 = getReg2(instruction);
  uint reg3 = getReg3(instruction);
  registers[reg1] = ~(registers[reg2] & registers[reg3]);
}

    
//PRE:instruction is a word of memory that holds an instruction for
//this action to do
//POST:adds the contents of a register specified by the instruction
//to a value specified by the instruction into another register
//specified by the instruction
void Machine::ADDI(uint instruction){
  uint reg1 = getReg1(instruction);
  uint reg2 = getReg2(instruction);
  uint num = getValue(instruction);
  registers[reg1] = registers[reg2] + num;
}

//PRE:instruction is a word of memory that holds an instruction for
//this action to do , mem is a pointer to a memory object and
//error and outString record if an
//error has occured and stores the error message in outString
//POST:gets the registers it needs from the instruction and loads a
//word from memory into a register specified in the instruction
//unless the address in memory gotten from the instruction is bad
void Machine::LW(uint instruction,Memory * mem,bool & error,MyString & outString){
  uint reg1 = getReg1(instruction);//register resultant is stored in
  uint reg2 = getReg2(instruction);//register address is in
  uint offset = getValue(instruction);//addition to address in reg2
  PCB * thisPCB = PCBQueue.getNth(0);
  uint address = registers[reg2] + offset;//represents
  bool stack = false;
  if((address % FOUR) == 0){
    //ASSERT:address is divisible by 4 so it good
    //max address
    uint maxAddress = (thisPCB->getProgWords() + stackNum) * FOUR;
    if(address >= thisPCB->getProgWords()){
      //ASSERT:address may goes into stack space
      stack = true;
      if(address >= maxAddress){
	//ASSERT:address is not in prog or stack space
	error = true;
      }
      else{
	//ASSERT:address is in stack space and good
	uint lookingForWord = address/FOUR;
	//since we in stack
	lookingForWord = lookingForWord - thisPCB->getProgWords();
	uint pageNum = findWordVPN(lookingForWord,pageSize);
	uint PPN;
	bool inMemory = thisPCB->isInMem(pageNum,true,PPN);
	if(!inMemory){
	  //ASSERT:stack page wasnt in memory
	  thisPCB->addPageFault();
	  loadStackPage(thisPCB->getPID(),pageNum,error,outString);
	}
	else{
	  //ASSERT: it is in memory and we are gonna use it
	  if(paging == 1){
	    //ASSERT:paging type is LRU
	    pageTable->LRU(thisPCB->getPID(),pageNum,stack);
	  }
	}
	inMemory = thisPCB->isInMem(pageNum,true,PPN);
	if((!error) && (inMemory)){
	  //ASSERT:there was no error loading it into memory
	  uint pageStart = PPN * pageSize;
	  address = ((lookingForWord % pageSize) + pageStart) * FOUR;
	  uint wrd = mem->getWord(address/FOUR);
	  registers[reg1] = wrd;
	}
      }
      if(error){
	//ASSERT:address was not in stack or prog space
	outString = outString + "The address specified is not in stack or program space";
      }
    }
    else{
      //ASSERT:there was no problem with the address and we have the
      //correct address
      uint lookingForWord = address/FOUR;
      uint pageNum = findWordVPN(lookingForWord,pageSize);
      uint PPN;
      bool inMemory = thisPCB->isInMem(pageNum,false,PPN);
      if(!inMemory){
	//ASSERT:page was not in memory
	thisPCB->addPageFault();
	loadProgPage(thisPCB->getPID(),pageNum,error,outString);
      }
      else{
	//ASSERT:it is in memory and we are touching it
	if(paging == 1){
	  //ASSERT:paging type is LRU
	  pageTable->LRU(thisPCB->getPID(),pageNum,stack);
	}
      }
      inMemory = thisPCB->isInMem(pageNum,false,PPN);
      if((!error) && (inMemory)){
	//ASSERT:there was no problem loading it into memory
	uint pageStart = PPN * pageSize;
	address = ((lookingForWord % pageSize) + pageStart) * FOUR;
	uint wrd = mem->getWord(address/FOUR);
	registers[reg1] = wrd;
      }
      
      //ASSERT:instruction at address in memory is stored in reg1
    }
  //when doing this, to get relative address get start address from
  //PCB and add to PC then add offset, if this ends up being greater
  //than the ending prog address, do that thing - ending address of
  //prog - 4 and add that to stack starting address and that will be
  //the address to use
  }
  else{
    //ASSERT:There was an error, address is bad
    error = true;
    outString = outString + "The address specified is either not divisible by four or is not in scope of memory";
  }
}

   
//PRE:instruction is a word of memory that holds an instruction for
//this action to do, mem is a pointer to a memory object, error and
//outstring record whether there was an error in this instruction
//POST:puts the contents of register one specified by
//instruction into memory at the address speicifed by the
//instruction unless an error occured
void Machine::SW(uint instruction, Memory * mem,bool & error,MyString & outString){
  uint reg1 = getReg1(instruction);
  uint replacement = registers[reg1];
  uint reg2 = getReg2(instruction);
  uint offset = getValue(instruction);
  PCB * thisPCB = PCBQueue.getNth(0);
  bool stack = false;
  uint address = registers[reg2] + offset;
  if((address % FOUR) == 0){
    //ASSERT:address is good and atleast above or
    uint maxAddress = (thisPCB->getProgWords() + stackNum) * FOUR;
    if(address/FOUR >= thisPCB->getProgWords()){
      //ASSERT:address may go into stack space
      stack = true;
      if(address >= maxAddress){
	//ASSERT:address is not in prog or stack space
	error = true;
      }
      else{
	//ASSERT:address is in stack space and good
	uint lookingForWord = address/FOUR;
	//since we in stack
	lookingForWord = lookingForWord - thisPCB->getProgWords();
	uint pageNum = findWordVPN(lookingForWord,pageSize);
	uint PPN;
	bool inMemory = thisPCB->isInMem(pageNum,true,PPN);
	if(!inMemory){
	  //ASSERT:stack page wasnt in memory
	  thisPCB->addPageFault();
	  loadStackPage(thisPCB->getPID(),pageNum,error,outString);
	}
	else{
	  if(paging == 1){
	    //ASSERT:paging type is LRU
	    pageTable->LRU(thisPCB->getPID(),pageNum,stack);
	  }
	}
	inMemory = thisPCB->isInMem(pageNum,true,PPN);
	if((!error) && (inMemory)){
	  //ASSERT:there was no error loading it into memory
	  uint pageStart = PPN * pageSize;
	  address = ((lookingForWord % pageSize) + pageStart) * FOUR;
	  uint wrd = registers[reg1];
	  mem->replaceWord(address/FOUR,wrd);
	  pageTable->makeDirty(thisPCB->getPID(),pageNum,stack);
	}
      }
      if(error){
	//ASSERT:address was not in stack or prog space
	outString = outString + "The address specified is not in stack or program space";
      }
    }
    else{
      //ASSERT:there was no error with address and so we have the
      //correct address in prog space.
      	uint lookingForWord = address/FOUR;
	uint pageNum = findWordVPN(lookingForWord,pageSize);
	uint PPN;
	bool inMemory = thisPCB->isInMem(pageNum,true,PPN);
	if(!inMemory){
	  //ASSERT:stack page wasnt in memory
	  thisPCB->addPageFault();
	  loadStackPage(thisPCB->getPID(),pageNum,error,outString);
	}
	else{
	  //ASSERT:we are already in memory.
	  if(paging == 1){
	    //ASSERT:paging type is LRU
	    pageTable->LRU(thisPCB->getPID(),pageNum,stack);
	  }
	}
	inMemory = thisPCB->isInMem(pageNum,true,PPN);
	if((!error) && (inMemory)){
	  //ASSERT:there was no error loading it into memory
	  uint pageStart = PPN * pageSize;
	  address = ((lookingForWord % pageSize) + pageStart) * FOUR;
	  uint wrd = registers[reg1];
	  mem->replaceWord(address,wrd);
	  pageTable->makeDirty(thisPCB->getPID(),pageNum,stack);
	}
    }
  }
  else{
    //ASSERT:there was an error with the address
    error = true;
    outString = outString + "The address specified is either not divisible by four or is not in scope of memory";
  }
}

  
  //PRE:instruction is a word of memory that holds an instruction for
//this action to do 
//POST:if the two registers in the instruction match, set the PC to PC=PC+Label
void Machine::BEQ(uint instruction){
  uint reg1 = getReg1(instruction);
  uint reg2 = getReg2(instruction);
  uint label = getValue(instruction);
  if(registers[reg1] == registers[reg2]){
    //ASSERT:contents of the registers are equal
    //so set PC to label
    PC = PC + label;
  }
}


//PRE:instruction is a word of memory that holds an instruction for
//this action to d o
//POST:sets contents of register y specified in instruction to PC
//and stores contents PC in register x specified in instruction
void Machine::JALR(uint instruction){
  uint reg1 = getReg1(instruction);
  uint reg2 = getReg2(instruction);
  if((registers[reg1] % FOUR) == 0){
    //ASSERT:whatever is in registers[reg1] is a valid pc number
    registers[reg2] = PC;
    //ASSERT:contents of PC is held in registers[reg2]
    PC = registers[reg1];
    //ASSERT:PC value is registers[reg1] now
  }
}

//PRE:object is defined
//POST:Halts the program and deletes the PCB in the front of the queue
//from the queue and deletes the pointer in it to get rid of mem leaks
void Machine::HALT(){
  PCB * deleteThis = PCBQueue.getNth(0);
  pageTable->deletePID(deleteThis->getPID());
  //ASSERT:all instances of PID have been removed from pageTable
  PCBQueue.deQueue();//node holding object in queue has been deleted
  delete deleteThis;//object on stack has been deleted
}

    
//PRE:instruction is a word of memory that holds an instruction for
//this action to do 
//POST:puts regInput into a register specified by instruction
void Machine::IN(uint instruction, uint regInput){
  uint reg1 = getReg1(instruction);
  registers[reg1] = regInput;
}

//PRE:instruction is a word of memory that holds an instruction for
//this action to do 
//POST:outputs the contents of register specifed in instruction to shell
void Machine::OUT(uint instruction,bool & output, MyString & outString){
  output = true;
  //we need to output a number
  uint reg1 = getReg1(instruction);
  uint outputNum = registers[reg1];
  char array[MAXINPUT];
  sprintf(array,"%i ",outputNum);
  //array holds outputnum as chars
  outString = outString + array;
}


//PRE:instruction is a word of memory that holds an instruction for
//this action to do 
//POST:stores the address specified in instruction in a register
//specified by instruction
void Machine::LA(uint instruction){
  uint reg1 = getReg1(instruction);
  uint value = getValue(instruction);//gets address of label from
				     //offset
  registers[reg1] = PC + value;//set registers[reg1] to address gotten
			       //by adding PC to value
}
  
//PRE:instruction is a word of memory that holds an instruction for
//this action to do 
//POST:if contents of reg1> contents of reg2 in instruction set PC = PC + label
void Machine::BGT(uint instruction){
  uint reg1 = getReg1(instruction);
  uint reg2 = getReg2(instruction);
  uint value = getValue(instruction);
  if(registers[reg1] > registers[reg2]){
    //ASSERT:contents of registers[reg1] are greater than the contents
    //of registers[reg2] so set PC to value
    PC = PC + value;
  }
}


//PRE:Object is defined and satisfys CI
//POST:sets PC to value
void Machine::setPC(uint value){
  PC = value;
}

//PRE:Object is defined and satisfys the CI
//POST:sets SPR to value
void Machine::setSP(uint value){
  registers[SPR] = value;
}



//PRE:step or run from cpu is asking for the opcode of a instruction
//POST:returns the opcode of the instruction
uint Machine::getOpcode(uint instruction){
  uint rv = instruction & opcodeMask;
  rv = rv >> OPCODESHIFT;
  return rv;
  
}



//PRE:instruction is a word from memory
//POST:returns the bits corresponding with the first register in
//memory that the instruction specifies 
uint Machine::getReg1(uint instruction){
  uint rv = instruction & reg1Mask;
  rv = rv >> REG1SHIFT;
  return rv;
}


//PRE:instruction is a word from memory
//POST:returns the bits corresponding with the first register in
//memory that the instruction specifies 
uint Machine::getReg2(uint instruction){
  uint rv = instruction & reg2Mask;
  rv = rv >> REG2SHIFT;
  return rv;
}


//PRE:instruction is a word from memory
//POST:returns the bits corresponding with the first register in
//memory that the instruction specifies 
uint Machine::getReg3(uint instruction){
  uint rv = instruction & reg3Mask;
  return rv;//reg3 is already leftmost bits
}


//PRE:instruction is a word from memory
//POST:returns the bits corresponding with the value that the
//instruction specifies 
uint Machine::getValue(uint instruction){
  uint rv = instruction & valueMask;
  uint isNeg = rv & isNegativeMask;
  if(isNeg != 0){
    //ASSERT:rv is negative
    rv = rv | makeNegativeMask;
    //ASSERT:rightmost 12 bits have been padded with 0's
  }
  return rv;//value is already leftmost bits
}

///////////////////////////////////////////////////////
//OPCODE INSTRUCTIONS END
//////////////////////////////////////////////////////


//PRE:word represents the word that is being modified, byteNum
//represents the bytenum from the left, 0 for leftmost, 3 for
//rightmost, that the byte replacement is replacing
//POST:replaces the byte number byteNum of word with replacement and
//returns it
uint Machine::replaceByte(uint word, uint byteNum, uint replacement){
  uint bitMask = BITMASKSTART;//bitMask has 24 0's and 8 1's on right
  uint maxShift = FOUR - 1;
  bitMask = bitMask << (maxShift - byteNum) * BITSPERBYTE;
  bitMask = ~bitMask;
  //ASSERT:bitmask is all 1's except for the byteNumth byte which is
  //0's
  uint temp = word & bitMask;
  uint shiftedReplacement = replacement <<(maxShift - byteNum) * BITSPERBYTE;
  //ASSERT:the replacement has been shifted into the right byte spot
  uint rv = temp | shiftedReplacement;
  //ASSERT:rv contains the original word with its byteNumth replaced
  //with replacement
  return rv;
}

//PRE:called in destructor
//POST:deletes all things on heap that may still be in PCBQueue if
//exit command is called before all programs are run
void Machine::cleanPCBQueue(){
  uint numNodes = PCBQueue.getNumNodes();
  for(int index = 0; index < numNodes; index++){
    PCB * aPCB = PCBQueue.getNth(0);//get 0th
    delete aPCB;//delete it
    PCBQueue.deQueue();//dequeue it to move another to front
  }
  //ASSERT:all things that were allocated on stack in PCBQueue have
  //been deallocated
}

//PRE:called in destructor
//POST:deletes all things on heap that are in memBlockList
void Machine::cleanMemBlockList(){
  uint numNodes = memBlockList.getNumNodes();
  for(int index = 0; index < numNodes; index++){
    freeMemBlock * aBlock = memBlockList.getNth(0);
    delete aBlock;
    memBlockList.deleteNth(0);
  }
  //ASSERT:all things that were allocated on stack in PCBQueue have
  //been deallocated
}




//destructor
Machine::~Machine(){
  cleanPCBQueue();
  cleanMemBlockList();
  delete mem;
}
