//
//  chip8.c
//  Chip8emu
//
//  Created by Soufiane on 28/11/2020.
//

#include "chip8.h"
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
const char chip8_default_charset[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

void chip8_init(struct chip8 *chip8){
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(chip8->memory.memory, chip8_default_charset, sizeof(chip8_default_charset));
}

void chip8_load(struct chip8 *chip8, const char *buffer, size_t size){
    assert(size+CHIP8_LOAD_ADDRESS<CHIP_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_LOAD_ADDRESS], buffer, size);
    chip8->registers.PC = CHIP8_LOAD_ADDRESS;
}

static char chip8_wait_for_key_press(struct chip8 *chip8){
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if(event.type != SDL_KEYDOWN)
            continue;
        char hkey = event.key.keysym.sym;
        char chip8_vkey = chip8_key_map(&chip8->keyboard, hkey);
        if(chip8_vkey != -1){
            return chip8_vkey;
        }
    }
    return -1;
}

static void chip8_exec_0x8000(struct chip8 *chip8, unsigned short opcode){
    unsigned char x = (opcode & 0x0f00) >> 8;
    unsigned char y = (opcode & 0x00f0) >> 4;
    unsigned char opcode_0x8000 = opcode & 0x000f;
    
    switch (opcode_0x8000) {
        case 0: //8xy0 - LD Vx, Vy - Set Vx = Vy.
            chip8->registers.V[x] = chip8->registers.V[y];
            break;
        case 1: //8xy1 - OR Vx, Vy - Set Vx = Vx OR Vy.
            chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
            break;
        case 2: //8xy2 - AND Vx, Vy - Set Vx = Vx AND Vy.
            chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
            break;
        case 3: //8xy3 - XOR Vx, Vy - Set Vx = Vx XOR Vy.
            chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
            break;
        case 4: //8xy4 - ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry.
        {
            short sum = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0f] = 0;
            if(sum > 255){
                chip8->registers.V[0x0F] = 1;
            }
            chip8->registers.V[x] = sum; // & 0xff;
        }
        case 5: //8xy5 - SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow.
            chip8->registers.V[0x0f] = 0;
            if(chip8->registers.V[x] > chip8->registers.V[y]){
                chip8->registers.V[0x0F] = 1;
            }
            chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
            break;
        case 6://8xy6 - SHR Vx {, Vy} - Set Vx = Vx SHR 1.
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b00000001;
            chip8->registers.V[x] = chip8->registers.V[x] / 2;
            break;
        case 7: //8xy7 - SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow.
            chip8->registers.V[0x0f] = 0;
            if(chip8->registers.V[y] > chip8->registers.V[x]){
                chip8->registers.V[0x0F] = 1;
            }
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
            break;
        case 0x0e://8xyE - SHL Vx {, Vy} - Set Vx = Vx SHL 1.
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] = chip8->registers.V[x] * 2;
            break;
        default:
            break;
    }
}

static void chip8_exec_0xF000(struct chip8 *chip8, unsigned short opcode){
    unsigned char x = (opcode & 0x0f00) >> 8;
    unsigned char opcode_0xF000 = opcode & 0x00ff;
    switch (opcode_0xF000) {
        case 0x07: // Fx07 - LD Vx, DT - Set Vx = delay timer value.
            chip8->registers.V[x] = chip8->registers.delay_timer;
            break;
        case 0x0A: // Fx0A - LD Vx, K - Wait for a key press, store the value of the key in Vx.
            chip8->registers.V[x] = chip8_wait_for_key_press(chip8);
            break;
        case 0x15: // Fx15 - LD DT, Vx - Set delay timer = Vx.
            chip8->registers.delay_timer = chip8->registers.V[x];
            break;
        case 0x18: // Fx18 - LD ST, Vx - Set sound timer = Vx.
            chip8->registers.sound_timer = chip8->registers.V[x];
            break;
        case 0x1E: // Fx1E - ADD I, Vx - Set I = I + Vx.
            chip8->registers.I = chip8->registers.I + chip8->registers.V[x];
            break;
        case 0x29: // Fx29 - LD F, Vx - Set I = location of sprite for digit Vx.
            chip8->registers.I = chip8->registers.V[x] * 5;
            break;
        case 0x33: // Fx33 - LD B, Vx - Store BCD representation of Vx in memory locations I, I+1, and I+2.
        {    unsigned char hundreds = chip8->registers.V[x] / 100;
            unsigned char tens = (chip8->registers.V[x] / 10) % 10;
            unsigned char ones = chip8->registers.V[x] % 10;
            chip8_memory_set(&chip8->memory, chip8->registers.I, hundreds);
            chip8_memory_set(&chip8->memory, chip8->registers.I+1, tens);
            chip8_memory_set(&chip8->memory, chip8->registers.I+2, ones);
            
        }
            break;
        case 0x55: // Fx55 - LD [I], Vx - Store registers V0 through Vx in memory starting at location I.
            {
                for (int index = 0; index <= x; index++) {
                    chip8_memory_set(&chip8->memory, chip8->registers.I + index, chip8->registers.V[index]);
                }
            }
            break;
        case 0x65: // Fx65 - LD Vx, [I] - Read registers V0 through Vx from memory starting at location I.
            {
                for (int index = 0; index <= x; index++) {
                    chip8->registers.V[index] = chip8_memory_get(&chip8->memory, chip8->registers.I + index);
                }
            }
            break;
        default:
            break;
    }
}

static void chip8_exec_parameterized(struct chip8 *chip8, unsigned short opcode){
    unsigned short nnn = opcode & 0x0fff;
    unsigned char kk = opcode & 0x00ff;
    unsigned char x = (opcode & 0x0f00) >> 8;
    unsigned char y = (opcode & 0x00f0) >> 4;
    unsigned char n = opcode & 0x000f;
    switch (opcode & 0xf000) {
        case 0x1000: //1nnn, jmp nnn
            chip8->registers.PC = nnn;
            break;
        case 0x2000: // 2nnn, call addr
            chip8_stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
        case 0x3000: // 3xkk, SE Vx, kk Skip next intruction if Vx == kk
            if (chip8->registers.V[x] == kk) {
                chip8->registers.PC += 2;
            }
            break;
        case 0x4000: // 3xkk, SNE Vx, kk  Skip next intruction if Vx != kk
            if (chip8->registers.V[x] != kk) {
                chip8->registers.PC += 2;
            }
            break;
        case 0x5000:// 5xy0, SE Vx, Vy  Skip next intruction if Vx == Vy
            if (chip8->registers.V[x] == chip8->registers.V[y]) {
                chip8->registers.PC += 2;
            }
            break;
        case 0x6000: //6xkk - LD Vx, byte -  set Vx = kk.
            chip8->registers.V[x] = kk;
            break;
        case 0x7000: //7xkk - ADD Vx, byte - Set Vx = Vx + kk.
            chip8->registers.V[x] += kk;
            break;
        case 0x8000:
            chip8_exec_0x8000(chip8, opcode);
            break;
        case 0x9000: //9xy0 - SNE Vx, Vy - Skip next instruction if Vx != Vy.
            if (chip8->registers.V[x] != chip8->registers.V[y]) {
                chip8->registers.PC += 2;
            }
            break;
        case 0xA000: //Annn - LD I, addr - Set I = nnn.
            chip8->registers.I = nnn;
            break;
        case 0xB000: //Bnnn - JP V0, addr - Jump to location nnn + V0.
            chip8->registers.PC = chip8->registers.V[0] + nnn;
            break;
        case 0xC000: //Cxkk - RND Vx, byte - Set Vx = random byte AND kk.
            srand((int)clock());
            chip8->registers.V[x] = (rand() & 255) & kk;
            break;
        case 0xD000:// Dxyn - DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        {
            const char *sprite = (const char*)&chip8->memory.memory[chip8->registers.I];
            chip8->registers.V[0x0f] = chip8_draw_sprite(&chip8->display, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
        }
            break;
        case 0xE000:
        {
            switch (opcode & 0x00ff) {
                case 0x9E: //Ex9E - SKP Vx - Skip next instruction if key with the value of Vx is pressed.
                    if (chip8_key_is_down(&chip8->keyboard, chip8->registers.V[x])) {
                        chip8->registers.PC += 2;
                    }
                    break;
                case 0xA1://ExA1 - SKNP Vx - Skip next instruction if key with the value of Vx is not pressed.
                    if (!chip8_key_is_down(&chip8->keyboard, chip8->registers.V[x])) {
                        chip8->registers.PC += 2;
                    }
                    break;
                default:
                    break;
            }
        }
            break;
        case 0xF000:
            chip8_exec_0xF000(chip8, opcode);
            break;
        default:
            break;
    }
}

void chip8_exec(struct chip8 *chip8, unsigned short opcode){
    switch (opcode) {
        case 0x00E0://cls
            chip8_display_clear(&chip8->display);
            break;
        case 0x00EE://ret
            chip8->registers.PC = chip8_stack_pop(chip8);
            break;
        default:
            chip8_exec_parameterized(chip8, opcode);
            break;
    }
}
