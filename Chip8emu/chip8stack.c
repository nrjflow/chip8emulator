//
//  chip8stack.c
//  Chip8emu
//
//  Created by Soufiane on 28/11/2020.
//

#include "chip8.h"
#include "chip8stack.h"
#include <assert.h>

static void chip8_stack_in_bounds(struct chip8 *chip8){
    assert(chip8->registers.SP < 16 && chip8->registers.SP >= 0);
}
void chip8_stack_push(struct chip8 *chip8, unsigned short val){
    chip8_stack_in_bounds(chip8);
    chip8->stack.stack[chip8->registers.SP] = val;
    chip8->registers.SP++;
}
unsigned short chip8_stack_pop(struct chip8 *chip8){
    chip8->registers.SP--;
    chip8_stack_in_bounds(chip8);
    return chip8->stack.stack[chip8->registers.SP];
}
