CC = gcc

BASE_CFLAGS = -Wall -Wextra -Wpedantic -std=c99
SDL_CFLAGS = $(shell pkg-config sdl3 --cflags)
SDL_LIBS = $(shell pkg-config sdl3 --libs)

DEBUG_FLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
RELEASE_FLAGS = -O2

# SRC = src/main.c src/chip8.c src/opcodes.c src/platform_sdl3.c
SRC = src/main.c
OUT = chip8

all: debug

debug:
	$(CC) $(BASE_CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(SRC) -o $(OUT) $(SDL_LIBS) $(DEBUG_FLAGS)

release:
	$(CC) $(BASE_CFLAGS) $(RELEASE_FLAGS) $(SDL_CFLAGS) $(SRC) -o $(OUT) $(SDL_LIBS)

clean:
	rm -f $(OUT)