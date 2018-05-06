#ifndef INCLUDED_PROCESSPAGEINFO
#define INCLUDED_PROCESSPAGEINFO
#include "Constants.h"

class ProcessPageInfo{
  //Class Invariant:VPN>=0

 private:
  uint VPN;
  bool inMem;
  bool inSwap;
  uint PPN;
  uint swapLoc;
  
  
 public:
  
  //base constructor
  ProcessPageInfo();

  //Constructor
  //PRE:
  //POST:creates object
  ProcessPageInfo(uint pVPN);

  //copy constructor
  //PRE:copy constructor for ProcessPageInfo is called
  //POST:creates deep copy of object being called
  ProcessPageInfo(const ProcessPageInfo & pProcessPageInfo);

  //PRE:object exists
  //POST:copies all info over from one ProcessPageInfo object to the other
  ProcessPageInfo & operator = (const ProcessPageInfo & pProcessPageInfo);

  //PRE:object satisfies the CI
  //POST:changes the inMem bool to parameter
  void changeInMem(bool mem);

  //PRE:object satisfies the CI
  //POST:changes the inSwap bool to parameter
  void changeInSwap(bool swap);
  
  //PRE:object exists and satisfies the CI
  //POST:returns object VPN
  uint getVPN();

  //PRE:object satisfies the CI
  //POST:returns if the object is in memory or not
  bool inMemory();

  //PRE:object satisfies the CI
  //POST:returns if the object is in swapspace or not
  bool inSwapSpace();

  //PRE:object exists and satisfies the CI, and is in memory
  //POST:returns the VPN physical location in memory
  uint getPPN();

  //PRE:object satisfies the CI and is in swap space
  //POST:returns the VPN location in swap space
  uint getSwapLoc();

  //PRE:object satisfies the CI
  //POST:makes the PPN = pPPN
  void addPPN(uint pPPN);
};


#endif
