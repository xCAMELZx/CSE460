#include "Assembler.h"
#include "VirtualMachine.h"

using namespace std;

int main(int argc, char * argv[]) {
    Assembler as;
    VirtualMachine vm;
    as.assemble(argv[1]);
    vm.execute(argv[1]);
}
