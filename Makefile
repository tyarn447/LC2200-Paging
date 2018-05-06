Simulate: main.o Helper.o Machine.o Memory.o MyString.o MyStringHelper.o freeMemBlock.o PCB.o PageInfo.o ProcessPageInfo.o PageTable.o
	g++ main.o Helper.o Machine.o Memory.o MyString.o MyStringHelper.o freeMemBlock.o PCB.o PageInfo.o ProcessPageInfo.o PageTable.o -o Simulate
main.o: main.cc Helper.h Constants.h
	g++ -c main.cc
Helper.o: Helper.cc Helper.h Constants.h MyString.h Machine.h freeMemBlock.h  
	g++ -c Helper.cc
Machine.o: Machine.cc Machine.h Helper.h Constants.h Memory.h LinkedList.h freeMemBlock.h PCB.h Queue.h PageTable.h
	g++ -c Machine.cc
Memory.o: Memory.cc Memory.h Constants.h MyString.h
	g++ -c Memory.cc
MyString.o:MyString.cc MyString.h MyStringConstants.h MyStringHelper.h
	g++ -c MyString.cc
MyStringHelper.o:MyStringHelper.cc MyStringHelper.h MyStringConstants.h
	g++ -c MyStringHelper.cc
freeMemBlock.o: freeMemBlock.cc freeMemBlock.h Constants.h
	g++ -c freeMemBlock.cc
PCB.o: PCB.cc PCB.h MyString.h ProcessPageInfo.h LinkedList.h
	g++ -c PCB.cc
PageInfo.o: PageInfo.cc PageInfo.h Constants.h
	g++ -c PageInfo.cc
ProcessPageInfo.o: ProcessPageInfo.cc ProcessPageInfo.h Constants.h
	g++ -c ProcessPageInfo.cc
PageTable.o: PageTable.cc PageTable.h Constants.h PageInfo.h LinkedList.h Queue.h MyString.h
	g++ -c PageTable.cc
clean:
	rm *.o *~ Simulate
