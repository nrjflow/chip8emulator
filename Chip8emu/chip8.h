//
//  Header.h
//  Chip8emu
//
//  Created by Soufiane on 26/11/2020.
//

#ifndef chip8_h
#define chip8_h
#include "config.h"
#include "chip8memory.h"
#include "chip8_registers.h"
#include "chip8stack.h"
#include "chip8keyboard.h"
#include "chip8display.h"
#include <stddef.h>

struct chip8 {
    struct chip8_memory memory;
    struct chip8_registers registers;
    struct chip8_stack stack;
    struct chip8_keyboard keyboard;
    struct chip8_display display;
};

void chip8_init(struct chip8 *chip8);
void chip8_load(struct chip8 *chip8, const char *buffer, size_t size);
void chip8_exec(struct chip8 *chip8, unsigned short opcode);
#endif /* chip8_h */
