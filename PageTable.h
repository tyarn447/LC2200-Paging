#ifndef INCLUDED_PAGETABLE
#define INCLUDED_PAGETABLE
#include "Constants.h"
#include "LinkedList.h"
#include "Queue.h"
#include "PageInfo.h"
#include "MyString.h"
#include <iostream>
using namespace std;

class PageTable{
  //Class Invariant:
 private:
  Queue <PageInfo *> usedPages;
  LinkedList<int> freePages;//should have same number of nodes as numFreepages
  uint numFreePages;//represents num free pages
  uint numUsedPages;//represents num used pages
 public:

  //base constructor
  PageTable();

  //constructor
  //PRE:
  //POST:creates object
  PageTable(uint numPages);
  
  //PRE:object satisfies the CI
  //POST:returns the number of freepages left
  uint getNumFreePages();

  //PRE:numFreePages>0 numNodes in freepages>0
  //POST:takes a page number from freepages and puts it in usedPages
  //with info in it
  uint addToUsedPages(uint VPN, uint PID);

  //PRE:numFreePages>0, parameters go inside object
  //POST:takes a page number from freepages and puts it in usedPages
  //with stack=true 
  uint stackAddToUsedPages(uint VPN, uint PID);

  //PRE:object satisfies the CI
  //POST:deletes a specific page from usedpages and adds it back to
  //freepages
  void deleteFront();

  //PRE:object satisies the CI
  //POST:deletes all objects in usedPages with a certain PID and adds
  //them back to freepages
  void deletePID(uint PID);

  //PRE:object satisfies the CI,numFreePages==0 
  //POST:returns a boolean, true if a nondirty page in usedpages was
  //deleted and false otherwise.Looks to delete from front to back, also
  //returns PID and VPN and if it was a stack or notby reference so
  //frameTable in PCB can be updated
  bool removeNonDirtyPage(uint & PID,uint & VPN, bool & stackBool);

  //PRE:object satisfies the CI, outString represents a MyString object
  //passed by reference 
  //POST:gives outString information about all pages in the table
  void giveOutString(MyString & outString);

  //PRE:object satisfies the CI, outString represents a MyString object
  //passed by reference 
  //POST:gives outString information about pages in the table whose PID
  //== PID
  void giveOutString(uint PID,MyString & outString);
  
  //PRE:object satisfies the CI and paging in machine == 1
  //POST:puts the object that has attributes equal to the parameters
  //given for PID, VPN and stackbool at the back of the queue.
  void LRU(uint PID, uint VPN, bool stackBool);

  //PRE:object is defined and satisfies the CI
  //POST:makes the page with the specified PID/VPN/StackBool a dirty
  //page
  void makeDirty(uint PID, uint VPN, bool stackBool);
  
  //destructor
  ~PageTable();
};

#endif
