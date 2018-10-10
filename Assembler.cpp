#include "Assembler.h"
// Call the assembler constructor. Based on the table.
// jumpTable function comes from the "jumpTable" from Pseudo Code
Assembler::Assembler() {
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
	jumpTable["and"] = &Assembler::and_;
	jumpTable["andi"] = &Assembler::andi;
	jumpTable["xor"] = &Assembler::xor_;
	jumpTable["xori"] = &Assembler::xori;
	jumpTable["compl"] = &Assembler::compl_;
	jumpTable["shl"] = &Assembler::shl;
	jumpTable["shla"] = &Assembler::shla;
	jumpTable["shr"] = &Assembler::shr;
	jumpTable["shra"] = &Assembler::shra;
	jumpTable["compr"] = &Assembler::compr;
	jumpTable["compri"] = &Assembler::compri;
	jumpTable["getstat"] = &Assembler::getstat;
	jumpTable["putstat"] = &Assembler::putstat;
	jumpTable["jump"] = &Assembler::jump;
	jumpTable["jumpe"] = &Assembler::jumpe;
	jumpTable["jumpg"] = &Assembler::jumpg;
	jumpTable["call"] = &Assembler::call;
	jumpTable["return"] = &Assembler::return_;
	jumpTable["read"] = &Assembler::read;
	jumpTable["write"] = &Assembler::write;
	jumpTable["halt"] = &Assembler::halt;
	jumpTable["noop"] = &Assembler::noop;
}
		  //Converting Assembly to Binary
		  void Assembler::assemble(string fileName)
      {
			  string barrier, opCode; //Barrier references when the Assembly code gets thrown into the conversion.

			  //Setting up the file names, and defining where it comes from
			  inputFile = fileName;
			  outputFile = fileName.erase(fileName.length() -2, 2) + ".o";
			  fstream output;

			  //Creating the .o file for output
			  output.open(outputFile.c_str(), ios::out);
			  output.close();
			  fstream assemblyCode;

			  //Open the .s file for input
			  assemblyCode.open(inputFile.c_str(), ios::in);

			  //Check to see if the file is open
			  if (!assemblyCode.is_open())
			  {
				  cout << "File cannot be read" << inputFile << endl;
				  exit(0);
			  }
			  //First line of assembly must be initiated, loaded into barrier
			  getline(assemblyCode, barrier);
			  {
				  //Repeat through entire code, and call when relevant.
				  while (!assemblyCode.eof()) {
					  if (barrier[0] == '!') {
						  getline(assemblyCode, barrier);
						  continue;
					  }
					  istringstream str(barrier.c_str());

					  try {
						  str >> opCode;
						  if (not jumpTable[opCode])
							  throw NullPointerException();
						  else
							  (this->*jumpTable[opCode]) (barrier.c_str());
					  }
					  catch (NullPointerException e) {
						  cerr << e.what() << endl;
						  exit(0);
					  }
					  getline(assemblyCode, barrier);
				  }
				  assemblyCode.close();
				  //End of the assembler FXN
			  }
      }
			  //Next comes the write fxn,,, Binary Soure to file
			  void Assembler::write(int codeBinary)
			  {
				  fstream outFile;

				  //Output the file
				  outFile.open(outputFile.c_str(), ios::in | ios::out | ios::ate);
				  //Check to see if it opened
				  if (!outFile.is_open()) {
					  cout << "FAILURE TO OPEN" << outputFile << endl;
					  exit(0);
				  }

				  //Instruction Code -- Write to file.
				  outFile << codeBinary << endl;
				  outFile.close();
			  }

			  //Checking the Register Destination Fxn -- confirming it is in range.
			  void Assembler::rdCHK(int RD) {
				  if (RD > 3 || RD < 0) {
					  cout << "Register Destination is invalid" << endl;
					  exit(0);
				  }
			  }
			  //Checking the Register Source  -- confirming that it's within range.
			  void Assembler::rsCHK(int RS) {
				  if (RS > 3 || RS < 0) {
					  cout << "Register Source is invalid" << endl;
				  }
			  }
			  //Checking the Address Fxn -- " .......... "
			  void Assembler::addrCHK(int ADDR) {
				  if (ADDR > 255 || ADDR < 0) {
					  cout << "Address is out of range" << endl;
					  exit(0);
				  }
			  }
			  //Checking the Constant Fxn -- "....."
			  void Assembler::constCHK(int CONST) {
				  if (CONST > 127 || CONST < -128) {
					  cout << "Constant not in range" << endl;
					  exit(1);
				  }
			  }
			  //Load Function
			  void Assembler::load(string i) {
				  int RD, ADDR;
				  string OPCODE;

				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> ADDR;

				  rdCHK(RD);
				  addrCHK(ADDR);

				  int binary = 0; // 0000
				  binary += RD << 9;
				  binary += ADDR;
				  write(binary);
			  }
			  //Loadi Fxn
			  void Assembler::loadi(string i) {
				  int RD, CONST, I = 1;
				  string OPCODE;
				  istringstream iStream(i.c_str());

				  rdCHK(RD);
				  constCHK(CONST);

				  int binary = 0; //0000
				  binary += RD << 9;
				  binary += I << 8;
				  if (CONST < 0)
					  CONST = CONST & 0x00FF;

				  binary += CONST;
				  write(binary);
			  }
			  //Store Fxn
			  void Assembler::store(string i) {
				  int RD, ADDR;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> ADDR;

				  rdCHK(RD);
				  addrCHK(ADDR);

				  int binary = 1; //0001
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += ADDR;
				  write(binary);
			  }
			  //Add Function
			  void Assembler::add(string i) {
				  int RD, RS, I = 0;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> RS;

				  rdCHK(RD);
				  rsCHK(RS);

				  int binary = 2; //0010
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;
				  binary += RS << 6;
				  write(binary);
			  }
			  //Addi Fxn
			  void Assembler::addi(string i) {
				  int RD, CONST, I = 1;
				  string OPCODE;

				  istringstream iStream(i.c_str());

				  iStream >> OPCODE >> RD >> CONST;

				  rdCHK(RD);
				  constCHK(CONST);

				  int binary = 2;
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;

				  if (CONST < 0)
					  CONST = CONST & 0x00FF;

				  binary += CONST;
				  write(binary);

			  }
			  //Addc Fxn
			  void Assembler::addc(string i) {
				  int RD, RS, I = 0;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> RS;

				  rdCHK(RD);
				  rsCHK(RS);

				  int binary = 3; //00011
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;
				  binary += RS << 6;

				  write(binary);
			  }
			  //addci
			  void Assembler::addci(string i) {
				  int RD, CONST, I = 1;
				  string OPCODE;

				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> CONST;

				  rdCHK(RD);
				  constCHK(CONST);

				  int binary = 3;
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;

				  if (CONST < 0)
					  CONST = CONST & 0x00FF;
				  binary += CONST;

				  write(binary);
			  }
			  //sub FXN
			  void Assembler::sub(string i) {
				  int RD, RS, I = 0;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> RS;

				  rdCHK(RD);
				  rsCHK(RS);

				  int binary = 4; //00100
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;
				  binary += RS << 6;

				  write(binary);

			  }
			  //subi function
			  void Assembler::subi(string i) {
				  int RD, CONST, I = 1;
				  string OPCODE;
				  istringstream iStream(i.c_str());

				  iStream >> OPCODE >> RD >> CONST;

				  rdCHK(RD);
				  constCHK(CONST);

				  int binary = 4;
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;

				  if (CONST < 0)
					  CONST = CONST & 0x00FF;
				  binary += CONST;

				  write(binary);
			  }
			  //subc
			  void Assembler::subc(string i) {
				  int RD, RS, I = 0;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> RS;

				  rdCHK(RD);
				  rsCHK(RS);

				  int binary = 5; //00101
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;
				  binary += RS << 6;
				  write(binary);

			  }
			  //subci
			  void Assembler::subci(string i)
			  {
				  int RD, CONST, I = 1;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> CONST;

				  rdCHK(RD);
				  constCHK(CONST);

				  int binary = 5;
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;

				  if (CONST < 0)
					  CONST = CONST & 0x0FF;
				  binary += CONST;
				  write(binary);
			  }
			  //and_ function
			  void Assembler::and_(string i) {
				  int RD, RS, I = 0;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> RS;

				  rdCHK(RD);
				  rsCHK(RS);

				  int binary = 6; //00110
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;
				  binary += RS << 6;

				  write(binary);

			  }
			  //andi function
			  void Assembler::andi(string i) {
				  int RD, CONST, I = 1;
				  string OPCODE;

				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> CONST;

				  rdCHK(RD);
				  constCHK(CONST);

				  int binary = 6;
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;

				  if (CONST < 0)
					  CONST = CONST & 0x0FF;
				  binary += CONST;
				  write(binary);
			  }
			//xor_ fxn
			  void Assembler::xor_(string i) {
				  int RD, RS, I = 0;
				  string OPCODE;

				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> RS;

				  rdCHK(RD);
				  rsCHK(RS);

				  int binary = 7; //00111
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;
				  binary += RS << 6;

				  write(binary);
			  }
			  //xori fxn
			  void Assembler::xori(string i) {
				  int RD, CONST, I = 1;
				  string OPCODE;

				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> CONST;

				  rdCHK(RD);
					constCHK(CONST);

					  int binary = 7;
					  binary = binary << 11;
					  binary += RD << 9;
					  binary += I = 8;

					  if (CONST < 0)
						  CONST = CONST & 0x0FF;
					  binary += CONST;
					  write(binary);
			}
			//compl_ fxn
			  void Assembler::compl_(string i) {
				  int RD, I = 0;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD;


				  rdCHK(RD);

				  int binary = 8; // 01000
				  binary = binary << 11;
				  binary += RD << 9;

				  write(binary);
			  }
			  //shl fxn -- BITSHIFT!!! Left register value.
			  void Assembler::shl(string i){
				  int RD, I = 0;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD;

				  rdCHK(RD);


				  int binary = 9; // 01001
				  binary = binary << 11;
				  binary += RD << 9;
				  write(binary);
			  }
			  //shla fxn .. yet another bitshift ,, to the left.
			  void Assembler::shla(string i){
				  int RD, I = 0;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD;

			      rdCHK(RD);

				  int binary = 10; // 01010
				  binary = binary << 11;
				  binary += RD << 9;

				  write(binary);
			  }
			  //shr fxn -- bitshift to the right this time......
			  void Assembler::shr(string i){
				  int RD;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD;


				  rdCHK(RD);

				  int binary = 11; // 01011
				  binary = binary << 11;
				  binary += RD << 9;
				  write(binary);
			  }
			  //shra function .. bitshift to right. Arithmetic register value.
			  void Assembler::shra(string i){
				  int RD;
				  string OPCODE;

				  istringstream iStream(i.c_str());
					iStream >> OPCODE >> RD;


				  rdCHK(RD);

				  int binary = 12; // 01100
				  binary = binary << 11;
				  binary += RD << 9;

				  write(binary);
			  }
			  //compr function -- comparison of two register values.
			  void Assembler::compr(string i) {
				  int RD, RS;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> RS;


				  rdCHK(RD);
				  rsCHK(RS);

				  int binary = 13; // 01101
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += RS << 6;
				  write(binary);
			  }
			  //compri fxn
			  void Assembler::compri(string i) {
				  int RD, CONST, I = 1;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD >> CONST;

				  rdCHK(RD);
				  constCHK(CONST);

				  int binary = 13; // 01101
				  binary = binary << 11;
				  binary += RD << 9;
				  binary += I << 8;

				  if (CONST < 0)
					  CONST = CONST & 0x00FF;

				  binary += CONST;

				  write(binary);
			  }
			  //getstat -- puts the value in status register
			  void Assembler::getstat(string i){
				  int RD;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD;

				 rdCHK(RD);

				  int binary = 14; // 01110
				  binary = binary << 11;
				  binary += RD << 9;

				  write(binary);
			  }
			  //putstat -- puts register value in status register
			  void Assembler::putstat(string i) {
				  int RD;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD;

				  rdCHK(RD);

				  int binary = 15; // 01111
				  binary = binary << 11;
				  binary += RD << 9;

				  write(binary);
			  }

			//jump fxn -- jumps to specified address
			  void Assembler::jump(string i)
			  {
				  int ADDR;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> ADDR;

				  addrCHK(ADDR);

				  int binary = 16; // 10000
				  binary = binary << 11;
				  binary += ADDR;

				  write(binary);
			  }
			  //jumpl -- jump function with a flag set. LESS
			  void Assembler::jumpl(string i) {
				  int ADDR;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> ADDR;

				  addrCHK(ADDR);

				  int binary = 17; // 10001
				  binary = binary << 11;
				  binary += ADDR;

				  write(binary);
			  }

			//jumpe fxn -- same as above BUT if equal flag is set
			  void Assembler::jumpe(string i){
				  int ADDR;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> ADDR;

				  addrCHK(ADDR);

				  int binary = 18; // 10010
				  binary = binary << 11;
				  binary += ADDR;

				  write(binary);
			  }
			  //jumpg fnxn -- " ...... " Greater Flag Set.
			  void Assembler::jumpg(string i) {
				  int ADDR;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> ADDR;

				  addrCHK(ADDR);

				  int binary = 19; // 10011
				  binary = binary << 11;
				  binary += ADDR;

				  write(binary);
			  }
			  //call fxn -- put address in Program Counter
			  void Assembler::call(string i){

				  int ADDR;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> ADDR;

				  addrCHK(ADDR);

				  int binary = 20; // 10100
				  binary = binary << 11;
				  binary += ADDR;

				  write(binary);
			  }
			  //return_ fxn -- pop and restore
			  void Assembler::return_(string i)
			  {
				  int binary = 21; // 10101
				  binary = binary << 11;

				  write(binary);
			  }

			//read fxn -- takes info from the .in file into register
			  void Assembler::read(string i){
				  int RD;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD;

				  rdCHK(RD);

				  int binary = 22; // 10110
				  binary = binary << 11;
				  binary += RD << 9;

				  write(binary);
			  }
			  //write fxn -- put values from register into .out file
			  void Assembler::write(string i){
				  int RD;
				  string OPCODE;
				  istringstream iStream(i.c_str());
				  iStream >> OPCODE >> RD;

				  rdCHK(RD);

				  int binary = 23; // 10111
				  binary = binary << 11;
				  binary += RD << 9;

				  write(binary);
			  }
			  //halt fxn -- stops entire process
			  void Assembler::halt(string i)
			  {
				  int binary = 24; // 11000
				  binary = binary << 11;

				  write(binary);
			  }

			  //noop fxn -- no operations
			  void Assembler::noop(string i)
			  {
				  int binary = 25; // 11001
				  binary = binary << 11;

				  write(binary);
			  }
