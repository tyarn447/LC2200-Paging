#ifndef INCLUDED_MACHINE
#define INCLUDED_MACHINE
#include "Constants.h"
#include "MyString.h"
#include "Memory.h"
#include "LinkedList.h"
#include "freeMemBlock.h"
#include "PCB.h"
#include "Queue.h"
#include "PageTable.h"
class Machine{
  
  //CLASS INVARIANT:mem is a pointer to a memory object, cpu holds a
  //cpu object. Max mem holds the maximum amount of memory allowed to
  //the memory object. stepFunc and runFunc tells the machine which
  //current function is still running, the two cannot be true at the
  //same time
  
 private:
  //Machine stuff
  bool stepFunc;//initialze to false, says whether we are still in step function
  bool runFunc;//initialze to false, says whether we are still in run function
  Memory * mem;
  uint timeSliceCounter;//reflects the number of timeslices we have done for a step
  uint PC;//program counter of the machine
  uint registers[NUMREGISTERS];//registers
  bool finishTimeSlice;
  int stepsNeeded;//amount of steps needed for step func
  int stepsTaken;//amount of steps taken for step func
  uint pidCounter;//initialze to 0
  bool loadedProgram;//tells you if the program has been loaded into memory yet or not

  //Integral Machine Parameters
  uint stackNum;//how much stack space should be given to each program
  uint maxMem;//holds the maximum amount of memory allowed for machine
  uint timeSlice;//reflects the number of instructions for each step
  uint memManageType;//0 denotes first fit, 1 denotes best fit
  uint pageSize;//denotes the byte size of each page in memory
  uint swapSpace;//denotes the byte size of swapSpac
  uint paging;//0=FIFO,  1=LRU
  uint numPages;
  
  //Lists for machine
  LinkedList <freeMemBlock *> memBlockList;
  Queue <PCB *> PCBQueue;
  PageTable * pageTable;
  
  
 public:
  //Constructor
  //PRE:
  //POST:Machine Object created
  Machine();
  
  //Constructor
  //PRE:numWords is the amount of memory available to machine, pstack is
  //the amount of space that should be given to each program for the
  //stack and pMemManageType is the type of memory
  //management,pPagesize is the physical bytes of each page of mem,
  //swapspace denotes size of swapspace file, paging denotes type of
  //paging.
  //POST:Machine Object is created with max memory,space for each stack
  //and memory management type
  Machine(int numWords,int pStack, int pMemMangageType, int pTimeslice,
	  int pPagesize, int pSwapspace, int pPaging);
  
  //PRE:line represents the line that was input that holds commands,
  //all the booleans will be used to tell the shell if something
  //should be done on return, array will be used to transfer output
  //and input from the shell to the machine and vice versa
  //POST:processes this line, parses it, and decides what cmd to run,
  //returns booleans telling the shell what to do.
  void process(char line[],bool & error, bool & input, bool & output, bool & done, MyString & outString);
  
  
  
  
  //PRE:takes a char * array which is compares to other char arrays to
  //see if they are the same
  //POST:returns a code representing what command, if not any, to
  //execute.
  void findFunc(char cmds[MAXCMDS][MAXINPUT],uint numCmds,bool & error, bool & input, bool & output, bool & done, MyString & outString);
  
  //PRE:cmds holds the tokens from the parsed line, numcmds holds the
  //amount of cmds in cmds, error and done are booleans by reference
  //to tell the shell if there was an error or if the function is
  //done.
  //POST:performs the load function if the correct number of cmds were
  //specified.
  void load(char cmds[MAXCMDS][MAXINPUT],uint numCmds, bool & error,bool & output, bool & done,MyString & outString);
  
  //PRE:inputFile is the name of a readable input file, done and error
  //represent booleans that can be changed to represent whether the
  //function is done or not
  //POST:loads the contents of the program into memory.
  void loadProgram(fstream * inputFile, bool & done, bool & error,bool & output,MyString name,MyString & outString);
  
  
  //PRE:PID represents the PID of the program we are loading into
  //memory, and VPN represents the virtual page number that we want to
  //load into memory.This PID and VPN should not already be present in
  //memory
  //POST:loads the memory that is on the VPN page from the program into
  //memory and makes a pageInfo object to put into the pagetable to
  //represent that it is there.
  void loadProgPage(uint PID, uint VPN,bool & error,MyString & outString);
  
  //PRE:PID represents the PID of the program we are loading into memory
  //and VPN represents the virtual page number for stack that we want to
  //load into memory, This PID and VPN should not be aleady present in
  //memory for stack.
  void loadStackPage(uint PID, uint VPN,bool & error,MyString & outString);



  //PRE:we have loaded VPN and PID into pageTable with no problems
  //POST:loads VPN from process with PID=PID into memory that is
  //referenced by PPN
  //POST:loads VPN from PID into the PPN(physical Page) of memory
  void loadIntoMem(uint PID, uint VPN, bool isStack, uint PPN);
  

  //PRE:PCB* thisPCB is a pointer to a PCB,VPN is a uint that references
  //a virtual page number, PPN is a uint that references Physical Page
  //Number.
  //POST:loads VPN from PCB into memory at PPN location
  void loadIntoMemHelper(uint VPN,uint PPN,PCB * thisPCB);

  //PRE:object satisfies the CI
  //POST:updates the ProcessPageTable of PCB with PID==PID
  void updateProcessPageTable(uint PID, uint VPN, bool stackBool, bool used,uint PPN);
  
  //PRE:numwords represents number of words we need for the program,
  //pStart,pEnd,sStart and sEnd all represent addresses for program
  //start/end and stack start/end
  //POST:returns true if there is memory avaiable for the program and
  //stack, it also put the addresses of that memory into the parameters
  //passed by reference
  bool findMem(uint numWords,uint & pStart,uint & pEnd,uint & sStart,uint & sEnd);
  
  
  
  //PRE:object is defined and satisfies the CI, memBlock is a block of
  //free memory that was just freed and is being given back to the free
  //memory list
  //POST:adds memBlock to the freeMemList, if memBlock is next to
  //another free block of memory, the two will be spliced together, if
  //not it will be added to the front of the memory list.
  void addToFreeMem(freeMemBlock * memBlock);
  

  //PRE:numwords represents the number of words needed from memory,
  //start and end represent the addresses that will be gotten if the
  //program finds memory big enough for numWords
  //POST:returns true if there is memory large enough for numWords and
  //puts addresses in start and end accordingly, false otherwise, finds
  //the first memory large enough to hold numWords and uses that
  bool firstFit(uint numWords, uint & start, uint & end);
  
  
  //PRE:numwords represents the number of words needed from memory,
  //start and end represent the addresses that will be gotten if the
  //program finds memory big enough for numWords
  //POST:returns true if there is memory large enough for numWords and
  //puts addresses in start and end accordingly, false otherwise, finds
  //the best closest amount of memory to numWords and uses that.
  bool bestFit(uint numWords, uint & start, uint & end);
  

  
  //PRE:cmds holds the tokens from the parsed, numcmds holds the
  //amount of cmds in the cmds array. error and done are booleans by
  //reference to tell the shell if there was an error or if the
  //function completed correctly.
  //POST:calls the appropriate function from the memory class.
  void memFunc(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error, bool & done,bool & output, MyString & outString);


  
  //PRE:object is defined and satisfies the CI
  //POST:sorts the memory blocks in memBlocklist using quicksort
  void sortMemory();



  //PRE:jobs was typed into shell, cmds are the tokens from the cmd
  //line, numcdms is number of tokens,error output and outString are
  //booleans/MyStrings to tell the shell to output stuff and what to
  //output
  //POST:calls a function that puts all information from PCBQueue into outstring
  void jobFunc(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error,bool & output, MyString & outString);
  
  
  //PRE:cmds represents the commands on the input line, numCmds is the
  //number of commands on the command line, error and output are
  //booleans that the shell will read, outString will be the string
  //output to the shell
  //POST:checks to see which pages function to use and executes the
  //correct one
  void pagesFunc(char cmds[MAXCMDS][MAXINPUT],uint numCmds,bool & error,bool & output, MyString & outString);
  

  //PRE:freemem was typed into shell, cmds are the tokens from the cmd
  //line, numcdms is number of tokens,error output and outString are
  //booleans/MyStrings to tell the shell to output stuff and what to
  //output
  //POST:calls a function that puts all information from memBlockList into outString
  void freememFunc(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error,bool & output, MyString & outString);
  
  
  //PRE:kill was typed into shell, cmds are the tokens from the cmd
  //line, numcdms is number of tokens,error output and outString are
  //booleans/MyStrings to tell the shell to output stuff and what to
  //output
  //POST:calls the kill command on a specific PID if the correct amount
  //of cmds were on command line, returns an error message otherwise
  void killFunc(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error,bool & output, MyString & outString);
  
    
  //PRE:pid is the pid we are looking for to take out of PCBQueue,
  //bool error and output will tell the shell what to output and
  //outString holds that output
  void kill(uint pid, bool & error,bool & output,MyString outString);
  
		  
  //PRE:numCmds is an int to tell how many commands were specified on
  //the command line, error and done and output all tell the shell
  //that there is an output on the command line ready, outstring is
  //what holds the output.
  //POST:Puts the contents of the registers and PC in outstring as a
  //string and tells the shell to output this string.
  void cpuFunc(uint numCmds, bool & error, bool & done, bool & output, MyString & outString);

    
  //PRE:num cmds is how many things are in the cmds array, error and
  //done and booleans that will tell the shell things, outstring is
  //where any output to he shell is held
  //POST:gets commands from memory and executes them
  void step(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error, bool & done, bool & input,bool & output,MyString & outString);



  //PRE:object is defined and satisfies the CI
  //POST:if there is anything in the PCBQueue it will rotate it by one
  //slot
  void rotatePCBQueue();

  //PRE:we are in the stepping or running a program
  //POST:copies all relevant information in the PCB to the cpu.
  void copyPCBToCPU();
  
  //PRE:we have finished step and run and have not halted a program
  //POST:copies all relevant information in the CPU to the PCB
  void copyCPUToPCB();
  
  
  //PRE:boolean stepFunc is true
  //where any output to he shell is held
  //POST:gets commands from memory and executes them
  void step(bool & error, bool & done,bool & input,bool & output, MyString & outString,uint regInput);

    
  //PRE:num cmds is how many things are in the cmds array, error and
  //done and booleans that will tell the shell things, outstring is
  //where any output to he shell is held
  //POST:gets commands from memory and executes them
  void run(char cmds[MAXCMDS][MAXINPUT], uint numCmds, bool & error, bool & done, bool & input,bool & output,MyString & outString);

  
  //PRE:outString is a Mystring object by reference that holds strings
  //POST:the contents of the CPU, such as the PC and registers are
  //held in outString.
  void displayContents(MyString & outString);


  //PRE:output is a boolean while outString is a MyString object by
  //reference
  //POST:adds the amount of memory in the machine, the number of words
  //of memory allocated to stack for each program, and the type of
  //memory management to the outString
  void config(bool & output, MyString & outString);


  
  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do 
  //POST:adds the contents of 2 registers specified by the instruction
  //into a third register.
  void ADD(uint instruction);

    
  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do 
  //POST:performs the bitwise operator nand on the bits of 2 registers
  //specified by the instruction and puts the output into another register
  void NAND(uint instruction);

    
  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do
  //POST:adds the contents of a register specified by the instruction
  //to a value specified by the instruction into another register
  //specified by the instruction
  void ADDI(uint instruction);

  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do , mem is a pointer to a memory object and
  //error and outString record if an
  //error has occured and stores the error message in outString
  //POST:gets the registers it needs from the instruction and loads a
  //word from memory into a register specified in the instruction
  //unless the address in memory gotten from the instruction is bad
  void LW(uint instruction,Memory * mem,bool & error,MyString & outString);

   
  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do, mem is a pointer to a memory object, error and
  //outstring record whether there was an error in this instruction
  //POST:puts the contents of register one specified by
  //instruction into memory at the address speicifed by the
  //instruction unless an error occured
  void SW(uint instruction,Memory * mem,bool & error,MyString & outString);

    
  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do 
  //POST:if the two registers in the instruction match, set the pc to
  //the label speicifed in the instruction
  void BEQ(uint instruction);

    
  //PRE:instruction is a word of memory that holds an instruction for
  //this action to d o
  //POST:sets contents of register y specified in instruction to PC
  //and stores contents PC in register x specified in instruction
  void JALR(uint instruction);

  //PRE:object is defined
  //POST:Halts the program
  void HALT();

    
  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do 
  //POST:puts regInput into a register specified by instruction
  void IN(uint instruction, uint regInput);

  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do 
  //POST:outputs the contents of register specifed in instruction to shell
  void OUT(uint instruction,bool & output, MyString & outString);


  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do 
  //POST:stores the address specified in instruction in a register
  //specified by instruction
  void LA(uint instruction);
  
  //PRE:instruction is a word of memory that holds an instruction for
  //this action to do 
  //POST:if reg1>reg2 in instruction set PC to label
  void BGT(uint instruction);
  
  //PRE:Object is defined and satisfys CI
  //POST:sets PC to value
  void setPC(uint value);

  //PRE:Object is defined and satisfys the CI
  //POST:sets SPR to value
  void setSP(uint value);

  //PRE:numSteps represents the amount of commands in memory we should
  //execute, mem is a reference to the memory object, error input and
  //done are all boolean references to give to the shell
  //POST:executes numsteps commands in memory or until halt command
  void cpuStep(uint numSteps,Memory * mem,bool & error,bool & input,bool & done,bool & output, MyString & outString);

  //PRE:this step will be called after outputting the something to the
  //shell so you dont have to reinput stepsneeded in the machine class,
  //it will do it here
  //POST:calls step function with the correct amount of steps needed
  void cpuStep(Memory * mem,bool & error,bool & input,bool & done,bool & output, MyString & outString);
  

  //PRE:step had asked for input, this is the continuation of this step
  //POST:continues to execute commands specified by the stepsNeeded member data
  void inStep(Memory * mem, bool & error, bool & input, bool & done,bool & output, MyString & outString,uint regInput);


  //PRE:PC is the program counter
  //POST:returns the word in memory that the PC is at
  uint getInstruction(uint PC);
  
  //PRE:step or run from cpu is asking for the opcode of a instruction
  //POST:returns the opcode of the instruction
  uint getOpcode(uint instruction);

  //PRE:instruction is a word from memory
  //POST:returns the bits corresponding with the first register in
  //memory that the instruction specifies 
  uint getReg1(uint instruction);


  //PRE:instruction is a word from memory
  //POST:returns the bits corresponding with the first register in
  //memory that the instruction specifies 
  uint getReg2(uint instruction);
  
  
  //PRE:instruction is a word from memory
  //POST:returns the bits corresponding with the first register in
  //memory that the instruction specifies 
  uint getReg3(uint instruction);
  
  
  //PRE:instruction is a word from memory
  //POST:returns the bits corresponding with the value that the
  //instruction specifies 
  uint getValue(uint instruction);

  //PRE:word represents the word that is being modified, byteNum
  //represents the bytenum from the left, 0 for leftmost, 3 for
  //rightmost, that the byte replacement is replacing
  //POST:replaces the byte number byteNum of word with replacement and
  //returns it
  uint replaceByte(uint word, uint byteNum, uint replacement);

  //PRE:called in destructor
  //POST:deletes all things on heap that may still be in PCBQueue if
  //exit command is called before all programs are run
  void cleanPCBQueue();
  
  //PRE:called in destructor
  //POST:deletes all things on heap that are in memBlockList
  void cleanMemBlockList();
  

  
  //Destructor
  ~Machine();
};


#endif
