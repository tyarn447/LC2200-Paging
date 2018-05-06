#ifndef INCLUDED_MYSTRINGHELPER
#define INCLUDED_MYSTRINGHELPER

//PRE:both newArray and oldArray are arrays that have been created on the heap, old array is full of chars while newArray is garbage except for last value which is EOS, oldArrayLen is an int representing the length of the old array. This can only be used when a char is being added to myString and the array isnt big enough to hold it. newArray is twice the size of oldArray.
//POST:copies oldArray into newArray;
void copyOver(char * newArray, char * oldArray, int lenOldArray);



//PRE:first and second are ints
//POST:returns the lower of the two ints.
int returnLower(int first, int second);


#endif
