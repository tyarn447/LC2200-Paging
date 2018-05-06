#include "PageInfo.h"
#include "Constants.h"
#include <iostream>
using namespace std;

//base constructor
PageInfo::PageInfo(){}

//constructor
//PRE:pPPN represents the physical page num, pVPN, represents the
//virtual page num, pPid represents the PID the VPN is coming from
//POST:creates the PageInfo object
PageInfo::PageInfo(uint pPPN, uint pVPN, uint pPID){
  PPN = pPPN;
  VPN = pVPN;
  PID = pPID;
  dirty = false;//always initialze to false;
  stack = false;
}
//constructor
//PRE:pPPN represents the physical page num, pVPN, represents the
//virtual page num, pPid represents the PID the VPN is coming from,
//stack represents the boolean for stack variable
//POST:creates the PageInfo object
PageInfo::PageInfo(uint pPPN, uint pVPN, uint pPID,bool pStack){
  PPN = pPPN;
  VPN = pVPN;
  PID = pPID;
  dirty = false;
  stack = pStack;
}


//copy constructor
//PRE:copy constructor for PageInfo is called
//POST:make an exact deep copy of the MyString object being called
PageInfo::PageInfo(const PageInfo & pPageInfo){
  PPN = pPageInfo.PPN;//copy of PPN value
  VPN = pPageInfo.VPN;//copy over VPN value
  PID = pPageInfo.PID;//copy over PID value
  dirty = pPageInfo.dirty;//copy over dirtybool
}

//PRE:object satisfies the CI
//POST:returns the PPN
uint PageInfo::getPPN(){
  return PPN;
}

//PRE:object satisfies the CI
//POST:returns the VPN
uint PageInfo::getVPN(){
  return VPN;
}

//PRE:object satisfies the CI
//POST:returns the PID
uint PageInfo::getPID(){
  return PID;
}

//PRE:object satisfies the CI
//POST:if page is dirty, returns true, else returns false;
bool PageInfo::isDirty(){
  return dirty;
}

//PRE:object satisfies the CI
//POST:makes the object dirty
void PageInfo::makeDirty(){
  dirty = true;
}

//PRE:object satisfies the CI
//POST:returns if the object is a stack pageInfo or not
bool PageInfo::isStack(){
  return stack;
}

//PRE:object exists
//POST:copies all info over from one PageInfo object to the other.
PageInfo & PageInfo::operator = (const PageInfo & pPageInfo){
  PPN = pPageInfo.PPN;//copy of PPN value
  VPN = pPageInfo.VPN;//copy over VPN value
  PID = pPageInfo.PID;//copy over PID value
  dirty = pPageInfo.dirty;//copy over dirtybool
  return(*this);
}
