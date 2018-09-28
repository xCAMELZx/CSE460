all: Assembler.cpp VirtualMachine.cpp
	g++ -o os os.cpp Assembler.cpp VirtualMachine.cpp
