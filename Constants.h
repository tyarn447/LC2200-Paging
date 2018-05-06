#ifndef INCLUDED_CONSTANTS
#define INCLUDED_CONSTANTS

#define uint unsigned int
#define NEGATIVE '-'
#define ASCIIZERO (int) '0'//AsciiZero used to turn int cast chars to ints 
#define EOS '\0'//End of string Character
#define BASE 10 //represents the base of the int, so we can do base 2
		//for binary, 10 for regular nums, ect
#define MAXINPUT 100 //represents the max number of chars able to be
		    //put in the input line
#define GIVEOUTSTRING 250
#define MAXCMDS 10//used for the array holding the parsed line
#define NEWLINE '\n'
#define SPACE ' '
#define SIX 6 //max length of the arrays the charexit, charload, ect will be held in
#define BITS 32
#define MINFREEPAGES 2
#define NUMLOADTOKENS 2//number of tokens allowed for load cmd
#define NUMMEMTOKENS 3//number of tokens allowed for mem cmd
#define NUMCPUTOKENS 1//number of tokens allowed for cpu cmd
#define NUMSTEPTOKENS 2//number of tokens allowed for step cmd
#define NUMRUNTOKENS 1//number of tokens allowed for run cmd
#define NUMKILLTOKENS 2 //number of tokens allowed for kill cmd
#define NUMFREEMEMTOKENS 1//number of tokens allowed for freemem cmd
#define NUMJOBTOKENS 1//number of tokens allowed for freemem cmd
#define NUMCONFIGTOKENS 1//number of tokens allowed for config cmd
#define NUMPAGESTOKENS 2
#define FOUR 4//should have named this bytes per word but o well, used for various various things
#define SHIFTMINUS 8//
#define SHIFT0 32//
#define SHIFT1 24//
#define SHIFT2 16//all used for shifting bits into the correct place to put them into memory
#define SHIFT3 8//
#define SHIFT4 0//
#define BLOCKSPERWORD 8//number of 4 bit blocks per word
#define ERROR -404//error code for a function to send back if something is incorrect
#define NUMREGISTERS 16//number of registers 
#define ITYPEVALUESHIFT 12//shift value for itype 
#define SPR 13//value of the stack pointer register in cpu registers array
#define BITSPERBYTE 8//number of bits per byte
#define STACKPOINTER 13//number of stack pointer in registers;
#define BITMASKSTART 0x000000FF
static uint opcodeMask = 0xF0000000;//
static uint reg1Mask = 0x0F000000;//
static uint reg2Mask = 0x00F00000;//all used to as masks for certain purposes
static uint valueMask = 0x000FFFFF;//
static uint reg3Mask = 0x0000000F;//
static uint isNegativeMask = 0x00080000;//
static uint makeNegativeMask = 0xFFF00000;//
#define OPCODESHIFT 28//shift to get opcode
#define REG1SHIFT 24//shift to get register 1
#define REG2SHIFT 20//shift to get register 2
enum opcodeType {ADDCODE,NANDCODE,ADDICODE,LWCODE,SWCODE,BEQCODE,JALRCODE,
		 HALTCODE,DUMMY8CODE,INCODE,OUTCODE,DUMMY11CODE,LACODE,BGTCODE};//opcode types

static char regNames[16][6] = {"$zero","$at","v0","a0","a1","a2","t0",
			       "t1","t2","s0","s1","s2","k0","sp","fp","ra"};//names o registers

//NAMES OF COMMANDS TO SHELL
static char CHAREXIT[SIX] = "exit";//will be used to see if input is this
static char CHARLOAD[SIX] = "load";//will be used to see if input is this
static char CHARMEM[SIX] = "mem";//will be used to see if input is this
static char CHARCPU[SIX] = "cpu";//will be used to see if input is this
static char CHARSTEP[SIX] =  "step";//will be used to see if input is this
static char CHARRUN[SIX] = "run";//will be used to see if input is this
static char CHARMEMORY[BASE] = "memory";//will be used with lc_config file
static char CHARSTACK[BASE] = "stack";//will be used with lc_config file
static char CHARMEMMANAGEMENT[BITS] = "mem-management";//will be used with lc_config file
static char CHARTIMESLICE[BASE] = "timeslice";//will be used with lc_config file
static char CHARPAGESIZE[BASE] = "pagesize";//will be used with lc_config file
static char CHARSWAPSPACE[BASE] = "swapspace";//will be used with lc_config file
static char CHARPAGING[BASE] = "paging";//will be used with lc_config file
static char CHARKILL[SIX] = "kill";//will be used to see if input is this
static char CHARJOBS[SIX] = "jobs";//will be used to see if input is this
static char CHARFREEMEM[BASE] = "freemem"; //will be used to see if input is this
static char CHARCONFIG[BASE] = "config";//will be used to see if input is this
static char CHARPAGES[BASE] = "pages";//will be used to see if input is this
#endif
