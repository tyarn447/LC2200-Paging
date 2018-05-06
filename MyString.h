#ifndef INCLUDED_MYSTRING
#define INCLUDED_MYSTRING
#include <iostream>
using namespace std;

class MyString{                      //CI: lenArray > 0
                                     //   0 <=lenString < lenArray 
 private:                            //   stringArray[0] - stringArray[n-1] is defined
                                     // stringArray[n] = EOS;
  int lenString;
  int lenArray;
  char * stringArray;
 
  
 public:

  
  //START CONSTRUCTORS/DESTRUCTOR
  //************************************************************************************************
  //************************************************************************************************
  
  //default constructor
  //PRE:MyString is called
  //POST:constructs a Mystring object of lenstring=0, lenArray=1, and a new array on the heap of size lenArray.
  MyString();
  

  //constructor for when you want to use a string to construct
  //PRE:MyString is called
  //POST:creates a Mystring object with the charArray as its array and corresponding string and array lengths
  MyString(char * charArray);

  
  //destructor
  //PRE:MyString object goes out of scope
  //POST:deletes the array on the heap that MyString object has access too.
  ~MyString();

  //copy constructor
  //PRE:copy Constructor for MyString is called
  //POST:makes an exact deep copy of the MyString object that is being called.
  MyString(const MyString & pMyString);
  
  //END CONSTRUCTORS/DESTRUCTOR
  //************************************************************************************************
  //************************************************************************************************
  


  //START OVERLOADED ASSIGNMENT OPERATORS
  //************************************************************************************************
  //************************************************************************************************
  

  //+operator that takes a myString
  //NOTE:pMyString does not change even though it is passed by reference.
  //PRE:Mystring is a defined object as well as pMyString
  //POST:initialzes new MyString object and adds the values/arrays from the two added MyStrings into it
  MyString operator + (const MyString & pMyString)const;
  
  

  

//NOTE:charArray is passed by reference but it will not be changed
//PRE:MyString is a defined object and charArray is a defined array of chars
//POST:returns a MyString that has added the two char arrays together
  MyString operator + (const char * charArray)const;


  //END OVERLOADED + OPERATORS
  //************************************************************************************************
  //************************************************************************************************
  

  

  //START OVERLOADED BOOLEAN OPERATORS
  //************************************************************************************************
  //************************************************************************************************
  

  //NOTES:pMyString is passed by reference but will not be changed
  //PRE:this object holds a char array, pMyString also holds a char array
  //POST:loops through the char arrays if they have the same length and returns true if they are the same false if they are not
  bool operator == (const MyString & pMyString) const;


  
  //NOTE:pMyString is being passed by reference but is not being changed
  //PRE:both objects are defined myStrings
  //POST:compares the chars in each chararray in the object and returns true if the implicit parameter is larger based on dictionary ordering, false otherwise.
  bool operator > (const MyString & pMyString) const;


  //NOTE:pMyString is being passed by reference but is not being changed
  //PRE:both objects are defined myStrings
  //POST:compares the chars in each chararray in the object and returns true if the implicit parameter is smaller based on dictionary ordering, false otherwise.
  bool operator < (const MyString & pMyString)const;


  //END OVERLOADED BOOLEAN OPERATORS
  //************************************************************************************************
  //************************************************************************************************
  

  


  //START OVERLOADED ASSIGNMENT OPERATORS
  //************************************************************************************************
  //************************************************************************************************
  
  
  //NOTE:pMyString is passed by reference but will not be changed
  //PRE:this object holds a char array, pMyString also holds a char array
  //POST:assigns all member data in this object to the pMyString object
  MyString & operator = (const MyString & pMyString);


  //NOTE:charArray is passed by reference but will not be changed
  //PRE:this object holds a char array, this char array will turn into the char array that is the parameter
  //POST: takes the char array it is given and adds the chars from it into its own char array that is memberdata
  MyString & operator = (const char * charArray);
  
  //END OVERLOADED ASSIGNMENT OPERATORS
  //************************************************************************************************
  //************************************************************************************************
  


  

  //START OVERLOADED INPUT/OUTPUT OPERATORS
  //************************************************************************************************
  //************************************************************************************************
  
  //need cout op << thing
  //NOTE:pMyString is being passed by reference but is not being changed
  //PRE:pMyString is a defined object
  //POST:prints out the array of pMyString 
  friend ostream & operator << (ostream & stream, const MyString & pMyString);


  //NOTE:pMyString is being passed by reference but is not being changed
  //PRE:pMyString is a defined object
  //POST:takes input to the char array of pMyString
  friend istream & operator >> (istream & stream, MyString & pMyString);
  //do I need to declare this as friend in .cc? idk


  //END OVERLOADED INPUT/OUTPUT OPERATORS
  //************************************************************************************************
  //************************************************************************************************
  

  

  //START MEMBER FUNCTIONS
  //************************************************************************************************
  //************************************************************************************************
  
  //PRE:takes a character
  //POST:adds character to myString array.
  void addChar(char myChar);

  
  //PRE:MyString has a defined char array
  //POST:returns a pointer to a deep copy of the array from MyString
  char * cstr() const;

  //PRE:none
  //POST:returns a deep copy of a new pointer to a character array that has been reversed.
  char * getReverse()const;


  //END MEMBER FUNCTIONS
  //************************************************************************************************
  //************************************************************************************************
  
  
};
#endif
