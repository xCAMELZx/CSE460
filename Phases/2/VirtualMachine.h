/*
Yousef Jarrar, Nicholas Chiodini
CSE 460 Dr. Z
Due Date: October 15th, 2018
Description: Virtual Machine Header File.
A constructer is used to define functions like memory size, register size,
base, limit, program counter, instruction register, stack point
and getting the clock time
*/



#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <fstream>
#include <vector>

using namespace std;

class OS;

class VirtualMachine { //Constructors for VM
    int msize; //memory size
    int rsize; //register size
    int pc, ir, sr, sp, clock;
    int msp; // minimal stack pointer, needed for stat
    vector<int> mem;
    vector<int> r;
    int base, limit;
public:
    VirtualMachine(): msize(256), rsize(4), clock(0) { // Size of Memory = 256
      //Register from r[0]-r[3] || Initialize clock to 0
      mem.resize(msize); r.resize(rsize);
    }
    void run(int);
    int get_clock();

friend
    class OS;
}; // VirtualMachine

#endif
