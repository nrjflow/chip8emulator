//
//  main.c
//  Chip8emu
//
//  Created by Soufiane on 26/11/2020.
//
#include "SDL2/SDL.h"
#include <stdio.h>
#include <unistd.h>
#include "chip8.h"
#include "SDL_beep.h"
const char KEYBOARD_MAP[16]= {
    SDLK_b, 'Y', 'Z', SDLK_LEFTBRACKET, SDLK_BACKSLASH,
    SDLK_RIGHTBRACKET, SDLK_CARET, SDLK_UNDERSCORE, SDLK_BACKQUOTE, SDLK_a,
    SDLK_w, SDLK_e, SDLK_r, SDLK_s, SDLK_d,
    SDLK_f,
};

int main(int argc, const char * argv[]) {
    
    //loading file
    FILE *program_file = fopen("TETRIS", "rb");
    
    if(!program_file){
        printf("Failed to load program!\n");
        return -1;
    }
     
    fseek(program_file, 0, SEEK_END);
    long size_of_program = ftell(program_file);
    fseek(program_file, 0, SEEK_SET);
    
    char program_buffer[size_of_program];
    
    size_t is_read = fread(program_buffer, (int)size_of_program, 1, program_file);
    if(!is_read){
        printf("Failed to read program file!\n");
        return -1;
    }
    //
    
    struct chip8 chip;
    chip8_init(&chip);
    chip8_load_keymap(&chip.keyboard, KEYBOARD_MAP);
    chip8_load(&chip, program_buffer, sizeof(program_buffer));
    //TESTS
    
    //
   
    SDL_Init(SDL_INIT_EVERYTHING);
    int quit = 0;
    SDL_Window *window = SDL_CreateWindow(
         TITLE,
         SDL_WINDOWPOS_UNDEFINED,
         SDL_WINDOWPOS_UNDEFINED,
         WIDTH*WINDOW_SCALE, HEIGHT*WINDOW_SCALE,
         SDL_WINDOW_SHOWN
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while (!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                {
                    char hkey = event.key.keysym.sym;
                    int vkey = chip8_key_map(&chip.keyboard, hkey);
                    if(vkey != -1)
                        chip8_key_down(&chip.keyboard, vkey);
                }
                    break;
                case SDL_KEYUP:
                {
                    char hkey = event.key.keysym.sym;
                    int vkey = chip8_key_map(&chip.keyboard, hkey);
                    if(vkey != -1)
                        chip8_key_up(&chip.keyboard, vkey);
                    
                }
                    break;
                default:
                    break;
            }

        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        for (int x=0; x<WIDTH; x++) {
            for (int y=0; y<HEIGHT; y++) {
                if(chip8_pixel_is_on(&chip.display, x, y)){
                    SDL_Rect rect;
                    rect.x=x * WINDOW_SCALE;
                    rect.y=y * WINDOW_SCALE;
                    rect.h=WINDOW_SCALE;
                    rect.w=WINDOW_SCALE;
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
        SDL_RenderPresent(renderer);
        if(chip.registers.delay_timer > 0){
            //usleep(1);
            chip.registers.delay_timer--;
        }
        if(chip.registers.sound_timer > 0){
            Beep(10*chip.registers.sound_timer, chip.registers.sound_timer);
            chip.registers.sound_timer=0;
        }
        
        unsigned short opcode = chip8_load_opcode(&chip.memory, chip.registers.PC);
        chip.registers.PC +=2;
        //printf("%x\n", opcode);
               
        chip8_exec(&chip, opcode);
    }
    
    SDL_DestroyWindow(window);
    return 0;
}
