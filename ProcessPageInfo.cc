#include "ProcessPageInfo.h"
#include "Constants.h"
#include <iostream>
using namespace std;

//base constructor
ProcessPageInfo::ProcessPageInfo(){}

//Constructor
//PRE:
//POST:creates object
ProcessPageInfo::ProcessPageInfo(uint pVPN){
  VPN = pVPN;
  inMem = false;
  inSwap = false;
}

// //copy constructor
// //PRE:copy constructor for ProcessPageInfo is called
// //POST:creates deep copy of object being called
// ProcessPageInfo::ProcessPageInfo(const ProcessPageInfo & pProcessPageInfo){
//   VPN = pProcessPageInfo.VPN;//copy over values
//   inMem = pProcessPageInfo.inMem;
//   inSwap = pProcessPageInfo.inSwap;
//   PPN = pProcessPageInfo.PPN;
//   swapLoc = pProcessPageInfo.swapLoc;
// }

// //PRE:object exists
// //POST:copies all info over from one ProcessPageInfo object to the other
// ProcessPageInfo & ProcessPageInfo::operator = (const ProcessPageInfo & pProcessPageInfo){
//   VPN = pProcessPageInfo.VPN;//copy over values
//   inMem = pProcessPageInfo.inMem;
//   inSwap = pProcessPageInfo.inSwap;
//   PPN = pProcessPageInfo.PPN;
//   swapLoc = pProcessPageInfo.swapLoc;
//   return(*this);
// }

//PRE:object satisfies the CI
//POST:changes the inMem bool to parameter
void ProcessPageInfo::changeInMem(bool mem){
  inMem = mem;
}

//PRE:object satisfies the CI
//POST:changes the inSwap bool to parameter
void ProcessPageInfo::changeInSwap(bool swap){
  inSwap = swap;
}


//PRE:object exists and satisfies the CI
//POST:returns object VPN
uint ProcessPageInfo::getVPN(){
  return VPN;
}

//PRE:object satisfies the CI
//POST:returns if the object is in memory or not
bool ProcessPageInfo::inMemory(){
  return inMem;
}

//PRE:object satisfies the CI
//POST:returns if the object is in swapspace or not
bool ProcessPageInfo::inSwapSpace(){
  return inSwap;
}

//PRE:object exists and satisfies the CI, and is in memory
//POST:returns the VPN physical location in memory
uint ProcessPageInfo::getPPN(){
  return PPN;
}

//PRE:object satisfies the CI and is in swap space
//POST:returns the VPN location in swap space
uint ProcessPageInfo::getSwapLoc(){
  return swapLoc;
}



//PRE:object satisfies the CI
//POST:makes the PPN = pPPN
void ProcessPageInfo::addPPN(uint pPPN){
  PPN = pPPN;
}
