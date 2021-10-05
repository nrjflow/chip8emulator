//
//  chip8stack.h
//  Chip8emu
//
//  Created by Soufiane on 28/11/2020.
//

#ifndef chip8stack_h
#define chip8stack_h

struct chip8;
struct chip8_stack {
    unsigned short stack[16];
};

void chip8_stack_push(struct chip8 *chip8, unsigned short val);
unsigned short chip8_stack_pop(struct chip8 *chip8);

#endif /* chip8stack_h */
