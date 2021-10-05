//
//  chip8_registers.h
//  Chip8emu
//
//  Created by Soufiane on 27/11/2020.
//

#ifndef chip8_registers_h
#define chip8_registers_h

struct chip8_registers{
    unsigned char V[16];
    unsigned short I;
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned char SP;
    unsigned short PC;
};

#endif /* chip8_registers_h */
