/*
Yousef Jarrar, Nicholas Chiodini
CSE 460 Dr. Z
Due Date: October 15th, 2018
Description: This is the header file for the Assembler Program
The header file contains a Assembler class that will define the assembler and
the opCodes to go with it. We need to watch for predefined functions like
read, return write. As they are used in C++
*/
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

using namespace std;

class NullPointerException: public runtime_error {
public:
NullPointerException(): runtime_error("Null Function Pointer") {}
};

class Assembler {
typedef int (Assembler::*FP)(istringstream &);
map <string, FP> jumpTable;

public:
Assembler();
int assemble(fstream&, fstream&);
int load (istringstream &);
int loadi (istringstream &);
int store (istringstream &);
int add (istringstream &);
int addi (istringstream &);
int addc (istringstream &);
int addci (istringstream &);
int sub (istringstream &);
int subi (istringstream &);
int subc (istringstream &);
int subci (istringstream &);
int _and (istringstream &);;
int andi (istringstream &);
int _xor (istringstream &);
int xori (istringstream &);
int _compl (istringstream &);
int shl (istringstream &);
int shla (istringstream &);
int shr (istringstream &);
int shra (istringstream &);
int compr (istringstream &);
int compri (istringstream &);
int getstat (istringstream &);
int putstat (istringstream &);
int jump (istringstream &);
int jumpl(istringstream &);
int jumpe (istringstream &);
int jumpg (istringstream &);
int call (istringstream &);
int _return (istringstream &);
int _read (istringstream &);
int _write (istringstream &);
int halt (istringstream &);
int noop (istringstream &);
}; //Assembler
#endif

