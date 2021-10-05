//
//  chip8memory.h
//  Chip8emu
//
//  Created by Soufiane on 26/11/2020.
//

#ifndef chip8memory_h
#define chip8memory_h
#include "config.h"

struct chip8_memory {
    unsigned char memory[CHIP_MEMORY_SIZE];
};

void chip8_memory_set(struct chip8_memory *memory ,int index, unsigned char value);
unsigned char chip8_memory_get(struct chip8_memory *memory, int index);
unsigned short chip8_load_opcode(struct chip8_memory *memory, int index);
#endif /* chip8memory_h */
