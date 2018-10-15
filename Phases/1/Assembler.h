#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cstdlib>
#include <stdexcept>

using namespace std;

class NullPointerException: public runtime_error {
    public: NullPointerException(): runtime_error("An error has occured") {}
};

class Assembler {
    private:
        typedef void(Assembler:: * FP)(string);

    // Instruction Set
    void load(string i);
    void loadi(string i);
    void store(string i);
    void add(string i);
    void addi(string i);
    void addc(string i);
    void addci(string i);
    void sub(string i);
    void subi(string i);
    void subc(string i);
    void subci(string i);
    void and_(string i);
    void andi(string i);
    void xor_(string i);
    void xori(string i);
    void compl_(string i);
    void shl(string i);
    void shla(string i);
    void shr(string i);
    void shra(string i);
    void compr(string i);
    void compri(string i);
    void getstat(string i);
    void putstat(string i);
    void jump(string i);
    void jumpl(string i);
    void jumpe(string i);
    void jumpg(string i);
    void call(string i);
    void return_(string i);
    void read(string i);
    void write(string i);
    void halt(string i);
    void noop(string i);

    // Variables
    string inputFile, outputFile;
    map < string, FP > jumpTable;

    // Check Methodology
    void rdCHK(int RD);
    void rsCHK(int RS);
    void addrCHK(int ADDR);
    void constCHK(int CONST);
    void write(int codeBinary);

    public:
        Assembler();
    void assemble(string fileName);
};
#endif
