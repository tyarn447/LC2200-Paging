#include "PageTable.h"
#include "Constants.h"
#include "PageInfo.h"
#include <iostream>
#include "LinkedList.h"
#include "MyString.h"
#include "Queue.h"
#include <stdio.h>
using namespace std;


//base constructor
PageTable::PageTable(){}

//constructor
//PRE:
//POST:creates object
PageTable::PageTable(uint numPages){
  numFreePages = numPages;
  numUsedPages = 0;
  for(uint index = 0; index < numPages; index++){
    freePages.addToBack(index);//intialze freepages queue
  }
}
  
//PRE:object satisfies the CI
//POST:returns the number of freepages left
uint PageTable::getNumFreePages(){
  return freePages.getNumNodes();
}




//PRE:numFreePages>0 numNodes in freepages>0
//POST:takes a page number from freepages and puts it in usedPages
//with info in it,returns the PPN that is being used to load program
//in
uint PageTable::addToUsedPages(uint VPN, uint PID){
  int newUsedPPN = freePages.getNth(0);
  freePages.deleteNth(0);
  numFreePages --;
  //now we have gotten the PPN from front of freepages and removed it
  PageInfo * newPage = new PageInfo(newUsedPPN, VPN, PID);//create new pageinfo
  usedPages.addToBack(newPage);//add new pageinfo to back
  numUsedPages ++;//iterate numUsedpages
  return newUsedPPN;
}

//PRE:numFreePages>0, parameters go inside object
//POST:takes a page number from freepages and puts it in usedPages
//with stack=true, returns the PPN used 
uint PageTable::stackAddToUsedPages(uint VPN, uint PID){
  int newUsedPPN = freePages.getNth(0);
  freePages.deleteNth(0);
  numFreePages --;
  //now have gotten PPN from front of freepages
  PageInfo * newPage = new PageInfo(newUsedPPN,VPN,PID,true);
  usedPages.addToBack(newPage);
  numUsedPages ++;
  return newUsedPPN;
}

//PRE:object satisfies the CI
//POST:deletes the page at the front of the queue 
void PageTable::deleteFront(){
  PageInfo * front = usedPages.getNth(0);
  uint addPPN = front->getPPN();
  freePages.addToBack(addPPN);//adds PPN from front back
  numFreePages ++;
  delete front;//delete heap thing
  usedPages.deQueue();//deletes front element
  numUsedPages --;
}

//PRE:object satisies the CI
//POST:deletes all objects in usedPages with a certain PID and adds
//them back to freepages
void PageTable::deletePID(uint PID){
  //dont forget to do this for swap area as well later..
  uint len = usedPages.getNumNodes();
  for(int index = 0; index < len; index++){
    PageInfo * thisPage = usedPages.getNth(index);
    uint posPID = thisPage->getPID();
    if(posPID == PID){
      //ASSERT:found a PID that should be deleted
      uint addPPN = thisPage->getPPN();
      freePages.addToBack(addPPN);
      numFreePages++;
      usedPages.deleteNth(index);
      index --;//cause the next node will fall into this index place
      len --;
      //so make sure index doesnt go one more up
      numUsedPages --;
      delete thisPage;//delete heap thing
    }
  }
}

//PRE:object satisfies the CI,numFreePages==0 
//POST:returns a boolean, true if a nondirty page in usedpages was
//deleted and false otherwise.Looks to delete from front to back, also
//returns PID and VPN and if it was a stack or notby reference so
//frameTable in PCB can be updated
bool PageTable::removeNonDirtyPage(uint & PID,uint & VPN, bool & stackBool){
  bool rv = false;
  uint len = usedPages.getNumNodes();
  uint index = 0;
  while((!rv) && (index < len)){
    //ASSERT:have not removed a page nor have we gone out of bounds
    PageInfo * thisPage = usedPages.getNth(index);
    if(thisPage->isDirty()){
    }
    if(!thisPage->isDirty()){
      //ASSERT:thisPage is not dirty
      PID = thisPage->getPID();
      VPN = thisPage->getVPN();
      stackBool = thisPage->isStack();
      uint addPPN = thisPage->getPPN();
      usedPages.deleteNth(index);//delete page
      delete thisPage;
      rv = true;//to break loop
      numUsedPages --;
      freePages.addToBack(addPPN);
      numFreePages ++;//add PPN back
    }
    else{
      index ++;
    }
  }
  return rv;
}


//PRE:object satisfies the CI, outString represents a MyString object
//passed by reference 
//POST:gives outString information about all pages in the table
void PageTable::giveOutString(MyString & outString){
  uint len = usedPages.getNumNodes();
  for(int index = 0; index < len; index++){
    char array[GIVEOUTSTRING];
    PageInfo * thisPage = usedPages.getNth(index);
    uint PPN = thisPage->getPPN();
    uint VPN = thisPage->getVPN();
    uint PID = thisPage->getPID();
    bool stack = thisPage->isStack();
    sprintf(array,"PPN:%i, VPN:%i, PID%i ",PPN,VPN,PID);
    outString = outString + array;
    if(stack){
      //ASSERT:this page is a stack
      outString = outString + " Stack";
    }
    outString = outString + "\n";
  }
  //swap area stuff
}


//PRE:object satisfies the CI, outString represents a MyString object
//passed by reference 
//POST:gives outString information about pages in the table whose PID
//== PID
void PageTable::giveOutString(uint PID,MyString & outString){
  uint len = usedPages.getNumNodes();
  for(int index = 0; index < len; index++){
    char array[GIVEOUTSTRING];
    PageInfo * thisPage = usedPages.getNth(index);
    uint PPN = thisPage->getPPN();
    uint VPN = thisPage->getVPN();
    uint thisPID = thisPage->getPID();
    bool stack = thisPage->isStack();
    if(thisPID == PID){
      sprintf(array,"PPN:%i, VPN:%i, PID%i ",PPN,VPN,PID);
      outString = outString + array;
      if(stack){
	//ASSERT:this page is a stack
	outString = outString + " Stack";
      }
      outString = outString + "\n";
    }
  }
  //swap area stuff
}


//PRE:object satisfies the CI and paging in machine == 1
//POST:puts the object that has attributes equal to the parameters
//given for PID, VPN and stackbool at the back of the queue.
void PageTable::LRU(uint PID, uint VPN, bool stackBool){
  uint len = usedPages.getNumNodes();
  bool found = false;
  uint index = 0;
  while((index < len) && (!found)){
    //ASSERT have not found node or gone out of bounds
    PageInfo * thisPage = usedPages.getNth(index);
    uint posPID = thisPage->getPID();
    uint posVPN = thisPage->getVPN();
    bool posStackBool = thisPage->isStack();
    if((posPID == PID) && (posVPN == VPN) && posStackBool == (stackBool)){
      //ASSERT:we have found the correct PageInfo object
      found = true;
      usedPages.addToBack(thisPage);
      usedPages.deleteNth(index);
      //delete from list, while retaining stack object
     
      //add to back
      
    }
    index ++;
  }
}


//PRE:object is defined and satisfies the CI
//POST:makes the page with the specified PID/VPN/StackBool a dirty
//page
void PageTable::makeDirty(uint PID, uint VPN, bool stackBool){
  uint len = usedPages.getNumNodes();
  bool found = false;
  uint index = 0;
  while((index < len) && (!found)){
    PageInfo * thisPage = usedPages.getNth(index);
    uint posPID = thisPage->getPID();
    uint posVPN = thisPage->getVPN();
    uint posStackBool = thisPage->isStack();
    if((posPID == PID) && (posVPN == VPN) && (posStackBool == stackBool)){
      //ASSERT:we have found correct page
      found = true;
      thisPage->makeDirty();
    }
    index ++;
  }
}



//destructor
PageTable::~PageTable(){
  uint len = usedPages.getNumNodes();
  for(int index = 0; index < len; index ++){
    PageInfo * deleteThis = usedPages.getNth(0);
    delete deleteThis;//delete heap thing
    usedPages.deleteNth(0);
  }//Everything on heap from usedPages should be gone
}
  
