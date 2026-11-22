#pragma once

/*
 * Buzz Signal eyes: expression data.
 *
 * Every face is drawn on a 128x64 grid, the same as the 1.3" SH1106 OLED
 * and the Flipper Zero screen. Each eye is three vertical bars. Emotion
 * comes from the bar heights; a small pixel symbol ("deco") is only added
 * when the bars can't say it alone.
 *
 * This file has no Flipper dependencies so it can be reused on the ESP32.
 */

#include <stdbool.h>
#include <stdint.h>

/* Per-rect animation. Bars and deco pixels both use this. */
typedef enum {
    BZ_A_NONE = 0,
    BZ_A_BLINK, /* follows the shared random blink */
    BZ_A_EQ, /* equalizer bounce, staggered by slot */
    BZ_A_EQ_FAST, /* faster equalizer for music */
    BZ_A_ZIG, /* bob up and down, odd slots out of phase (dizzy) */
    BZ_A_BUILD, /* grow in from nothing, staggered (snap on) */
    BZ_A_DROP, /* collapse and vanish, staggered (detach) */
    BZ_A_DOT, /* blink on in sequence (thinking dots, find-me waves) */
    BZ_A_FILL, /* battery cell filling in a loop */
} BzAnim;

/* Whole-face motion applied to all bars. */
typedef enum {
    BZ_G_NONE = 0,
    BZ_G_LOOK, /* glance left, then right */
    BZ_G_HOP,
    BZ_G_HOP_FAST,
    BZ_G_BOB,
    BZ_G_PULSE, /* heartbeat scale */
    BZ_G_POP, /* slow swell */
    BZ_G_SHAKE,
    BZ_G_SHIVER,
    BZ_G_TILT, /* sheared side to side, reads as tilting */
    BZ_G_DIM, /* outlines only, low-power look */
    BZ_G_GLITCH,
} BzGroupAnim;

/* Motion applied to all deco pixels together. */
typedef enum {
    BZ_D_NONE = 0,
    BZ_D_FLOAT, /* drift up and disappear (Zz, music note) */
    BZ_D_FLASH,
    BZ_D_TEAR, /* slide down and disappear */
    BZ_D_BOB,
} BzDecoAnim;

typedef struct {
    int8_t x;
    int8_t y;
    int8_t w;
    int8_t h;
    uint8_t anim; /* BzAnim */
    uint8_t slot; /* stagger index for EQ/BUILD/DROP/DOT/FILL, parity for ZIG */
} BzRect;

typedef struct {
    const char* name;
    const BzRect* bars;
    uint8_t bar_count;
    const BzRect* deco;
    uint8_t deco_count;
    uint8_t group_anim; /* BzGroupAnim */
    uint8_t deco_anim; /* BzDecoAnim */
    bool caps; /* equalizer peak caps above the bars */
} BzMood;

typedef enum {
    /* Core feelings and signal moments */
    BZ_MOOD_IDLE = 0,
    BZ_MOOD_HAPPY,
    BZ_MOOD_EXCITED,
    BZ_MOOD_LOVE,
    BZ_MOOD_LISTENING,
    BZ_MOOD_MUSIC,
    BZ_MOOD_SURPRISED,
    BZ_MOOD_CURIOUS,
    BZ_MOOD_THINKING,
    BZ_MOOD_CONFUSED,
    BZ_MOOD_SLEEPY,
    BZ_MOOD_BORED,
    BZ_MOOD_SAD,
    BZ_MOOD_ANGRY,
    BZ_MOOD_SCARED,
    BZ_MOOD_DIZZY,
    BZ_MOOD_FOCUSED,
    BZ_MOOD_WINK,
    BZ_MOOD_SHY,
    BZ_MOOD_SMUG,
    BZ_MOOD_NOTIFY,
    BZ_MOOD_CHARGING,
    BZ_MOOD_LOWBAT,
    BZ_MOOD_GLITCH,
    /* MagSafe moments */
    BZ_MOOD_SNAPON,
    BZ_MOOD_DETACH,
    BZ_MOOD_SLIPPING,
    BZ_MOOD_SPOTLIGHT,
    BZ_MOOD_CAMERA,
    BZ_MOOD_POCKET,
    BZ_MOOD_FINDME,
    BZ_MOOD_UPSIDEDOWN,
    /* Static frames used by the blink and the snap-on boot sequence */
    BZ_MOOD_HALF,
    BZ_MOOD_CLOSED,
    BZ_MOOD_OFF,
    BZ_MOOD_SNAP1,
    BZ_MOOD_SNAP2,
    BZ_MOOD_SNAP3,
    BZ_MOOD_COUNT,
} BzMoodId;

/* Number of expressions you can browse (everything before the static frames). */
#define BZ_BROWSABLE_COUNT BZ_MOOD_HALF

extern const BzMood bz_moods[BZ_MOOD_COUNT];
