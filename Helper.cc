#include "Helper.h"
#include <iostream>
#include <fstream>
#include "Constants.h"
#include "MyString.h"
#include "Machine.h"
#include "freeMemBlock.h"
using namespace std;


//PRE:argv is a character array 
//POST:takes the number specified in chars in the array and makes it into an int
int charToInt(char array[]){
  int counter = 0;
  bool negative = false;
  while(array[counter + 1] != EOS){
    counter++;
  }
  //only have this for forloop here so i can change it if number is
  //negative 
  int counter2 = 0;
  //ASSERT:counter is at position before EOS
  if(array[0] == NEGATIVE){
    //ASSERT:number is negative
    counter --;//deiterate counter to reflect we have one less num to
	       //multiply
    counter2 = 1;//change array index so we dont get value of -
    negative = true; 
  }
  int multiplier = 1;
  for(int index = 0; index < counter; index ++){
    multiplier *= BASE;
  }
  //ASSERT:multiplier is at correct value
  int returnNum = 0;
  while(array[counter2] != EOS){
    int number =(int)array[counter2] - ASCIIZERO;//ASSERT:number holds char in int form
    number = number * multiplier;
    multiplier /= BASE;
    returnNum = returnNum + number;
    counter2++;
  }
  if(negative){
    //ASSERT:number is negative
    returnNum = returnNum * -1;
  }
  //ASSERT:we have the correct returnNum
  return returnNum;
}





//PRE:numWords contains an integer representing the number of words of
//memory our maching has,stackNum contains the number of words to of
//stack space to give each program in the machine, and memManageType
//gives the type of memory management to use for the machine, pagesize
//gives the physical byte size for each page, swapspace gives the size
//of the swapspace file and paging gives the paging type being used
//POST:simulates a shell, where the user can input commands to a
//simulated machine, this function will stop running if the user
//inputs "exit"
void shell(int numWords,int stackNum, int memMangageType,int timeslice,
	   int pagesize, int swapspace, int paging){
  Machine Delta(numWords,stackNum,memMangageType,timeslice,
		pagesize, swapspace, paging);
  bool finished = false;
  while(!finished){
     cout<< ">";
    char line[MAXINPUT];
    readLine(line);
    if(compare(line,CHAREXIT)){
      //ASSERT:the input was "exit"
      finished = true;
    }
    else{
      bool error = false;//true if outstring holds error
      bool needInput = false;//will change to true if a function needs input
      bool output = false;//true if output is in outString
      bool done = true;//false if run or step is not done
      MyString outString;//holds output from program
      Delta.process(line,error,needInput,output,done,outString);
      if(error){
      	//ASSERT:Array should contain error message
      	cout<<outString<<endl;
      }
      while(!done){
	//done will get changed to false only in step and run when it
	//is trying to input or output something
	shellHelper(output,error,needInput,done,outString,Delta);
      }
      if(output){
	//ASSERT:the array holds some output, print that output
        cout<<outString<<endl;	
      }
    }
  }
}

//PRE:output, error,needInput and done are all references to booleans
//to tell the shellHelper what to do and such, outString is a Mystring
//object. This function should only be called when done = false. Delta
//is a reference to a machine
//POST:deals with inputting and outputting things to the shell when
//the machine is executing programs.
void shellHelper(bool & output, bool & error, bool & needInput,bool & done, MyString & outString, Machine & Delta){
  if(output | error){
    //ASSERT:we need to output something
    cout<<outString<<endl;
    output = true;//just incase it isnt
    //ASSERT:we have outputted something
    error = false;//just in case
  }
  
  char inputLine[MAXINPUT];//this must be made anyways
  if(needInput){
	  //ASSERT:the program is asking for input 
    cout<<outString<<endl;
    
    readLine(inputLine);
    //ASSERT:we are reading a line to pass to process
  }
  MyString newOutString;
  outString = newOutString;//do this to make outString empty
  //after you have outputted just
  //incase outString contained
  //something
  //put inputline into process even if it empty, process will
  //see that output is still true and do something where it
  //doesnt need anything in inputline
  Delta.process(inputLine,error,needInput,output,done,outString);
}

//PRE:input is the char array that will hold the input on the cmd
//line
//POST:reads the line that was input to the shell until the new line
//character
void readLine(char array[]){
  bool finished = false;
  int counter = 0;
  while(!finished){
    char thischar;
    thischar = cin.get();
    if(thischar == NEWLINE){
      //ASSERT:we are are the end of the line
      finished = true;
    }
    else{
      //ASSERT:This is not the end of the line
      array[counter] = thischar;
      counter++;
    }
  }
  //ASSERT:The whole line is in the array
  array[counter] = EOS;//ADD EOS to end of line
}


//PRE:string1 represents an array of chars as does string2
//POST:compares the two arrays, if they are the same returns true,
//else returns false
bool compare(char string1[], char string2[]){
  bool rv = true;
  int len1 = getLength(string1);
  int len2 = getLength(string2);
  if(len1 != len2){
    //ASSERT:Lengths are not the same so false
    rv = false;
  }
  int counter = 0;
  while((counter < len1) && (rv == true)){
    char char1 = string1[counter];
    char char2 = string2[counter];
    if(char1 != char2){
      //ASSERT:char in same position are not the same so false
      rv = false;
    }
    else{
      counter ++;
    }
  }
  //ASSERT:returning true if they are the same string, false otherwise
  return rv;
}

//PRE:string is a char array
//POST:returns the number of elements up until end of string
//character.
int getLength(char string[]){
  int rv = 0;
  bool finished = false;
  while(!finished){
    char single;
    single = string[rv];
    if(single != EOS){
      //ASSERT:single is not EOS
      rv++;
    }
    else{
      //ASSERT:we have found the EOS 
      finished = true;
    }
  }
  return rv;
}

//PRE:string1 and string2 are both arrays of chars
//POST:returns a char * that is string1 concatenated with string2
char * concat(char string1[], char string2[]){
  int len1 = getLength(string1);
  int len2 = getLength(string2);
  int len = len1 + len2 + 1;
  char * rv = new char[len];
  int rvIndex = 0;
  for(int index = 0; index < len1; index++){
    rv[index] = string1[index];
    rvIndex++;
  }
  for(int index = 0; index < len2; index++){
    rv[rvIndex] = string2[index];
    rvIndex++;
  }
  rv[rvIndex] = EOS;
  return rv;
}

//PRE:line is an array of chars, start is where in the array the
//function should start counting
//POST:returns the place in the array where the next white space is
int skipBlanks(char line[], int start){
  int returnVal = start;
  bool finished = false;
  while(!finished){
    if(line[returnVal] == SPACE){
      //ASSERT:we are on a space char
      returnVal++;
    }
    else{
      //ASSERT:we have reached a nonblank char
      finished = true;
    }
    //ASSERT:we are returning the index of the next nonblank char
    return returnVal;
  }
}

//PRE:line is an array of chars that represent multiple words,
//wordArray is a pointer to an array of chars, each array represents a
//command or parameter to a command
//POST:turns each word, separated by a space, into its own char
//array, also returns the number of things in the char * wordArray[].
uint tokenize(char line[], char cmds[MAXCMDS][MAXINPUT]){
  int lineIndex = 0;
  uint numTokens = 0;
  bool finished = false;
  while(!finished){
    //ASSERT:we are not finished tokenizing
    int index = 0;
    lineIndex = skipBlanks(line, lineIndex);
    while((line[lineIndex]) != SPACE && (line[lineIndex] != EOS)){
      //ASSERT:character is not a space or EOS
      cmds[numTokens][index] = line[lineIndex];
      index++;
      lineIndex++;
    }
    //ASSERT:we  are finshed with one token, lineindex is either at a space or EOS
    
    cmds[numTokens][index] = EOS;//set EOS Char at end of token
    numTokens++;
    if(line[lineIndex] == EOS){
      finished = true;
    }
  }
  return numTokens;
}



//PRE:takes a ifstream object and 7 parameters by reference that
//signify the stack space allowed to a function, the memory management
//type and the amout of memory, timeslice number, pagesize, swapspace
//allowed, and paging type.
//POST:changes the by reference parameters based on what is in the
//ifstream object.
void getParameters(ifstream & configFile, int & memorySpace, int & stack, int & memManageType, int & timeslice,int & pagesize, int & swapspace, int & paging){
  uint numParameters = 7;
  for(int index = 0; index < numParameters; index++){
    char posParam[MAXINPUT];
    configFile >> posParam;
    if(compare(posParam,CHARMEMORY)){
      //ASSERT:we are on the line with memory
      configFile >> memorySpace;
      
    }
    else if(compare(posParam,CHARSTACK)){
      //ASSERT:we are on the line with stack
      configFile >> stack;
    }
    else if(compare(posParam,CHARMEMMANAGEMENT)){
      //ASSERT:we are on the line with memManageType
      configFile >> memManageType;
    }
    else if(compare(posParam,CHARTIMESLICE)){
      //ASSERT:we are on the line wiht timeslice
      configFile >> timeslice;
    }
    else if(compare(posParam,CHARPAGESIZE)){
      //ASSERT:we are on the line with pagesize
      configFile >> pagesize;
    }
    else if(compare(posParam,CHARSWAPSPACE)){
      //ASSERT:we are on the line with swapspace
      configFile >> swapspace;
    }
    else{
      //ASSERT:we are on the line with paging
      configFile >> paging;
    }
  }
}


//PRE:memArray is a filled up array of freeMemBlock objects, start is
//the start of the array, generally 0 on first call, end is theindex
//of the freememBlock  objects in memArray on first call but just
//simpl y an int representing the end of the array we are looking at.
//POST:organizes list by using quicksort method
void quickSort(freeMemBlock * memArray[], uint start, uint end){
  if(start < end){
    uint mid = partition(memArray,start,end); //find pivot val/swap stuff around in algorithm
    quickSort(memArray,start,mid - 1);//mid - 1 and mid + 1 because
				      //mid is partition.
    
    quickSort(memArray,mid + 1,end);//divides the array socan perform
				    //quicksort on it again.
  }
}


//PRE:memArray is a filled up array of freeMemBlock objects, start is
//the beggining of the part of the array being looked at, end is the
//index of the the last object in memArray
//POST:return position the partition element was swapped into
uint partition(freeMemBlock * memArray[],uint start,uint end){
  uint left = start + 1;
  uint right = end - 1;
  bool finished = false;
  while(finished == false){
    while((memArray[left]->getStart() < memArray[start]->getStart()) && (left < right)){
      left ++;
      
    }
    while(memArray[right]->getStart() > memArray[start]->getStart()){
      //while the right of the array is greater than the start,
      //iterate right down by 1 each time 
      right --;
      
    }
    if(right >= left){//swapping
      freeMemBlock * swap = memArray[right];
      //pointer to a pointer, copying pointer.
      memArray[right] = memArray[left];
      memArray[left] = swap;
    }
    finished = true; 
  }
  freeMemBlock * swap = memArray[right];//swap the pivot val.
  memArray[right] = memArray[start];
  memArray[start] = swap;
  return right;
}



//PRE:wordNum represents a word in a file, pageSize represents the
//size of each page,pageSize > 0;
//POST:returns the VPN that this word would be on
uint findWordVPN(uint wordNum,uint pageSize){
  uint rv;
  if(pageSize == 1){
    //ASSERT:the pageSize is 1 word
    rv = wordNum;
  }
  else{
    //ASSERT:the pageSize is not 1
    rv = wordNum/pageSize;
  }
  return rv;
}
