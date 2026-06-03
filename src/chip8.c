#include "../include/chip8.h"

void EdS_chip_8_init(struct EdS_chip_8* obj) {
    EdS_chip_8_init_font(obj);
}

void EdS_chip_8_init_font(struct EdS_chip_8* obj) {
    for (size_t i = 0; i < 5; ++i) {
        obj->memory[0x050 + 5 * 0 + i] = sprite_0[i];
        obj->memory[0x050 + 5 * 1 + i] = sprite_1[i];
        obj->memory[0x050 + 5 * 2 + i] = sprite_2[i];
        obj->memory[0x050 + 5 * 3 + i] = sprite_3[i];
        obj->memory[0x050 + 5 * 4 + i] = sprite_4[i];
        obj->memory[0x050 + 5 * 5 + i] = sprite_5[i];
        obj->memory[0x050 + 5 * 6 + i] = sprite_6[i];
        obj->memory[0x050 + 5 * 7 + i] = sprite_7[i];
        obj->memory[0x050 + 5 * 8 + i] = sprite_8[i];
        obj->memory[0x050 + 5 * 9 + i] = sprite_9[i];
        obj->memory[0x050 + 5 * 10 + i] = sprite_A[i];
        obj->memory[0x050 + 5 * 11 + i] = sprite_B[i];
        obj->memory[0x050 + 5 * 12 + i] = sprite_C[i];
        obj->memory[0x050 + 5 * 13 + i] = sprite_D[i];
        obj->memory[0x050 + 5 * 14 + i] = sprite_E[i];
        obj->memory[0x050 + 5 * 15 + i] = sprite_F[i];
    }
}

void EdS_chip_8_main_loop(struct EdS_chip_8* obj) {
    // Adjust timings: around 700 instructions/second fits well enough for most CHIP-8 programs

    // Fetch the instruction from memory at the current PC
    uint16_t current_instruction = ((uint16_t)obj->memory[obj->PC++]) << 8;
    current_instruction |= (uint16_t)obj->memory[obj->PC++];

    // Decode the instruction to find out what the emulator should do
    uint8_t first_nibble = (uint8_t)((current_instruction & 0xF000) >> 12);
    uint8_t X = (uint8_t)((current_instruction & 0x0F00) >> 8);
    uint8_t Y = (uint8_t)((current_instruction & 0x00F0) >> 4);
    uint8_t N = (uint8_t)(current_instruction & 0x000F);
    uint8_t NN = (Y << 4) | N;
    uint16_t NNN = (((uint16_t)X) << 8) | (uint16_t)NN;

    // 00E0 (clear screen)
    // 1NNN (jump)
    // 6XNN (set register VX)
    // 7XNN (add value to register VX)
    // ANNN (set index register I)
    // DXYN (display/draw)

    switch (first_nibble) {
        case 0:
            // 0NNN
            // 00E0
            // 00EE
            break;
        case 1:
            // 1NNN
            break;
        case 2:
            // 2NNN
            break;
        case 3:
            // 3XNN
            break;
        case 4:
            // 4XNN
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 0xA:
            break;
        case 0xB:
            break;
        case 0xC:
            break;
        case 0xD:
            break;
        case 0xE:
            break;
        case 0xF:
            break;
        default:
            // error
            break;
    }

    // Execute the instruction and do what it tells you
}