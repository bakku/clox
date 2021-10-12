#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

static void initLineArray(RLELineArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->lines = NULL;
}

static void freeLineArray(RLELineArray* array) {
    FREE_ARRAY(int, array->lines, array->capacity);
}

static void writeLine(RLELineArray* array, int line) {
    if (array->capacity != 0 && array->lines[array->count - 1] == line) {
        array->lines[array->count - 2] = array->lines[array->count - 2] + 1;
        return;
    }

    if (array->capacity == 0 || array->capacity < array->count + 2) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->lines = GROW_ARRAY(int, array->lines, oldCapacity, array->capacity);
    }

    array->lines[array->count] = 1;
    array->lines[array->count + 1] = line;

    array->count += 2;
}

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initValueArray(&chunk->constants);
    initLineArray(&chunk->lines);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLineArray(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    writeLine(&chunk->lines, line);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void writeConstant(Chunk* chunk, Value value, int line) {
    int constant = addConstant(chunk, value);

    uint8_t part1 = (constant >> 16) & 0xff;
    uint8_t part2 = (constant >> 8) & 0xff;
    uint8_t part3 = constant & 0xff;

    writeChunk(chunk, OP_CONSTANT_LONG, line);
    writeChunk(chunk, part1, line);
    writeChunk(chunk, part2, line);
    writeChunk(chunk, part3, line);
}

int getLine(Chunk* chunk, int instructionOffset) {
    int indexCounter = 1;
    int offsetCounter = 0;
    int amountCounter = 1;
    
    while (offsetCounter < instructionOffset) {
        if (chunk->lines.lines[indexCounter - 1] > amountCounter) {
            amountCounter++;
        } else {
            amountCounter = 1;
            indexCounter += 2;
        }

        offsetCounter++;
    }
    
    return chunk->lines.lines[indexCounter];
}
