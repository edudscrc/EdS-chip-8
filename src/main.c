#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static const int screen_width = 640;
static const int screen_height = 480;

// Starts up SDL and creates window
bool init();

// Loads media
bool load_media();

// Frees media and shuts down SDL
void close_app();

// The window we'll be rendering to
static SDL_Window *window = NULL;

// The surface contained by the window
static SDL_Surface *screen_surface = NULL;

// The image we will load and show on the screen
static SDL_Surface *hello_world = NULL;

int main()
{
    // Final exit code
    int exit_code = 0;

    // Initialize
    if (!init())
    {
        SDL_Log("Unable to initialize program!\n");
        exit_code = 1;
    }
    else
    {
        // Load media
        if (!load_media())
        {
            SDL_Log("Unable to load media!\n");
            exit_code = 2;
        }
        else
        {
            // The quit flag
            bool quit = false;

            // The event data
            SDL_Event e;
            SDL_zero(e);

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

                // Fill the surface white
                SDL_FillSurfaceRect(screen_surface, NULL, SDL_MapSurfaceRGB(screen_surface, 0xFF, 0xFF, 0xFF));

                // Render image on screen
                SDL_BlitSurface(hello_world, NULL, screen_surface, NULL);

                // Update the surface
                SDL_UpdateWindowSurface(window);
            }
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
        if (!(window = SDL_CreateWindow("SDL3 Tutorial: Hello SDL3", screen_width, screen_height, 0)))
        {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Get window surface
            screen_surface = SDL_GetWindowSurface(window);
        }
    }

    return success;
}

bool load_media()
{
    // File loading flag
    bool success = true;

    // Load splash image
    const char *image_path = "hello-sdl3.bmp";
    if (!(hello_world = SDL_LoadBMP(image_path)))
    {
        SDL_Log("Unable to load image %s! SDL Error: %s\n", image_path, SDL_GetError());
        success = false;
    }

    return success;
}

void close_app()
{
    // Clean up surface
    SDL_DestroySurface(hello_world);
    hello_world = NULL;

    // Destroy window
    SDL_DestroyWindow(window);
    window = NULL;
    screen_surface = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}