#ifndef CHIP_8_H
#define CHIP_8_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

struct EdS_chip_8 {
    uint8_t memory[4096];
    bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT];

    // points at the current instruction in memory
    uint16_t PC;
    // I -> Index Register -> used to point at locations in memory
    uint16_t I;
    // used to call functions and return from them
    uint16_t stack[16];
    uint8_t stack_pointer;

    uint8_t delay_timer;
    uint8_t sound_timer;

    bool keyboard[16];

    // V0 until VF
    uint8_t V[16];
};

void EdS_chip_8_init(struct EdS_chip_8* obj);
void EdS_chip_8_load_rom(struct EdS_chip_8* obj, const char *rom);
void EdS_chip_8_init_font(struct EdS_chip_8* obj);
void EdS_chip_8_main_loop(struct EdS_chip_8* obj);

#endif