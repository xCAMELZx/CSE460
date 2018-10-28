all: os

os: Assembler.h VirtualMachine.h OS.h Assembler.cpp VirtualMachine.cpp OS.cpp
	g++ -std=c++11 -Wall -o os Assembler.cpp VirtualMachine.cpp OS.cpp
	
clean:
	rm -rf os
