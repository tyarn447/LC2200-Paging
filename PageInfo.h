#ifndef INCLUDED_PAGEINFO
#define INCLUDED_PAGEINFO
#include "Constants.h"
#include <iostream>
using namespace std;

class PageInfo{
  //CLASS INVARIANT:PPN>=0, VPN>=0, PID>=0 and PID
  //should be an existing PID in Machine class, dirtyBool is true if
  //memory in this page has been changed, false otherwise. dirtybool
  //will always be intialized to false
 private:
  uint PPN;//represents the Physical Page Number 
  uint VPN;//represents the virtual page number
  uint PID;//represents the PID that the virtual page number is from
  bool dirty;//represents if the page is dirty or not
  //for a page to be dirty, memory must have been changed on it
  bool stack;

 public:
  //base constructor
  PageInfo();

  //constructor
  //PRE:pPPN represents the physical page num, pVPN, represents the
  //virtual page num, pPid represents the PID the VPN is coming from
  //POST:creates the PageInfo object
  PageInfo(uint pPPN, uint pVPN, uint pPID);


  //constructor
  //PRE:pPPN represents the physical page num, pVPN, represents the
  //virtual page num, pPid represents the PID the VPN is coming from,
  //stack represents the boolean for stack variable
  //POST:creates the PageInfo object
  PageInfo(uint pPPN, uint pVPN, uint pPID,bool pStack);

  
  //copy constructor
  //PRE:copy constructor for PageInfo is called
  //POST:make an exact deep copy of the MyString object being called
  PageInfo(const PageInfo & pPageInfo);

  
  
  //PRE:object satisfies the CI
  //POST:returns the PPN
  uint getPPN();

  //PRE:object satisfies the CI
  //POST:returns the VPN
  uint getVPN();

  //PRE:object satisfies the CI
  //POST:returns the PID
  uint getPID();

  //PRE:object satisfies the CI
  //POST:if page is dirty, returns true, else returns false;
  bool isDirty();

  //PRE:object satisfies the CI
  //POST:makes the object dirty
  void makeDirty();

  //PRE:object satisfies the CI
  //POST:returns if the object is a stack pageInfo or not
  bool isStack();
  
  //PRE:object exists
  //POST:copies all info over from one PageInfo object to the other.
  PageInfo & operator = (const PageInfo & pPageInfo);
  
};
#endif
