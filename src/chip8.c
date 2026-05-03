#include "../include/chip8.h"

void chip8_init(struct Chip8 *chip8)
{
    chip8->pc = 0x200;
    chip8->opcode = 0;
    chip8->I = 0;
    chip8->sp = 0;
 
    for (int i = 0; i < 4096; ++i) chip8->memory[i] = 0;
    for (int i = 0; i < 16; ++i) chip8->V[i] = 0;
    for (int i = 0; i < 16; ++i) chip8->stack[i] = 0;
    for (int i = 0; i < 64 * 32; ++i) chip8->display[i] = 0;
    for (int i = 0; i < 16; ++i) chip8->keypad[i] = false;

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    for (int i = 0; i < 80; ++i) chip8->memory[0x50 + i] = chip8_fontset[i];
}

void chip8_load_rom(struct Chip8 *chip8, const char *rom_path)
{
    FILE *rom_buffer = fopen(rom_path, "rb");
    if (!rom_buffer)
    {
        // Error
        return;
    }

    if (fseek(rom_buffer, 0, SEEK_END) != 0)
    {
        // Error
        fclose(rom_buffer);
        return;
    }
    long buffer_size = ftell(rom_buffer);
    if (buffer_size < 0)
    {
        // Error
        fclose(rom_buffer);
        return;
    }
    fseek(rom_buffer, 0, SEEK_SET);
    rewind(rom_buffer);

    size_t read = fread(&(chip8->memory[0x200]), 1, buffer_size, rom_buffer);
    if (read != buffer_size)
    {
        // Error
        fclose(rom_buffer);
        return;
    }
}

void chip8_emulate_cycle()
{
    // Fetch, Decode, Execute one opcode

    // Fetch opcode
    /*
    1) Fetch one opcode from the memory at the location specified by the pc.
    2) Data is stored in an array in which each address contains one byte (memory).
    3) As one opcode is 2 bytes long, we will need to fetch two successive bytes and merge them to get the actual opcode.
    */

    // Decode opcode
    /*
    Check the opcode table to see what the fetched opcode means.
    */

    // Execute opcode
    /*
    Execute the opcode instruction.
    Update the pc.
    */

    // Update timers
    /*
    They both count down to 0 if they have been set to a value larger than 0.
    Frequency of 60 Hz -> decrement 60 times in 1 second.
    Implement something that slows down the emulation cycle to execute 60 opcodes in 1 second.
    */
}