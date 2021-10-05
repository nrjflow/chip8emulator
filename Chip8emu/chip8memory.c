//
//  chip8memory.c
//  Chip8emu
//
//  Created by Soufiane on 26/11/2020.
//

#include "chip8memory.h"
#include <assert.h>

static void chip8_is_address_in_bounds(int index){
    assert(index >= 0 && index < CHIP_MEMORY_SIZE);
}

void chip8_memory_set(struct chip8_memory *memory ,int index, unsigned char value){
    chip8_is_address_in_bounds(index);
    memory->memory[index] = value;
}

unsigned char chip8_memory_get(struct chip8_memory *memory, int index){
    chip8_is_address_in_bounds(index);
    return memory->memory[index];
}

unsigned short chip8_load_opcode(struct chip8_memory *memory, int index){
    unsigned char byte1 = chip8_memory_get(memory, index);
    unsigned char byte2 = chip8_memory_get(memory, index+1);
    return byte1 << 8 | byte2;
}

