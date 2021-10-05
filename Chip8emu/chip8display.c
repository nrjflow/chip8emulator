//
//  chip8display.c
//  Chip8emu
//
//  Created by Soufiane on 28/11/2020.
//

#include "chip8display.h"
#include <assert.h>
#include <memory.h>
static void chip8_display_check_bounds(int x, int y){
    assert(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}
void chip8_pixel_on(struct chip8_display *display, int x, int y){
    //chip8_display_check_bounds(x, y);
    display->pixel[y][x] = true;
}

void chip8_pixel_xor_set(struct chip8_display *display, int x, int y){
    //chip8_display_check_bounds(x, y);
    display->pixel[y][x] = true;
}

bool chip8_pixel_is_on(struct chip8_display *display, int x, int y){
    chip8_display_check_bounds(x, y);
    return display->pixel[y][x];
}

bool chip8_draw_sprite(struct chip8_display *display, int x, int y, const char *sprite, int N){
    bool pixel_overwrite = false;
    for (int line_n=0; line_n < N; line_n++) {
        char pixels_byte = sprite[line_n];
        for (int pixel_n=0; pixel_n < 8; pixel_n++) {
            if ((pixels_byte & (128 >> pixel_n)) == 0)
                continue;
            if(chip8_pixel_is_on(display, (x + pixel_n) % WIDTH, (y + line_n) % HEIGHT))
                pixel_overwrite = true;
            display->pixel[(y + line_n) % HEIGHT][(x + pixel_n) % WIDTH] ^= 1;
        }
    }
    return pixel_overwrite;
}

void chip8_display_clear(struct chip8_display *display){
    memset(display->pixel, 0, sizeof(display->pixel));
}
