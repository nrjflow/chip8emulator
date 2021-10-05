//
//  chip8keyboard.c
//  Chip8emu
//
//  Created by Soufiane on 28/11/2020.
//

#include "chip8keyboard.h"
#include <assert.h>
static void chip8_is_valid_key(int vkey){
    assert(vkey >=0 && vkey < 16);
}

void chip8_load_keymap(struct chip8_keyboard *keyboard, const char *map){
    keyboard->keymap = map;
}

int chip8_key_map(struct chip8_keyboard *keyboard, int hkey){
    for (int index=0; index<16; index++) {
        if (keyboard->keymap[index]==hkey) {
            return index;
        }
    }
    return -1;
}
void chip8_key_down(struct chip8_keyboard *keyboard, int vkey){
    chip8_is_valid_key(vkey);
    keyboard->keyboard[vkey]=true;
}
void chip8_key_up(struct chip8_keyboard *keyboard, int vkey){
    chip8_is_valid_key(vkey);
    keyboard->keyboard[vkey]=false;
}
bool chip8_key_is_down(struct chip8_keyboard *keyboard, int vkey){
    chip8_is_valid_key(vkey);
    return keyboard->keyboard[vkey];
}
