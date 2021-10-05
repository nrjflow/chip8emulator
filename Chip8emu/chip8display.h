//
//  chip8display.h
//  Chip8emu
//
//  Created by Soufiane on 28/11/2020.
//

#ifndef chip8display_h
#define chip8display_h
#include "config.h"
#include <stdbool.h>
struct chip8_display {
    bool pixel[HEIGHT][WIDTH];
};

void chip8_pixel_on(struct chip8_display *display, int x, int y);
void chip8_pixel_xor_set(struct chip8_display *display, int x, int y);
bool chip8_pixel_is_on(struct chip8_display *display, int x, int y);
bool chip8_draw_sprite(struct chip8_display *display, int x, int y, const char *sprite, int N);
void chip8_display_clear(struct chip8_display *display);
#endif /* chip8display_h */
