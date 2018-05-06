#include "MyStringHelper.h"
#include "MyStringConstants.h"

//PRE:both newArray and oldArray are arrays that have been created on the heap, old array is full of chars while newArray is garbage except for last value which is EOS, oldArrayLen is an int representing the length of the old array. This can only be used when a char is being added to myString and the array isnt big enough to hold it. newArray is twice the size of oldArray. Will also be used in cstr.
//POST:copies oldArray into newArray
void copyOver(char * newArray, char * oldArray,int lenOldArray){
  int index = 0;
  for(index; index < lenOldArray; index++){
    newArray[index] = oldArray[index];
  }
  
}



//PRE:first and second are ints, first and second cannot be equal.
//POST:returns the lower of the two ints.
int returnLower(int first, int second){
  int returnVal = first;
  if(first > second){
    returnVal = second;
  }
  return returnVal;
}
