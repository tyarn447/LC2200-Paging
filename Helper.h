#ifndef INCLUDED_HELPER
#define INCLUDED_HELPER
#include "MyString.h"
#include "Machine.h"
#include <iostream>
#include <fstream>
using namespace std;

//PRE:argv is a character array 
//POST:takes the number specified in chars in the array and makes it into an int
int charToInt(char argv[]);


//PRE:numWords contains an integer representing the number of words of
//memory our maching has,stackNum contains the number of words to of
//stack space to give each program in the machine, and memManageType
//gives the type of memory management to use for the machine, pagesize
//gives the physical byte size for each page, swapspace gives the size
//of the swapspace file and paging gives the paging type being used
//POST:simulates a shell, where the user can input commands to a
//simulated machine, this function will stop running if the user
//inputs "exit"
void shell(int numWords,int stackNum,int memManageType, int timeslice,
	   int pagesize, int swapspace, int paging);


//PRE:output, error,needInput and done are all references to booleans
//to tell the shellHelper what to do and such, outString is a Mystring
//object. This function should only be called when done = false.Delta
//is a reference to a machine
//POST:deals with inputting and outputting things to the shell when
//the machine is executing programs.
void shellHelper(bool & output, bool & error, bool & needInput,bool & done, MyString & outString, Machine & Delta);

//PRE:input is the char array that will hold the input on the cmd
//line
//POST:reads the line that was input to the shell until the new line
//character
void readLine(char array[]);

//PRE:string1 represents an array of chars as does string2
//POST:compares the two arrays, if they are the same returns true,
//else returns false
bool compare(char string1[], char string2[]);


//PRE:string is a char array
//POST:returns the number of elements up until end of string
//character.
int getLength(char string[]);


//PRE:string1 and string2 are both arrays of chars
//POST:returns a char * that is string1 concatenated with string2
char * concat(char string1[], char string2[]);


//PRE:line is an array of chars that represent multiple words,
//command is a pointer to an array of chars, each array represents a
//command or parameter to a command
//POST:turns each word, separated by a space, into its own char
//array, also returns the number of things in the char array.
uint tokenize(char line[],char cmds[MAXCMDS][MAXINPUT]);

//PRE:line is an array of chars, start is where in the array the
//function should start counting
//POST:returns the place in the array where the next white space is
int skipBlanks(char line[], int start);

//PRE:takes a ifstream object and 7 parameters by reference that
//signify the stack space allowed to a function, the memory management
//type and the amout of memory, timeslice number, pagesize, swapspace
//allowed, and paging type.
//POST:changes the by reference parameters based on what is in the
//ifstream object.
void getParameters(ifstream & configFile, int & memorySpace, int & stack, int & memManageType, int & timeslice,int & pagesize, int & swapspace, int & paging);



//PRE:memArray is a filled up array of freeMemBlock objects, start is
//the start of the array, generally 0 on first call, end is theindex
//of the freememBlock  objects in memArray on first call but just
//simpl y an int representing the end of the array we are looking at.
//POST:organizes list by using quicksort method
void quickSort(freeMemBlock * memArray[], uint start, uint end);


//PRE:memArray is a filled up array of freeMemBlock objects, start is
//the beggining of the part of the array being looked at, end is the
//index of the the last object in memArray
//POST:return position the partition element was swapped into
uint partition(freeMemBlock * memArray[],uint start,uint end);

//PRE:wordNum represents a word in a file,pageSize represents the size
//of each page,pageSize>0
//POST:returns the VPN that this word would be on
uint findWordVPN(uint wordNum,uint pageSize);

//PRE:PPN represents the physical page number, PPN>=0
//POST:returns the physical location in memory that the page number
//represents.
uint getAddress(uint PPN);

#endif
