#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    VM vm;
    
    initVM(&vm);

    Chunk chunk;

    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    writeChunk(&chunk, OP_NEGATE, 1);

    writeChunk(&chunk, OP_RETURN, 3);

    interpret(&vm, &chunk);
    
    freeVM(&vm);
    freeChunk(&chunk);

    return 0;
}
