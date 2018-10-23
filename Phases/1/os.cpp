/*
Yousef Jarrar, Nicholas Chiodini
CSE 460 Dr. Z
Due Date: October 15th, 2018
Description: This is the 16 bit VM like described in the lab manual
It holds 4 general purpose registers and clock counter.
Contains a ALU and 256 Memory
*/

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

#include "Assembler.h"
#include "VirtualMachine.h"

using namespace std;

int main(int argc, char *argv[]) // This is where all the action takes place
{
    Assembler as;
    VirtualMachine vm;

    if (argc == 1) {
        cout << "Must supply an assembly file name.\n";
        exit(1);
    }

    string assemblyFile = argv[1];
    int pos = assemblyFile.find("."); //Grabbing the file and filling the array
    if (pos > assemblyFile.length() || assemblyFile.substr(pos) != ".s") {
        cout << "No .s suffix.\n"; //Checks for .s file
        exit(2);
    }

    string name = assemblyFile.substr(0, pos);
    string objectFile = name + ".o"; //Grabs the name and add .o at the end

    fstream assembly, objectCode;
    assembly.open(assemblyFile.c_str(), ios::in);
    objectCode.open(objectFile.c_str(), ios::out);
    if (!assembly.is_open() or !objectCode.is_open()) {
        cout << "Couldn't open " << assemblyFile << " and/or " << objectFile << endl;
        exit(3);
    }

    if (as.assemble(assembly, objectCode)) {
        cout << "Assembler Error\n";
        assembly.close();
        objectCode.close();
        exit(4);
    }

    assembly.close();
    objectCode.close(); // No errors found, closes both files

    objectCode.open(objectFile.c_str(), ios::in);
    if (!objectCode.is_open()) {
        cout << "Couldn't open " << objectFile << endl;
        exit(5);
    } // Checking the .in file

    string inFile = name + ".in";
    string outFile = name + ".out";
    fstream in, out;
    in.open(inFile.c_str(), ios::in);
    out.open(outFile.c_str(), ios::out);
    if (!in.is_open() or !out.is_open()) {
        cout << "Couldn't open " << inFile << " and/or " << outFile << endl;
        exit(6);
    }

    vm.run(objectCode, in, out); //VM takes control of .in and .out and loads into memory before executing
    out << "\nClock = " << vm.get_clock() << endl; // prints the value of clock

    objectCode.close();
    in.close();
    out.close();

    return 0;
} 
