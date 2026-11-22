#pragma once

/*
 * Buzz Signal-eye animation engine.
 *
 * Pure C, no Flipper or ESP32 dependencies. Give it the current time in
 * milliseconds and it returns a list of rectangles to draw on a 128x64
 * screen. The Flipper app draws them with the Canvas API; an ESP32 port can
 * draw the same list with U8g2 (drawRBox / drawBox / drawFrame).
 */

#include <stdbool.h>
#include <stdint.h>

#include "buzz_faces.h"

#define BZ_SCREEN_W 128
#define BZ_SCREEN_H 64

/* The color panel (1.27" SSD1351) is 128x96. The face is drawn in the
 * 128x64 grid above; add BZ_FACE_OFFSET_Y to every y when drawing on it so
 * the face sits in the middle with room for symbols above and below. */
#define BZ_PANEL_W 128
#define BZ_PANEL_H 96
#define BZ_FACE_OFFSET_Y 16
#define BZ_MAX_DRAW 48

typedef enum {
    BZ_STYLE_BAR = 0, /* filled, slightly rounded (eye bars) */
    BZ_STYLE_BOX, /* filled, square (pixel symbols, caps) */
    BZ_STYLE_FRAME, /* outline only (dim, unlit dots, empty battery cells) */
} BzStyle;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    uint8_t style; /* BzStyle */
    uint32_t rgb; /* 0xRRGGBB for color panels; monochrome screens ignore it */
} BzDrawRect;

typedef struct {
    uint8_t mood;
    uint32_t mood_start_ms;

    /* Shared random blink, 3 to 5 s apart: 40 ms half, 60 ms closed, 40 ms half. */
    uint32_t next_blink_ms;
    uint32_t blink_start_ms;
    bool blinking;

    /* Equalizer peak caps: jump up with the bar, fall back slowly. */
    float cap_y[16];
    bool caps_ready;
    uint32_t last_render_ms;

    uint32_t rng;
} BzEngine;

void bz_engine_init(BzEngine* engine, uint32_t now_ms);

/* Switch expression. Animations restart from the beginning. */
void bz_engine_set_mood(BzEngine* engine, uint8_t mood, uint32_t now_ms);

/* The mood's main color as 0xRRGGBB. */
uint32_t bz_mood_color(uint8_t mood);

/* Fill `out` with what to draw right now. Returns the number of rects. */
uint8_t bz_engine_render(BzEngine* engine, uint32_t now_ms, BzDrawRect* out, uint8_t max_out);
