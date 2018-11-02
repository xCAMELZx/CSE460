#ifndef OS_H
#define OS_H

#include <fstream>
#include <list>
#include <queue>
#include <string>
#include <climits>

#include "VirtualMachine.h"
#include "Assembler.h"

using namespace std;

class OS;

class PCB {
    int pc;
    int r[4];
    int ir;
    int sr;
    int sp;
    int msp;
    int base;
    int limit;

    string prog;
    fstream in;
    fstream out;
    fstream stack;

    int io_completion;

    int turn_around_time;
    int cpu_time;
    int wait_time; int wait_time_begin;
    int io_time;   int io_time_begin;
public:
    PCB(const string &p, const int &b, const int &l):
        pc(b), ir(0), sr(2), sp(256), msp(sp), base(b), limit(l), prog(p),
        cpu_time(0), wait_time(0), wait_time_begin(0), io_time(0), io_time_begin(0) {
            for (int i = 0; i < 4; i++) {
                r[i] = 0;
            }
        }
friend
    class OS;
};

class OS {
    VirtualMachine vm;
    Assembler as;

    list<PCB *> pcb; // jobs
    queue<PCB *> readyQ;
    queue<PCB *> waitQ;
    PCB * running;
    const static int TIME_SLICE = 15;
    const static int CONTEXT_SWITCH_TIME = 5;

    fstream progs;
    int idle_time;
    int sys_time; // total sys time

    int dewaitPcbs(); // Returns minimal io_completion
public:
    OS();
    ~OS();
    void run();
    void loadState(); // loads VM state from PCB entry
    void saveState(); // saves VM state to PCB entry
    void contextSwitch();
};

#endif
