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
#include <cstdio>
#include <iomanip>

#include "Assembler.h"
#include "VirtualMachine.h"
#include "OS.h"

using namespace std;

OS::OS()
{
    // Gather processes info
    #if _WIN32 || defined(__CYGWIN32__)
        system("dir /B *.s > progs");
    #else
        system("ls *.s > progs");
    #endif
    progs.open("./progs", ios::in);
    if (!progs.is_open()) {
        cout << "Couldn't open progs file!" << endl;
        exit(2);
    }

    // Load each process
    string assemblyFile;
    int baseRunning = 0; // Current base for a process
    while (progs >> assemblyFile) {
        int pos = assemblyFile.find("."); //Grabbing the file and filling the array
        if (pos > (int)assemblyFile.length() || assemblyFile.substr(pos) != ".s") {
            cout << "No .s suffix.\n"; //Checks for .s file
            exit(3);
        }

        string name = assemblyFile.substr(0, pos);
        string objectFile = name + ".o"; //Grabs the name and add .o at the end

        fstream assembly, objectCode;
        assembly.open(assemblyFile.c_str(), ios::in);
        objectCode.open(objectFile.c_str(), ios::out);
        if (!assembly.is_open() or !objectCode.is_open()) {
            cout << "Couldn't open " << assemblyFile << " and/or " << objectFile << endl;
            exit(4);
        }

        if (as.assemble(assembly, objectCode)) {
            cout << "Assembler Error\n";
            assembly.close();
            objectCode.close();
            exit(5);
        }

        assembly.close();
        objectCode.close(); // No errors found, closes both files

        objectCode.open(objectFile.c_str(), ios::in);
        if (!objectCode.is_open()) {
            cout << "Couldn't open " << objectFile << endl;
            exit(6);
        }

        // Create new pcb
        PCB *newPcb = new PCB(name, baseRunning, baseRunning);
        string inFile = name + ".in";
        string outFile = name + ".out";
        newPcb->in.open(inFile.c_str(), ios::in);
        newPcb->out.open(outFile.c_str(), ios::out);
        if (!newPcb->in.is_open() || !newPcb->out.is_open()) {
            cout << "Couldn't open " << inFile << " and/or " << outFile << endl;
            exit(7);
        }

        // Load program's code
        for (; objectCode >> vm.mem[baseRunning]; baseRunning++);
        newPcb->limit = baseRunning;
        newPcb->sp = vm.msize;

        // Push the new PCB to the list
        pcb.push_back(newPcb);
        // And put it in the ready queue
        readyQ.push(newPcb);

        // Report
        cout << "Process " << newPcb->prog << " was loaded" << endl;
    }

    // First process is running at the start
    running = NULL;
    if (!readyQ.empty()) {
        running = readyQ.front();
        readyQ.pop();
        loadState();
    }
    sys_time = 0;
    idle_time = 0;
}

OS::~OS()
{
    // Calculate metrics
    int systemTime = idle_time + sys_time;
    double systemUtilization = 1.0 - ((double) idle_time) / ((double) vm.clock);
    int userClocks = 0;
    for (list<PCB *>::iterator it = pcb.begin(); it != pcb.end(); ++it) {
        PCB *currPcb = *it;
        userClocks += currPcb->cpu_time;
    }
    double userUtilization = ((double) userClocks) / ((double) vm.clock);
    double secondsRunning = ((double) vm.clock) / 1000.0;
    double throughput = ((double) pcb.size()) / secondsRunning;

    // Print metrics and finalize the processes
    for (list<PCB *>::iterator it = pcb.begin(); it != pcb.end(); ++it) {
        PCB *currPcb = *it;
        fixed(currPcb->out);
        currPcb->out << "\nThis process Statistics:\n" << setprecision(2);
        currPcb->out << "    CPU time = " << currPcb->cpu_time << " ticks\n";
        currPcb->out << "    Waiting time = " << currPcb->wait_time << " ticks\n";
        currPcb->out << "    Turnaround time = " << currPcb->turn_around_time << " ticks\n";
        currPcb->out << "    I/O time = " << currPcb->io_time << " ticks\n";
        currPcb->out << "    Largest stack size = " << vm.msize - currPcb->msp << endl;
        currPcb->out << "\nFull VM Statistics:\n" << setprecision(2);
        currPcb->out << "    System Time = " << systemTime << " ticks\n";
        currPcb->out << "    System CPU Utilization = " << systemUtilization << " %\n";
        currPcb->out << "    User CPU Utilization = " << userUtilization << "%\n";
        currPcb->out << "    Throughput: = " << throughput << " proc/s" << endl;
        currPcb->in.close();
        currPcb->out.close();
        delete currPcb;
    }
}

void OS::run()
{
    cout << endl << "Running OS..." << endl << endl;
    while (running != NULL || !readyQ.empty() || !waitQ.empty()) {
        int oldClock = vm.clock;
        if (running != NULL) {
            vm.run(TIME_SLICE);
            running->cpu_time += vm.clock - oldClock;
        }
        contextSwitch();
    }
}

void OS::loadState()
{
    if (running != NULL) {
        vm.pc = running->pc;
        for (int i = 0; i < 4; i++) {
            vm.r[i] = running->r[i];
        }
        vm.ir = running->ir;
        vm.sr = running->sr;
        vm.sp = running->sp;
        vm.msp = running->msp;
        vm.base = running->base;
        vm.limit = running->limit;
        if (running->sp != vm.msize) {
            string stackFile = running->prog + ".st";
            ifstream stack(stackFile.c_str(), std::ios::in);
            if (!stack.is_open()) {
                cout << "Failed to open " << stackFile << " to read stack dump" << endl;
                exit(9);
            }
            for (int i = running->sp; i < vm.msize; i++) {
                stack >> vm.mem[i];
            }
            stack.close();
        }
    }
}

void OS::saveState()
{
    if (running != NULL) {
        running->pc = vm.pc;
        for (int i = 0; i < 4; i++) {
            running->r[i] = vm.r[i];
        }
        running->ir = vm.ir;
        running->sr = vm.sr;
        running->sp = vm.sp;
        running->msp = vm.msp;
        running->base = vm.base;
        running->limit = vm.limit;
        if (running->sp != vm.msize) {
            string stackFile = running->prog + ".st";
            ofstream stack(stackFile.c_str(), std::ios::out | std::ios::trunc);
            if (!stack.is_open()) {
                cout << "Failed to open " << stackFile << " to save stack dump" << endl;
                exit(8);
            }
            for (int i = running->sp; i < vm.msize; i++) {
                stack << vm.mem[i] << " ";
            }
            stack.flush();
            stack.close();
        }
    }
}

int OS::dewaitPcbs()
{
    int minWaitDeadline = INT_MAX;
    int waitSize = waitQ.size();
    for (int i = 0; i < waitSize; i++) {
        PCB *currPcb = waitQ.front();
        waitQ.pop();
        if (currPcb->io_completion <= vm.clock) {
            readyQ.push(currPcb);
            currPcb->io_time += vm.clock - currPcb->io_time_begin;
            currPcb->io_time_begin = 0;
            currPcb->wait_time_begin = vm.clock;
        } else {
            waitQ.push(currPcb);
            minWaitDeadline = min(minWaitDeadline, currPcb->io_completion);
        }
    }

    return minWaitDeadline;
}


void OS::contextSwitch()
{
    // Take context switching penalty
    vm.clock += CONTEXT_SWITCH_TIME;
    sys_time += CONTEXT_SWITCH_TIME;

    int minWaitDeadline = INT_MAX;

    if (!waitQ.empty()) {
        minWaitDeadline = dewaitPcbs();
    }

    // Extract reason from register
    int switchReason = (vm.sr >> 5) & 7;
    int switchRegister = (vm.sr >> 8) & 3;

    if (switchReason == 0 && (vm.sr >> 4 & 1)) {
        // Classify overflow as result 8 for consistency of checking
        switchReason = 8;
    }

    // Place in appropriate queue
    if (switchReason == 0) {
        readyQ.push(running);
        saveState();
    } else if (switchReason == 6 || switchReason == 7) {
        // IO
        running->io_completion = vm.clock + 27;
        switch (switchReason) {
        case 6: // read
            running->in >> vm.r[switchRegister];
            // assert in range
            break;
        case 7: // write
            running->out << vm.r[switchRegister];
            break;
        }
        waitQ.push(running);
        running->io_time_begin = vm.clock;
        minWaitDeadline = min(minWaitDeadline, running->io_completion);
        saveState();
    } else {
        // Terminate
        string failure = "unknown reason";
        string stackFile = running->prog + ".st";
        switch (switchReason) {
        case 1:
            remove(stackFile.c_str());
            failure = "halt";
            break;
        case 2:
            failure = "out-of-bound reference";
            break;
        case 3:
            failure = "stack overflow";
            break;
        case 4:
            failure = "stack underflow";
            break;
        case 5:
            failure = "invalid opcode";
            break;
        case 8:
            failure = "overflow";
            break;
        default:
            failure = "unknown reason";
        }
        cout << "Process " << running->prog << " terminated because of " << failure << endl;
        running->out << "\n\nProcess terminated because of " << failure << endl;
        running->turn_around_time = vm.clock;
    }
    running = NULL;

    // Check if no process is ready and we need to idle
    if (running == NULL && readyQ.empty() && !waitQ.empty()) {
        // Update ticks
        idle_time += minWaitDeadline - vm.clock;
        vm.clock = minWaitDeadline;

        // Update waitQ and readyQ
        dewaitPcbs();
    }

    // Take the first process from a ready queue
    if (!readyQ.empty()) {
        running = readyQ.front();
        running->wait_time += vm.clock - running->wait_time_begin;
        running->wait_time_begin = 0;
        readyQ.pop();
        loadState();
    }
}

int main(int argc, char *argv[]) // This is where all the action takes place
{
    if (argc != 1) {
        cout << "Usage: " << argv[0] << endl;
        exit(1);
    }
    OS os;
    os.run();
    cout << endl << "Execution finished" << endl << endl;
    return 0;
}
