#include <iostream>
#include <fstream>
#include "Constants.h"
#include "Helper.h"
using namespace std;
//PRE:There is one command line argument representing the number of
//words of memory our machine has
//Usage: ./simulator file.lc_config
//POST:simulates a machine, allows one to execute valid commands
int main(int argc, char * argv[]){
  if(argc != 2){
    cout << "Usage ./simulator file.lc_config"<<endl;
  }
  else{
    ifstream inputFile (argv[1]);
    if(inputFile != NULL){
      //ASSERT:input file is openable
      int numWords;
      int stackNum;
      int memManageType;
      int timeslice;
      int pagesize;
      int swapspace;
      int paging;
      getParameters(inputFile,numWords,stackNum,memManageType,timeslice,pagesize,swapspace,paging);
      //ASSERT;numwords/stack/memmanagetype/pagesize/swapspace/paging
      //contains the ints that are
      //specified in the inputFile
      if((numWords%pagesize == 0) && (stackNum%pagesize == 0) && (swapspace%pagesize ==0)){
	if(numWords > stackNum){
	  shell(numWords,stackNum,memManageType,timeslice,pagesize,swapspace,paging);
	}
	else{
	  cout<<"Number of words allocated to stack for each program is greater than the number of words for memory, this is an error in the config file"<<endl;
	}
      }
      else{
	cout<<"Memory or stackspace or swapspace is not a multiple of pagesize"<<endl;
      }
    }
  }
}
