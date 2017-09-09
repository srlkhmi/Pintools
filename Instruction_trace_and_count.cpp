#include <iostream>
#include <fstream>
#include <stdio.h>
#include "pin.H"


std::map <ADDRINT, std::string> instructions;
static UINT64 ins_count = 0;
std::ofstream TraceFile;
std::ofstream InsCountFile;

VOID instruction_count() 
{ 
	ins_count++; 
}


void Instruction_trace(INS ins, VOID *v)
{
	ADDRINT insaddr=INS_Address(ins);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)instruction_count, IARG_END);
	std::string disass(INS_Disassemble(ins));
	TraceFile<<insaddr<<" "<<disass<<endl;
}

VOID Fini(INT32 code, VOID *v)
{
    
	InsCountFile<< "Instruction Count  " << ins_count << endl;
    TraceFile.close();
	InsCountFile.close();
}

INT32 Usage()
{
    
    return -1;
}

int main(int argc, char * argv[])
{
    TraceFile.open("ins_trace.out");
    TraceFile<<hex;
    TraceFile.setf(ios::showbase);   

	InsCountFile.open("ins_count.out");
    if (PIN_Init(argc, argv)) return Usage();

   
    INS_AddInstrumentFunction(Instruction_trace, 0);

   
    PIN_AddFiniFunction(Fini, 0);
    
   
    PIN_StartProgram();
    
    return 0;
}
