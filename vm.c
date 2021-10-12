#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

static void resetStack(VM* vm) {
    vm->stackTop = vm->stack;
}

void initVM(VM* vm) {
    resetStack(vm);
}

void freeVM(VM* vm) {
}

void push(VM* vm, Value value) {
    *vm->stackTop = value;
    vm->stackTop++;
}

Value pop(VM* vm) {
    vm->stackTop--;
    return *vm->stackTop;
}

static uint8_t readByte(VM* vm) {
    uint8_t instruction = *(vm->ip);
    vm->ip++;
    return instruction;
}

static Value readConstant(VM* vm) {
    return vm->chunk->constants.values[readByte(vm)];
}

static InterpretResult run(VM* vm) {
    for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");

        for (Value* slot = vm->stack; slot < vm->stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }

        printf("\n");
        
        disassembleInstruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
#endif

        uint8_t instruction;

        switch (instruction = readByte(vm)) {
        case OP_CONSTANT: {
            Value constant = readConstant(vm);
            push(vm, constant);
            break;
        }
        case OP_NEGATE:
            push(vm, -pop(vm));
            break;
        case OP_RETURN:
            printValue(pop(vm));
            printf("\n");
            return INTERPRET_OK;
        }
    }
}

InterpretResult interpret(VM* vm, Chunk* chunk) {
    vm->chunk = chunk;
    vm->ip = vm->chunk->code;
    return run(vm);
}

