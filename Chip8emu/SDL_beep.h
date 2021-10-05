//
//  SDL_beep.h
//  Chip8emu
//
//  Created by Soufiane on 28/11/2020.
//

#ifndef SDL_beep_h
#define SDL_beep_h

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes);

int Beep(int delay, int multi);
#endif /* SDL_beep_h */
