#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <SDL.h>
#include <stdint.h>

#define DISPLAY_SCALE 10
#define DISPLAY_TITLE "CHIP-8 Emulator"

extern void display_init(void);
extern void display_update(uint32_t *screen);
extern void display_close(void);

#endif
