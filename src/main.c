#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "../include/chip8.h"

static const int screen_width = 640;
static const int screen_height = 320;

// Starts up SDL and creates window
bool init();

// Frees media and shuts down SDL
void close_app();

// The window we'll be rendering to
static SDL_Window *window = NULL;

static SDL_Renderer *renderer = NULL;

static SDL_Texture *texture = NULL;

int main()
{
    // Final exit code
    int exit_code = 0;

    uint32_t *pixels = (uint32_t*)malloc(sizeof(uint32_t) * 64 * 32);

    // Initialize
    if (!init()) {
        SDL_Log("Unable to initialize program!\n");
        exit_code = 1;
    } else {
        // The quit flag
        bool quit = false;

        // The event data
        SDL_Event e;
        SDL_zero(e);

        struct EdS_chip_8 c8;
        EdS_chip_8_init(&c8);
        EdS_chip_8_load_rom(&c8, "./roms/logo_IBM.ch8");

        // The main loop
        while (!quit)
        {
            // Get event data
            while (SDL_PollEvent(&e))
            {
                // If event is quit type
                if (e.type == SDL_EVENT_QUIT)
                {
                    // End the main loop
                    quit = true;
                }
            }

            EdS_chip_8_main_loop(&c8);

            for (int i = 0; i < DISPLAY_HEIGHT; ++i)
                for (int j = 0; j < DISPLAY_WIDTH; ++j)
                    pixels[i * DISPLAY_WIDTH + j] = c8.display[i * DISPLAY_WIDTH + j] ? 0xFFFFFFFF : 0x000000FF;

            SDL_UpdateTexture(texture, NULL, pixels, DISPLAY_WIDTH * sizeof(uint32_t));

            SDL_RenderClear(renderer);

            SDL_RenderTexture(renderer, texture, NULL, NULL);

            SDL_RenderPresent(renderer);
        }
    }

    // Clean up
    close_app();

    return exit_code;
}

bool init()
{
    // Initialization flag
    bool success = true;
    
    // Initialie SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create window
        if (!SDL_CreateWindowAndRenderer("EdS CHIP-8", screen_width, screen_height, 0, &window, &renderer))
        {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            if (!(texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32))) {
                SDL_Log("Texture could not be created! SDL error: %s\n", SDL_GetError());
                success = false;
            }
        }
    }

    return success;
}

void close_app()
{
    // Clean up texture
    SDL_DestroyTexture(texture);

    // Destroy window
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}