#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct Chip8
{
    // 4 kilobytes of RAM
    uint8_t memory[4096];

    // 16 8-bit general-purpose variable registers.
    // The 16th register is used for the 'carry flag'.
    uint8_t V[16];
    // Index register. Used to point at locations in memory
    uint16_t I;
    // Program Counter. Points at the current instruction in memory
    uint16_t pc;

    // Used to call and return from functions. The array will save addresses.
    uint16_t stack[16];
    uint8_t sp;

    // This is decremented at a rate of 60 Hz until it reaches 0
    uint8_t delay_timer;
    // It functions like the delay_timer, but which also gives off a beeping sound as long as it's not 0
    uint8_t sound_timer;

    uint8_t display[64 * 32];
    bool keypad[16];

    uint16_t opcode;
};

// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM

// For the display, each pixel is a boolean value (a bit). It can be on or off.
// Basically, "off" can be black and "on" can be white. Or any other colors.
// Drawing is done in XOR mode and if a pixel is turned off as a result of drawing, the VF register is set.
// This is used for collision detection.

void chip8_init(struct Chip8 *chip8);
void chip8_load_rom(struct Chip8 *chip8, const char *rom_path);
void chip8_emulate_cycle();
//void chip8_set_keys();