#include "MyString.h"
#include "MyStringConstants.h"
#include "MyStringHelper.h"
#include <iostream>
using namespace std;

//START CONSTRUCTORS/DESTRUCTOR
//*************************************************************************************************
//*************************************************************************************************



//PRE:MyString is called
//POST:constructs a Mystring object of lenstring=0, lenArray=1, and a new array on the heap of size lenArray.
MyString::MyString(){
  lenString = 0;
  lenArray = 1;
  stringArray = new char[lenArray];
  stringArray[lenArray - 1] = EOS;
}



//PRE:MyString is called
//POST:creates a Mystring object with the charArray as its array and corresponding string and array lengths
MyString::MyString(char * charArray){
  

  lenString = 0;
  char inChar;
  inChar = charArray[lenString];
  
  while(inChar != EOS){
    lenString++;
    inChar = charArray[lenString];
  }//gets lenstring so you dont have to go through addchar
  lenArray = lenString + 1; 
  stringArray = new char[lenArray];//makes new array on heap of correct length
  stringArray[lenString] = EOS;//last char is EOS always
  copyOver(stringArray, charArray, lenArray);//copies the chararray into the stringarray member data
}

  

//PRE:copy Constructor for MyString is called
//POST:makes an exact deep copy of the MyString object that is being called.
MyString::MyString(const MyString & pMyString){
  lenString = pMyString.lenString;//copy over int value
  lenArray = pMyString.lenArray; //copy over int value
  stringArray = new char[lenString + 1];//make new array on heap
  copyOver(stringArray,pMyString.stringArray,pMyString.lenString);//copy over array completely
  stringArray[pMyString.lenString] = EOS;
}



//destructor
//PRE:MyString object goes out of scope
//POST:deletes the array on the heap that MyString object has access too.
MyString::~MyString(){
  delete [] stringArray;//deletes the array on the heap that stringArray is pointing too
}

//END CONSTRUCTORS/DESTRUCTOR
//*************************************************************************************************
//*************************************************************************************************






//START OVERLOADED BOOLEANS
//*************************************************************************************************
//*************************************************************************************************

//PRE:this object holds a char array, pMyString also holds a char array
//POST:loops through the char arrays if they have the same length and returns true if they are the same false if they are not
bool MyString::operator == (const MyString & pMyString) const{
  bool returnVal = true;//initialze as true so if it finds a false it can immediately return false instead of going through whole array
  if(lenString == pMyString.lenString){//can only be true lengths of string are the same.
    int index = 0;
    while((returnVal) && (index < lenString)){//loop through string
      if(stringArray[index] != pMyString.stringArray[index]){//if char in same pos is not the same return false
	returnVal = false;
      }
      index ++;
    }
  }
  else{
    returnVal = false;
  }
  return returnVal;
}

//NOTE:pMyString is being passed by reference but is not being changed
//PRE:both objects are defined myStrings AND their arrayStrings do not equal each other.
//POST:compares the chars in each chararray in the object and returns true if the implicit parameter coems first in the dictionary based on dictionary ordering, false otherwise.
bool MyString::operator > (const MyString & pMyString) const{
  //if a char is "placed first in dictionary" it will be smaller and vice versa. ie 'a' < ' b' is where it returns true so a comes first in dictionary.
  //ab comes in dictionary before abc
  bool returnVal = true;
  int lowerSize = returnLower(lenString, pMyString.lenString);//get the lowersize of the two strings, this way if they are the same but one is longer than the other the longer will be lower in dict
  int sameLetter = 0;
  int index = 0;
  while((returnVal) && (index < lowerSize)){//only loop through array while we havent found which one is higher in dict
    if(stringArray[index] == pMyString.stringArray[index]){
      sameLetter++;
      index++;
    }
    else if(stringArray[index] > pMyString.stringArray[index]){//if char in implicit is larger than char in explicit it means it is lower in dictionary
      returnVal = false;
    }//if returnVal does not change that means it is the same up to this point which means the smaller of the two will be first in the dictionary
    else{
      index++;
    }
  }//ASSERT:the are the same up to this point but one is longer than the other.
  if(sameLetter == lowerSize){
    if(lenString > pMyString.lenString){
      returnVal = false; //ASSERT:lenString is longer than pMySTring.lenString so it must come lower in the dictionary. so the returnVal is false.
    }
  }
  return returnVal;
}



//NOTE:pMyString is being passed by reference but is not being changed
//PRE:both objects are defined myStrings  AND their arrayStrings do not equal each other.
//POST:compares the chars in each chararray in the object and returns true if the implicit parameter comes last in the dictionary based on dictionary ordering, false otherwise.
bool MyString::operator < (const MyString & pMyString) const{
  bool returnVal = true;
  int lowerSize = returnLower(lenString, pMyString.lenString);
  int sameLetter = 0;
  int index = 0;
  while((returnVal) && (index < lowerSize)){//get the lowersize of the two strings, this way if they are the same but one is longer than the other the longer will be lower in dict
    if(stringArray[index] == pMyString.stringArray[index]){
      sameLetter ++;
      index++;
    }
    else if(stringArray[index] < pMyString.stringArray[index]){ //if char in implicit is less than char in explicit it means that it is higher in dictionary which is bad so make return val false
      returnVal = false;
    }//if returnVal does not change that means it is the same up to this point which means the smaller of the two will be first in the dictionary
    else{
      index++;
    }
  }//ASSERT:the are the same up to this point but one is longer than the other.
  if(sameLetter == lowerSize){
    if(lenString < pMyString.lenString){
      returnVal = false; //ASSERT:lenstring is less than pMyString.lenString so it must come higher in dictionary which is bad so returnVal is false.
    }
  }
  return returnVal;
}


//END OVERLOADED BOOLEANS
//*************************************************************************************************
//*************************************************************************************************




//START OVERLOADED + OPERATORS
//*************************************************************************************************
//*************************************************************************************************


//+operator that takes a myString
//NOTE:pMyString does not change even though it is passed by reference.
//PRE:Mystring is a defined object as well as pMyString
//POST:initialzes new MyString object and adds the values/arrays from the two added MyStrings into it
MyString MyString::operator + (const MyString & pMyString)const{//****NOTE IF YOU WANT TOO AND UR NOT TOO LAZY CHANGE THIS TO MAKE A ARRAY OF LENSTRING+LENSTRING AND JUST COPY OVER?
  MyString temp;
  char inChar;
  temp = *this;
  /*
  for(int index = 0; index < lenString; index ++){//goes to lenString so it doesnt hit EOS
    inChar = stringArray[index];
    temp.addChar(inChar);//add the first myStrings chars
  }
  */
  for(int index2 = 0; index2 < pMyString.lenString; index2++){//goes to lenstring so it doesnt hit EOS
    inChar = pMyString.stringArray[index2];
    temp.addChar(inChar);
  }
  temp.addChar(EOS);//put in EOS so addChar adds an EOS to end of string.
  return temp;
}
//NOTE:charArray is passed by reference but it will not be changed
//PRE:MyString is a defined object and charArray is a defined array of chars
//POST:returns a MyString that has added the two char arrays together
MyString MyString::operator + (const char * charArray)const{
  MyString temp;
  char inChar;
  temp = *this;
  int index2 = 0;
  inChar = charArray[index2];
  while(inChar != EOS){
    temp.addChar(inChar);
    index2++;
    inChar = charArray[index2];
  }
  temp.addChar(EOS);
  return temp;

}
//END OVERLOADED + OPERATORS
//*************************************************************************************************
//*************************************************************************************************



//START OVERLOADED ASSIGNMENT OPERATORS
//*************************************************************************************************
//*************************************************************************************************



//PRE:this object holds a char array, pMyString also holds a char array
//POST:makes a deep copy of pMyString into the implicit parameter.
MyString & MyString::operator = (const MyString & pMyString){
  if(&pMyString != this){
    lenString = pMyString.lenString;//copy over int value 
    lenArray = pMyString.lenString + 1; //copy over int value 
    delete [] stringArray; //delete array on heap pointer is pointing too
    stringArray = new char[lenString + 1];
    copyOver(stringArray,pMyString.stringArray,pMyString.lenString);//copy over chars
    stringArray[lenString] = EOS; 
  }
  return(*this);
}



//NOTE:charArray is passed by reference but will not be changed
//PRE:this object holds a char array, this char array will turn into the char array that is the parameter
//POST: takes the char array it is given and adds the chars from it into its own char array that is memberdata
MyString & MyString::operator = (const char * charArray){
  char inChar;
  int index = 0;
  lenString = 0;// reconstructing object just incase it already has stuff in array
  lenArray = 1;//reconstructing object just incase it already has stuff in array
  delete [] stringArray;//delete original array on help so you can create a fresh one to addchars too.
  stringArray = new char[lenArray];// reconstructing object just incase it already has stuff in array
  stringArray[lenArray - 1] = EOS;// reconstructing object just incase it already has stuff in array
  inChar = charArray[index];//get first char to see if it is just an empty string so the while loop doesnt go if it is an EOS.
  
  while(inChar != EOS){
    addChar(inChar);
    index++;
    inChar = charArray[index];
  }
  return(*this);
}

//END OVERLOADED ASSIGNMENT OPERATORS
//*************************************************************************************************
//*************************************************************************************************




//START OVERLOADED INPUT/OUTPUT OPERATORS
//*************************************************************************************************
//*************************************************************************************************


//NOTE:pMyString is being passed by reference but is not being changed
//PRE:pMyString is a defined object
//POST:prints out the array of pMyString 
ostream & operator << (ostream & stream, const MyString & pMyString){
  stream << pMyString.stringArray;
  return (stream);
}
//do I need to declare this as friend in .cc? idk

//PRE:pMyString is a defined object
//POST:takes input to the char array of pMyString
//dont use const MyString because we are changing it
istream & operator >> (istream & stream, MyString & pMyString){
  char inChar = stream.get();
  while(inChar != NEWLINE){
    pMyString.addChar(inChar);
    inChar = stream.get();
  }
  return (stream);

}

//END OVERLOADED INPUT/OUTPUT OPERATORS
//*************************************************************************************************
//*************************************************************************************************





//START MEMBER FUNCTIONS
//*************************************************************************************************
//*************************************************************************************************

//PRE:takes a character
//POST:adds character to myString array.
void MyString::addChar(char myChar){
  
  if(myChar != EOS){
    
    if(lenString == lenArray - 1){//ASSERT:if this is true then lenArray must be doubled as and make new stringArray and copy old one over.
      int oldArrayLen = lenArray;
      lenArray *= TWO;//double lenArray
      //make temp new array of double lenArray and copy old one into it and put EOS at lenArray - 1 and delete old array.
      char * temp = new char[lenArray];
      
      copyOver(temp,stringArray,lenArray);
      delete [] stringArray;//ASSERT:array on heap has been deallocated
      stringArray = temp;
      stringArray[lenArray - 1] = EOS;
      stringArray[lenString] = myChar;
      lenString ++;//iterate lenString now.
    }
    else{
      stringArray[lenString] = myChar;//stringArray isnt too small so you can just put next char in next spot
      lenString++;
    }
  }
  else{
    stringArray[lenString] = EOS;//if it is EOF then you must put EOS at this spot cause there may be empty spots after it.
  }
}



//PRE:MyString has a defined char array
//POST:returns a pointer to a deep copy of the array from MyString
char * MyString::cstr() const {
  char * returnVal;
  returnVal = new char[lenString + 1];
  copyOver(returnVal,stringArray,lenString);//use lenString instead of lenArray cause made a new more optimal smaller array based on lenString.
  returnVal[lenString] = EOS;
  return returnVal;
}



//PRE:none
//POST:returns a deep copy of a new pointer to a character array that is the reverse of the char array in MyString object
char * MyString::getReverse()const{
  char * returnVal;
  returnVal = new char[lenString + 1];
  int startVal = lenString;
  returnVal[startVal] = EOS;//make last char EOS automatically cause when you do assigment op it will while loop till it hits EOS
  startVal --;
  for(int index = 0; index < lenString; index++){
    returnVal[startVal] = stringArray[index];
    startVal--;
  }
  return returnVal;
}


//END MEMBER FUNCTIONS
//*************************************************************************************************
//*************************************************************************************************
