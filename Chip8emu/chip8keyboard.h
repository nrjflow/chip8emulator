//
//  chip8keyboard.h
//  Chip8emu
//
//  Created by Soufiane on 28/11/2020.
//

#ifndef chip8keyboard_h
#define chip8keyboard_h
#include <stdbool.h>
struct chip8_keyboard{
    bool keyboard[16];
    const char* keymap;
};

void chip8_load_keymap(struct chip8_keyboard *keyboard, const char *map);
int chip8_key_map(struct chip8_keyboard *keyboard, int hkey);
void chip8_key_down(struct chip8_keyboard *keyboard, int vkey);
void chip8_key_up(struct chip8_keyboard *keyboard, int vkey);
bool chip8_key_is_down(struct chip8_keyboard *keyboard, int vgkey);

#endif /* chip8keyboard_h */
