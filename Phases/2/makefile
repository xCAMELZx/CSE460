all: os

os: Assembler.h VirtualMachine.h OS.h Assembler.cpp VirtualMachine.cpp OS.cpp
	g++ -Wall -Wextra -o os Assembler.cpp VirtualMachine.cpp OS.cpp
	
clean:
	rm -rf os
