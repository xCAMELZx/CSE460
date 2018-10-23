/*
Yousef Jarrar, Nicholas Chiodini
CSE 460 Dr. Z
Due Date: October 15th, 2018
Description: The assembler cpp file will take in the opcodes that were defined
and will translate to object oriented code. Each operation is defined based on
table provided to us in Phase 1.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include "Assembler.h"

using namespace std;

Assembler::Assembler()
{
    jumpTable["load"] = &Assembler::load;
    jumpTable["loadi"] = &Assembler::loadi;
    jumpTable["store"] = &Assembler::store;
    jumpTable["add"] = &Assembler::add;
    jumpTable["addi"] = &Assembler::addi;
    jumpTable["addc"] = &Assembler::addc;
    jumpTable["addci"] = &Assembler::addci;
    jumpTable["sub"] = &Assembler::sub;
    jumpTable["subi"] = &Assembler::subi;
    jumpTable["subc"] = &Assembler::subc;
    jumpTable["subci"] = &Assembler::subci;
    jumpTable["and"] = &Assembler::_and;
    jumpTable["andi"] = &Assembler::andi;
    jumpTable["xor"] = &Assembler::_xor;
    jumpTable["xori"] = &Assembler::xori;
    jumpTable["compl"] = &Assembler::_compl;
    jumpTable["shl"] = &Assembler::shl;
    jumpTable["shla"] = &Assembler::shla;
    jumpTable["shr"] = &Assembler::shr;
    jumpTable["shra"] = &Assembler::shra;
    jumpTable["compr"] = &Assembler::compr;
    jumpTable["compri"] = &Assembler::compri;
    jumpTable["getstat"] = &Assembler::getstat;
    jumpTable["putstat"] = &Assembler::putstat;
    jumpTable["jump"] = &Assembler::jump;
    jumpTable["jumpl"] = &Assembler::jumpl;
    jumpTable["jumpe"] = &Assembler::jumpe;
    jumpTable["jumpg"] = &Assembler::jumpg;
    jumpTable["call"] = &Assembler::call;
    jumpTable["return"] = &Assembler::_return;
    jumpTable["read"] = &Assembler::_read;
    jumpTable["write"] = &Assembler::_write;
    jumpTable["halt"] = &Assembler::halt;
    jumpTable["noop"] = &Assembler::noop;
} // Assembler

int Assembler::assemble(fstream& in, fstream& out)
//Reads the Assembly Code from programs provided ".s" files
{
    string line;
    string opCode;
    const int success = false;
    const int error = true;

    int instruction;

    getline(in, line);
    while (!in.eof()) {
        istringstream str(line.c_str());
        str >> opCode; // Takes in a string (first word) from Assembly Code
        if (opCode[0] == '!') { // Set's the first word as the opCode
            getline(in, line);
            continue;
        }

        try {
            if (not jumpTable[opCode] )
                throw NullPointerException();
            else instruction = (this->*jumpTable[opCode])(str);

        } catch (NullPointerException e) {
            cerr << e.what() << endl;
            return error;
        }

        if (instruction == -1)
            return error;
        out << instruction << endl;
        getline(in, line);
    }
    return success;
}

/*Remember that there are 2 formats for instructions

Format 1: [15:11][10:9][8][7:6][5:0]
             OP    RD   I   RS UNUSED
Format 2:
          [15:11][10:9][8] [7:0]
            OP     RD   I  ADDR/CONST
After checking that rd and const/addr are within range
we set the opcode and we store the instruction based on format
ex: inst = inst | rd << 9 | addr
*/

int Assembler::load(istringstream & str)
{
    int rd, addr; //Defining rd, and Address
    str >> rd >> addr; //Takes from the string and stores them
    if (rd < 0 || rd > 3) //Checks rd is within range "0<= x =>3"
        return -1;
    if (addr < 0 || addr > 255) // Checks Address is within range 0 < x > 255
        return -1;
    int inst=0; // Set opCodes value from jumpTable
    inst = inst | rd<<9 | addr; // Based on Formats 1 & 2 ;; Store instruction
    return inst;
}

int Assembler::loadi(istringstream & str)
{
    int rd, constant;  //Defining RD and Constant
    str >> rd >> constant; // Takes from the string and stores them
    if (rd < 0 || rd > 3) // Checks again like previous instruction
        return -1;
    if (constant < -128 || constant > 127) //Checks constant's range -128 < x > 127
        return -1;
    int inst=0;
    inst = inst | rd<<9 | 1<<8 | (0x000000ff & constant); // Last part is I bit
    // i << 8;
    return inst;
}

int Assembler::store(istringstream & str)
{
    int rd, addr;
    str >> rd >> addr;
    if (rd < 0 || rd > 3)
        return -1;
    if (addr < 0 || addr > 255)
        return -1;
    int inst=1; //Remember that the number defined is the opCode from Binary 0001
    inst = inst<<11 | rd<<9 | addr;
    return inst;
}

int Assembler::add(istringstream & str)
{
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3)
        return -1;
    if (rs < 0 || rs > 3)
        return -1;
    int inst=2; // 0010 Binary Number for 2
    inst = inst<<11 | rd<<9 | rs<<6;
    return inst;
}

int Assembler::addi(istringstream & str)
{
    int rd, constant;
    str >> rd >> constant;
    if (rd < 0 || rd > 3)
        return -1;
    if (constant < -128 || constant > 127)
        return -1;
    int inst=2;
    inst = inst<<11 | rd<<9 | 1<<8 | (0x000000ff & constant);
    return inst;
}

int Assembler::addc(istringstream & str)
{
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3)
        return -1;
    if (rs < 0 || rs > 3)
        return -1;
    int inst=3;
    inst = inst<<11 | rd<<9 | rs<<6;
    return inst;
}

int Assembler::addci(istringstream & str)
{
    int rd, constant;
    str >> rd >> constant;
    if (rd < 0 || rd > 3)
        return -1;
    if (constant < -128 || constant > 127)
        return -1;
    int inst=3;
    inst = inst<<11 | rd<<9 | 1<<8 | (0x000000ff & constant);
    return inst;
}

int Assembler::sub(istringstream & str)
{
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3)
        return -1;
    if (rs < 0 || rs > 3)
        return -1;
    int inst=4;
    inst = inst<<11 | rd<<9 | rs<<6;
    return inst;
}

int Assembler::subi(istringstream & str)
{
    int rd, constant;
    str >> rd >> constant;
    if (rd < 0 || rd > 3)
        return -1;
    if (constant < -128 || constant > 127)
        return -1;
    int inst=4;
    inst = inst<<11 | rd<<9 | 1<<8 | (0x000000ff & constant);
    return inst;
}

int Assembler::subc(istringstream & str)
{
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3)
        return -1;
    if (rs < 0 || rs > 3)
        return -1;
    int inst=5;
    inst = inst<<11 | rd<<9 | rs<<6;
    return inst;
}

int Assembler::subci(istringstream & str)
{
    int rd, constant;
    str >> rd >> constant;
    if (rd < 0 || rd > 3)
        return -1;
    if (constant < -128 || constant > 127)
        return -1;
    int inst=5;
    inst = inst<<11 | rd<<9 | 1<<8 | (0x000000ff & constant);
    return inst;
}

int Assembler::_and(istringstream & str)
{
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3)
        return -1;
    if (rs < 0 || rs > 3)
        return -1;
    int inst=6;
    inst = inst<<11 | rd<<9 | rs<<6;
    return inst;
}

int Assembler::andi(istringstream & str)
{
    int rd, constant;
    str >> rd >> constant;
    if (rd < 0 || rd > 3)
        return -1;
    if (constant < -128 || constant > 127)
        return -1;
    int inst=6;
    inst = inst<<11 | rd<<9 | 1<<8 | (0x000000ff & constant);
    return inst;
}

int Assembler::_xor(istringstream & str)
{
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3)
        return -1;
    if (rs < 0 || rs > 3)
        return -1;
    int inst=7;
    inst = inst<<11 | rd<<9 | rs<<6;
    return inst;
}

int Assembler::xori(istringstream & str)
{
    int rd, constant;
    str >> rd >> constant;
    if (rd < 0 || rd > 3)
        return -1;
    if (constant < -128 || constant > 127)
        return -1;
    int inst=7;
    inst = inst<<11 | rd<<9 | 1<<8 | (0x000000ff & constant);
    return inst;
}

int Assembler::_compl(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=8;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::shl(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=9;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::shla(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=10;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::shr(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=11;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::shra(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=12;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::compr(istringstream & str)
{
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3)
        return -1;
    if (rs < 0 || rs > 3)
        return -1;
    int inst=13;
    inst = inst<<11 | rd<<9 | rs<<6;
    return inst;
}

int Assembler::compri(istringstream & str)
{
    int rd, constant;
    str >> rd >> constant;
    if (rd < 0 || rd > 3)
        return -1;
    if (constant < -128 || constant > 127)
        return -1;
    int inst=13;
    inst = inst<<11 | rd<<9 | 1<<8 | (0x000000ff & constant);
    return inst;
}

int Assembler::getstat(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=14;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::putstat(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=15;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::jump(istringstream & str)
{
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255)
        return -1;
    int inst=16;
    inst = inst<<11 |  addr;
    return inst;
}

int Assembler::jumpl(istringstream & str)
{
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255)
        return -1;
    int inst=17;
    inst = inst<<11 | addr;
    return inst;
}

int Assembler::jumpe(istringstream & str)
{
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255)
        return -1;
    int inst=18;
    inst = inst<<11 | addr;
    return inst;
}

int Assembler::jumpg(istringstream & str)
{
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255)
        return -1;
    int inst=19;
    inst = inst<<11 | addr;
    return inst;
}

int Assembler::call(istringstream & str)
{
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255)
        return -1;
    int inst=20;
    inst = inst<<11 | addr;
    return inst;
}

int Assembler::_return(istringstream & str)
{
    int inst=21;
    inst = inst<<11;
    return inst;
}

int Assembler::_read(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=22;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::_write(istringstream & str)
{
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3)
        return -1;
    int inst=23;
    inst = inst<<11 | rd<<9;
    return inst;
}

int Assembler::halt(istringstream & str)
{
    int inst=24;
    inst = inst<<11;
    return inst;
}

int Assembler::noop(istringstream & str)
{
    int inst=25;
    inst = inst<<11;
    return inst;
}
