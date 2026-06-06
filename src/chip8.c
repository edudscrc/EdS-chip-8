#include "../include/chip8.h"

void EdS_chip_8_init(struct EdS_chip_8* obj) {
    EdS_chip_8_init_font(obj);

    obj->PC = 0x200;
    obj->I = 0;
    obj->stack_pointer = 0;

    for (uint8_t i = 0; i < 16; ++i) obj->keyboard[i] = false;
}

void EdS_chip_8_load_rom(struct EdS_chip_8* obj, const char *rom) {
    FILE *rom_file = fopen(rom, "rb");
    fseek(rom_file, 0, SEEK_END);
    long rom_size = ftell(rom_file);
    rewind(rom_file);

    uint8_t *data = (uint8_t*)malloc(sizeof(uint8_t) * rom_size);
    fread(data, 1, rom_size, rom_file);

    for (long i = 0; i < rom_size; ++i) obj->memory[0x200 + i] = data[i];

    free(data);
}

void EdS_chip_8_init_font(struct EdS_chip_8* obj) {
    const uint8_t sprite_0[5] = {0xF0, 0x90, 0x90, 0x90, 0xF0}; // 0
    const uint8_t sprite_1[5] = {0x20, 0x60, 0x20, 0x20, 0x70}; // 1
    const uint8_t sprite_2[5] = {0xF0, 0x10, 0xF0, 0x80, 0xF0}; // 2
    const uint8_t sprite_3[5] = {0xF0, 0x10, 0xF0, 0x10, 0xF0}; // 3
    const uint8_t sprite_4[5] = {0x90, 0x90, 0xF0, 0x10, 0x10}; // 4
    const uint8_t sprite_5[5] = {0xF0, 0x80, 0xF0, 0x10, 0xF0}; // 5
    const uint8_t sprite_6[5] = {0xF0, 0x80, 0xF0, 0x90, 0xF0}; // 6
    const uint8_t sprite_7[5] = {0xF0, 0x10, 0x20, 0x40, 0x40}; // 7
    const uint8_t sprite_8[5] = {0xF0, 0x90, 0xF0, 0x90, 0xF0}; // 8
    const uint8_t sprite_9[5] = {0xF0, 0x90, 0xF0, 0x10, 0xF0}; // 9
    const uint8_t sprite_A[5] = {0xF0, 0x90, 0xF0, 0x90, 0x90}; // A
    const uint8_t sprite_B[5] = {0xE0, 0x90, 0xE0, 0x90, 0xE0}; // B
    const uint8_t sprite_C[5] = {0xF0, 0x80, 0x80, 0x80, 0xF0}; // C
    const uint8_t sprite_D[5] = {0xE0, 0x90, 0x90, 0x90, 0xE0}; // D
    const uint8_t sprite_E[5] = {0xF0, 0x80, 0xF0, 0x80, 0xF0}; // E
    const uint8_t sprite_F[5] = {0xF0, 0x80, 0xF0, 0x80, 0x80}; // F

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
            switch (NNN) {
                case 0x00E0:
                    // clear screen
                    for (size_t i = 0; i < DISPLAY_HEIGHT; ++i)
                        for (size_t j = 0; j < DISPLAY_WIDTH; ++j)
                            obj->display[i * DISPLAY_WIDTH + j] = false;
                    break;
                case 0x00EE:
                    // return from a subroutine
                    obj->PC = obj->stack[--obj->stack_pointer];
                    break;
                default:
                    // Don't need to implement 0x0NNN
                    break;
            }
            break;
        case 1:
            // 1NNN (jumps to address NNN)
            obj->PC = NNN;
            break;
        case 2:
            // Calls subroutine at address NNN
            obj->stack[obj->stack_pointer++] = obj->PC;
            obj->PC = NNN;
            break;
        case 3:
            // Skip instruction
            if (obj->V[X] == NN) obj->PC += 2;
            break;
        case 4:
            // Skip instruction
            if (obj->V[X] != NN) obj->PC += 2;
            break;
        case 5:
            // Skip instruction
            if (obj->V[X] == obj->V[Y]) obj->PC += 2;
            break;
        case 6:
            // 6XNN (Sets VX to NN)
            obj->V[X] = NN;
            break;
        case 7:
            // 7XNN (Adds NN to VX) (carry flag is not changed)
            obj->V[X] += NN;
            break;
        case 8:
            break;
        case 9:
            // Skip instruction
            if (obj->V[X] != obj->V[Y]) obj->PC += 2;
            break;
        case 0xA:
            // ANNN (Sets I to the address NNN)
            obj->I = NNN;
            break;
        case 0xB:
            // Jumps to the address NNN + V0 (0xBNNN)
            obj->PC = obj->V[0] + NNN;
            // I could also make a setup that changes 0xBNNN for 0xBXNN to support more CHIP-8 programs
            // Jumps to address XNN + VX
            // 0xBXNN below:
            obj->PC = NNN + obj->V[X];
            break;
        case 0xC: {
            // Sets VX to the result of a bitwise AND operation on a random number and NN
            srand((unsigned int)time(NULL));
            uint8_t random_number = (uint8_t)(rand() % (255 - 0 + 1) + 0);
            obj->V[X] = random_number & NN;
            break;
        }
        case 0xD: {
            // DXYN:
            // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
            // Each row of 8 pixels is read as bit-coded starting from memory location I.
            // I values does not change after the execution of this instruction.
            // VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and
            // to 0 if that does not happen.

            uint8_t x = obj->V[X] % DISPLAY_WIDTH; // & 63
            uint8_t y = obj->V[Y] % DISPLAY_HEIGHT; // & 31

            obj->V[15] = 0;

            for (uint16_t i = 0; i < N; ++i) {
                uint8_t sprite_byte = obj->memory[obj->I + i];

                uint8_t bit_values[8];
                bit_values[0] = (sprite_byte & (1 << 7)) >> 7;
                bit_values[1] = (sprite_byte & (1 << 6)) >> 6;
                bit_values[2] = (sprite_byte & (1 << 5)) >> 5;
                bit_values[3] = (sprite_byte & (1 << 4)) >> 4;
                bit_values[4] = (sprite_byte & (1 << 3)) >> 3;
                bit_values[5] = (sprite_byte & (1 << 2)) >> 2;
                bit_values[6] = (sprite_byte & (1 << 1)) >> 1;
                bit_values[7] = sprite_byte & 1;

                for (uint8_t j = 0; j < 8; ++j) {
                    if (bit_values[j] == 1 && obj->display[y * DISPLAY_WIDTH + x] == true) {
                        obj->display[y * DISPLAY_WIDTH + x] = false;
                        obj->V[15] = 1;
                    }
                    else if (bit_values[j] == 1 && obj->display[y * DISPLAY_WIDTH + x] == false) {
                        obj->display[y * DISPLAY_WIDTH + x] = true;
                    }
                    ++x;
                    if (x > 63) {
                        x = obj->V[X] % 64;
                        break;
                    }
                }

                x = obj->V[X] % 64;

                ++y;
                if (y > 31) {
                    y = obj->V[Y] % 32;
                    break;
                }
            }
            break;
        }
        case 0xE:
            if (NN == 0x9E) {
                // if key corresponding to VX value is pressed
                if (obj->keyboard[obj->V[X]]) obj->PC += 2;
            } else if (NN == 0xA1) {
                // if key corresponding to VX value is NOT pressed
                if (!obj->keyboard[obj->V[X]]) obj->PC += 2;
            }
            break;
        case 0xF:
            break;
        default:
            // error
            break;
    }
}