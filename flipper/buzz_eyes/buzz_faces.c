#include "buzz_faces.h"

#include <stddef.h>

/*
 * Grid: 128x64. Left eye bars start at x=32, right eye at x=80.
 * Bars are 4 px wide with a 2 px gap, so each eye is 16 px wide and the
 * eyes are 48 px apart (center to center).
 */

#define R(x, y, w, h, a, s) {(x), (y), (w), (h), (a), (s)}

/* One eye: three 4 px bars at x0, x0+6, x0+12. (y, h) per bar.
 * EYE goes through EYE_IMPL so shorthand like OPEN expands into its six numbers first. */
#define EYE(...) EYE_IMPL(__VA_ARGS__)
#define EYE_IMPL(x0, y1, h1, y2, h2, y3, h3, a, s0) \
    R((x0), (y1), 4, (h1), (a), (s0)),          \
        R((x0) + 6, (y2), 4, (h2), (a), (s0) + 1), R((x0) + 12, (y3), 4, (h3), (a), (s0) + 2)

/* The neutral open eye: middle bar tallest. */
#define OPEN 24, 16, 20, 24, 24, 16
#define LINES 34, 3, 34, 3, 34, 3
#define ARCH 22, 8, 18, 8, 22, 8

#define N(arr) ((uint8_t)(sizeof(arr) / sizeof((arr)[0])))

/* ---------- Core ---------- */

static const BzRect b_idle[] = {EYE(32, OPEN, BZ_A_BLINK, 0), EYE(80, OPEN, BZ_A_BLINK, 3)};
static const BzRect b_happy[] = {EYE(32, ARCH, BZ_A_NONE, 0), EYE(80, ARCH, BZ_A_NONE, 3)};

static const BzRect b_excited[] = {
    EYE(32, 20, 10, 14, 12, 20, 10, BZ_A_NONE, 0),
    EYE(80, 20, 10, 14, 12, 20, 10, BZ_A_NONE, 3)};
static const BzRect d_sparkles[] = {
    R(12, 6, 2, 10, 0, 0),
    R(8, 10, 10, 2, 0, 0),
    R(114, 6, 2, 10, 0, 0),
    R(110, 10, 10, 2, 0, 0)};

/* Love: each eye becomes five thin bars shaped like a heart. */
#define HEART(x0)                                                                       \
    R((x0), 23, 3, 10, 0, 0), R((x0) + 4, 21, 3, 16, 0, 0), R((x0) + 8, 25, 3, 18, 0, 0), \
        R((x0) + 12, 21, 3, 16, 0, 0), R((x0) + 16, 23, 3, 10, 0, 0)
static const BzRect b_love[] = {HEART(31), HEART(79)};

static const BzRect b_listening[] = {
    EYE(32, 28, 10, 18, 26, 24, 16, BZ_A_EQ, 0),
    EYE(80, 20, 22, 27, 12, 22, 20, BZ_A_EQ, 3)};

static const BzRect b_music[] = {
    EYE(32, 22, 18, 16, 30, 26, 10, BZ_A_EQ_FAST, 0),
    EYE(80, 26, 10, 18, 26, 22, 18, BZ_A_EQ_FAST, 3)};
static const BzRect d_note[] = {
    R(114, 4, 2, 12, 0, 0),
    R(109, 13, 6, 4, 0, 0),
    R(116, 4, 4, 2, 0, 0),
    R(119, 6, 2, 2, 0, 0)};

static const BzRect b_surprised[] = {
    EYE(32, 18, 28, 14, 36, 18, 28, BZ_A_NONE, 0),
    EYE(80, 18, 28, 14, 36, 18, 28, BZ_A_NONE, 3)};

static const BzRect b_curious[] = {
    EYE(32, 26, 12, 22, 20, 26, 12, BZ_A_BLINK, 0),
    EYE(80, 20, 22, 16, 30, 20, 22, BZ_A_BLINK, 3)};

static const BzRect b_thinking[] = {
    EYE(28, 16, 12, 12, 20, 16, 12, BZ_A_NONE, 0),
    EYE(76, 16, 12, 12, 20, 16, 12, BZ_A_NONE, 3)};
static const BzRect d_dots[] = {
    R(100, 52, 3, 3, BZ_A_DOT, 1),
    R(106, 52, 3, 3, BZ_A_DOT, 2),
    R(112, 52, 3, 3, BZ_A_DOT, 3)};

static const BzRect b_confused[] = {
    EYE(32, OPEN, BZ_A_NONE, 0),
    EYE(80, 30, 6, 28, 8, 30, 6, BZ_A_NONE, 3)};
static const BzRect d_question[] = {
    R(110, 4, 6, 2, 0, 0),
    R(116, 6, 2, 4, 0, 0),
    R(112, 10, 4, 2, 0, 0),
    R(112, 12, 2, 3, 0, 0),
    R(112, 17, 2, 2, 0, 0)};

static const BzRect b_lines[] = {EYE(32, LINES, BZ_A_NONE, 0), EYE(80, LINES, BZ_A_NONE, 3)};
static const BzRect d_zz[] = {
    R(106, 8, 7, 2, 0, 0),
    R(111, 10, 2, 2, 0, 0),
    R(109, 12, 2, 2, 0, 0),
    R(107, 14, 2, 2, 0, 0),
    R(106, 16, 7, 2, 0, 0),
    R(116, 2, 4, 1, 0, 0),
    R(119, 3, 1, 1, 0, 0),
    R(117, 4, 1, 1, 0, 0),
    R(116, 5, 4, 1, 0, 0)};

static const BzRect b_bored[] = {
    EYE(32, 30, 8, 30, 8, 30, 8, BZ_A_BLINK, 0),
    EYE(80, 30, 8, 30, 8, 30, 8, BZ_A_BLINK, 3)};

/* Sad: bars droop toward the outside edges. */
static const BzRect b_sad[] = {
    EYE(32, 30, 12, 27, 14, 24, 16, BZ_A_NONE, 0),
    EYE(80, 24, 16, 27, 14, 30, 12, BZ_A_NONE, 3)};
static const BzRect d_tear[] = {R(34, 46, 2, 3, 0, 0), R(33, 48, 4, 2, 0, 0)};

/* Angry: bars slope down toward the middle like a frown. */
static const BzRect b_angry[] = {
    EYE(32, 18, 24, 22, 20, 26, 16, BZ_A_NONE, 0),
    EYE(80, 26, 16, 22, 20, 18, 24, BZ_A_NONE, 3)};

/* Scared: thinner 3 px bars. */
#define THIN_EYE(x0)                                                              \
    R((x0), 26, 3, 12, 0, 0), R((x0) + 5, 24, 3, 16, 0, 0), R((x0) + 10, 26, 3, 12, 0, 0)
static const BzRect b_scared[] = {THIN_EYE(34), THIN_EYE(82)};
static const BzRect d_sweat[] = {R(105, 8, 2, 2, 0, 0), R(104, 10, 4, 3, 0, 0), R(105, 13, 2, 1, 0, 0)};

static const BzRect b_dizzy[] = {
    EYE(32, 26, 12, 26, 12, 26, 12, BZ_A_ZIG, 0),
    EYE(80, 26, 12, 26, 12, 26, 12, BZ_A_ZIG, 3)};

static const BzRect b_focused[] = {
    EYE(32, 28, 8, 18, 28, 28, 8, BZ_A_NONE, 0),
    EYE(80, 28, 8, 18, 28, 28, 8, BZ_A_NONE, 3)};

static const BzRect b_wink[] = {EYE(32, OPEN, BZ_A_BLINK, 0), EYE(80, ARCH, BZ_A_NONE, 3)};

static const BzRect b_shy[] = {
    EYE(32, 34, 8, 32, 10, 34, 8, BZ_A_NONE, 0),
    EYE(80, 34, 8, 32, 10, 34, 8, BZ_A_NONE, 3)};
static const BzRect d_blush[] = {
    R(31, 48, 3, 2, 0, 0),
    R(37, 48, 3, 2, 0, 0),
    R(43, 48, 3, 2, 0, 0),
    R(79, 48, 3, 2, 0, 0),
    R(85, 48, 3, 2, 0, 0),
    R(91, 48, 3, 2, 0, 0)};

static const BzRect b_smug[] = {
    EYE(32, 30, 8, 28, 10, 26, 12, BZ_A_NONE, 0),
    EYE(80, 26, 12, 28, 10, 30, 8, BZ_A_NONE, 3)};

static const BzRect b_notify[] = {
    EYE(36, 18, 24, 14, 28, 18, 24, BZ_A_NONE, 0),
    EYE(84, 18, 24, 14, 28, 18, 24, BZ_A_NONE, 3)};
static const BzRect d_bang[] = {R(114, 3, 3, 10, 0, 0), R(114, 15, 3, 3, 0, 0)};

static const BzRect d_battery[] = {
    R(44, 20, 36, 2, 0, 0),
    R(44, 42, 36, 2, 0, 0),
    R(44, 20, 2, 24, 0, 0),
    R(78, 20, 2, 24, 0, 0),
    R(80, 28, 3, 8, 0, 0),
    R(49, 25, 7, 14, BZ_A_FILL, 0),
    R(58, 25, 7, 14, BZ_A_FILL, 1),
    R(67, 25, 7, 14, BZ_A_FILL, 2)};

static const BzRect b_lowbat[] = {
    EYE(32, 32, 8, 30, 10, 32, 8, BZ_A_NONE, 0),
    EYE(80, 32, 8, 30, 10, 32, 8, BZ_A_NONE, 3)};
static const BzRect d_lowbat[] = {
    R(104, 4, 14, 1, 0, 0),
    R(104, 11, 14, 1, 0, 0),
    R(104, 4, 1, 8, 0, 0),
    R(117, 4, 1, 8, 0, 0),
    R(118, 6, 2, 4, 0, 0),
    R(106, 6, 3, 4, BZ_A_DOT, 0)};

static const BzRect b_glitch[] = {
    EYE(32, 16, 10, 30, 14, 20, 8, BZ_A_NONE, 0),
    EYE(80, 24, 16, 14, 20, 30, 10, BZ_A_NONE, 3)};
static const BzRect d_glitch[] = {
    R(10, 50, 4, 2, 0, 0),
    R(116, 12, 3, 2, 0, 0),
    R(60, 6, 6, 1, 0, 0),
    R(70, 56, 2, 2, 0, 0)};

/* ---------- MagSafe moments ---------- */

static const BzRect b_snapon[] = {EYE(32, OPEN, BZ_A_BUILD, 0), EYE(80, OPEN, BZ_A_BUILD, 3)};
static const BzRect d_click[] = {R(14, 31, 8, 2, 0, 0), R(106, 31, 8, 2, 0, 0)};

static const BzRect b_detach[] = {EYE(32, OPEN, BZ_A_DROP, 0), EYE(80, OPEN, BZ_A_DROP, 3)};

static const BzRect b_open[] = {EYE(32, OPEN, BZ_A_NONE, 0), EYE(80, OPEN, BZ_A_NONE, 3)};
static const BzRect d_edges[] = {R(8, 30, 6, 2, 0, 0), R(114, 30, 6, 2, 0, 0)};

static const BzRect d_spot[] = {
    R(10, 6, 2, 8, 0, 0),
    R(7, 9, 8, 2, 0, 0),
    R(116, 48, 2, 8, 0, 0),
    R(113, 51, 8, 2, 0, 0)};

static const BzRect d_frame[] = {
    R(6, 6, 10, 2, 0, 0),
    R(6, 6, 2, 10, 0, 0),
    R(112, 6, 10, 2, 0, 0),
    R(120, 6, 2, 10, 0, 0),
    R(6, 56, 10, 2, 0, 0),
    R(6, 48, 2, 10, 0, 0),
    R(112, 56, 10, 2, 0, 0),
    R(120, 48, 2, 10, 0, 0)};

static const BzRect b_pocket[] = {
    EYE(32, 32, 6, 30, 8, 32, 6, BZ_A_NONE, 0),
    EYE(80, 32, 6, 30, 8, 32, 6, BZ_A_NONE, 3)};

static const BzRect b_findme[] = {EYE(32, OPEN, BZ_A_EQ, 0), EYE(80, OPEN, BZ_A_EQ, 3)};
static const BzRect d_waves[] = {
    R(22, 26, 2, 12, BZ_A_DOT, 1),
    R(16, 22, 2, 20, BZ_A_DOT, 2),
    R(10, 18, 2, 28, BZ_A_DOT, 3),
    R(104, 26, 2, 12, BZ_A_DOT, 1),
    R(110, 22, 2, 20, BZ_A_DOT, 2),
    R(116, 18, 2, 28, BZ_A_DOT, 3)};

/* Upside down: bars hang from the top edge. */
static const BzRect b_upside[] = {
    EYE(32, 18, 12, 18, 20, 18, 12, BZ_A_NONE, 0),
    EYE(80, 18, 12, 18, 20, 18, 12, BZ_A_NONE, 3)};
static const BzRect d_mouth[] = {R(60, 50, 8, 2, 0, 0), R(60, 50, 2, 6, 0, 0), R(66, 50, 2, 6, 0, 0)};

/* ---------- Static frames ---------- */

static const BzRect b_half[] = {
    EYE(32, 28, 8, 26, 12, 28, 8, BZ_A_NONE, 0),
    EYE(80, 28, 8, 26, 12, 28, 8, BZ_A_NONE, 3)};
static const BzRect b_snap1[] = {R(32, 28, 4, 8, 0, 0)};
static const BzRect b_snap2[] = {EYE(32, OPEN, BZ_A_NONE, 0)};

/* ---------- Table ---------- */

#define M(name, bars, g, caps) {(name), (bars), N(bars), NULL, 0, (g), BZ_D_NONE, (caps)}
#define MD(name, bars, deco, g, d, caps) {(name), (bars), N(bars), (deco), N(deco), (g), (d), (caps)}

const BzMood bz_moods[BZ_MOOD_COUNT] = {
    [BZ_MOOD_IDLE] = M("Idle", b_idle, BZ_G_LOOK, false),
    [BZ_MOOD_HAPPY] = M("Happy", b_happy, BZ_G_HOP, false),
    [BZ_MOOD_EXCITED] = MD("Excited", b_excited, d_sparkles, BZ_G_HOP_FAST, BZ_D_FLASH, false),
    [BZ_MOOD_LOVE] = M("Love", b_love, BZ_G_PULSE, false),
    [BZ_MOOD_LISTENING] = M("Listening", b_listening, BZ_G_NONE, true),
    [BZ_MOOD_MUSIC] = MD("Vibing", b_music, d_note, BZ_G_NONE, BZ_D_FLOAT, true),
    [BZ_MOOD_SURPRISED] = M("Surprised", b_surprised, BZ_G_POP, false),
    [BZ_MOOD_CURIOUS] = M("Curious", b_curious, BZ_G_BOB, false),
    [BZ_MOOD_THINKING] = MD("Thinking", b_thinking, d_dots, BZ_G_NONE, BZ_D_NONE, false),
    [BZ_MOOD_CONFUSED] = MD("Confused", b_confused, d_question, BZ_G_NONE, BZ_D_BOB, false),
    [BZ_MOOD_SLEEPY] = MD("Sleepy", b_lines, d_zz, BZ_G_NONE, BZ_D_FLOAT, false),
    [BZ_MOOD_BORED] = M("Bored", b_bored, BZ_G_NONE, false),
    [BZ_MOOD_SAD] = MD("Sad", b_sad, d_tear, BZ_G_NONE, BZ_D_TEAR, false),
    [BZ_MOOD_ANGRY] = M("Angry", b_angry, BZ_G_SHAKE, false),
    [BZ_MOOD_SCARED] = MD("Scared", b_scared, d_sweat, BZ_G_SHIVER, BZ_D_TEAR, false),
    [BZ_MOOD_DIZZY] = M("Dizzy", b_dizzy, BZ_G_NONE, false),
    [BZ_MOOD_FOCUSED] = M("Focused", b_focused, BZ_G_NONE, false),
    [BZ_MOOD_WINK] = M("Wink", b_wink, BZ_G_BOB, false),
    [BZ_MOOD_SHY] = MD("Shy", b_shy, d_blush, BZ_G_NONE, BZ_D_FLASH, false),
    [BZ_MOOD_SMUG] = M("Smug", b_smug, BZ_G_BOB, false),
    [BZ_MOOD_NOTIFY] = MD("Alert", b_notify, d_bang, BZ_G_HOP, BZ_D_FLASH, false),
    [BZ_MOOD_CHARGING] = {"Charging", NULL, 0, d_battery, N(d_battery), BZ_G_NONE, BZ_D_NONE, false},
    [BZ_MOOD_LOWBAT] = MD("Low battery", b_lowbat, d_lowbat, BZ_G_NONE, BZ_D_NONE, false),
    [BZ_MOOD_GLITCH] = MD("Glitch", b_glitch, d_glitch, BZ_G_GLITCH, BZ_D_FLASH, false),

    [BZ_MOOD_SNAPON] = MD("Snap on", b_snapon, d_click, BZ_G_NONE, BZ_D_FLASH, false),
    [BZ_MOOD_DETACH] = M("Bye for now", b_detach, BZ_G_NONE, false),
    [BZ_MOOD_SLIPPING] = MD("Slipping", b_open, d_edges, BZ_G_TILT, BZ_D_FLASH, false),
    [BZ_MOOD_SPOTLIGHT] = MD("My turn", b_excited, d_spot, BZ_G_HOP, BZ_D_FLASH, false),
    [BZ_MOOD_CAMERA] = MD("Say cheese", b_surprised, d_frame, BZ_G_NONE, BZ_D_FLASH, false),
    [BZ_MOOD_POCKET] = M("Pocket mode", b_pocket, BZ_G_DIM, false),
    [BZ_MOOD_FINDME] = MD("Find me", b_findme, d_waves, BZ_G_NONE, BZ_D_NONE, false),
    [BZ_MOOD_UPSIDEDOWN] = MD("Upside down", b_upside, d_mouth, BZ_G_BOB, BZ_D_NONE, false),

    [BZ_MOOD_HALF] = M("Half", b_half, BZ_G_NONE, false),
    [BZ_MOOD_CLOSED] = M("Closed", b_lines, BZ_G_NONE, false),
    [BZ_MOOD_OFF] = {"Off", NULL, 0, NULL, 0, BZ_G_NONE, BZ_D_NONE, false},
    [BZ_MOOD_SNAP1] = M("Snap 1", b_snap1, BZ_G_NONE, false),
    [BZ_MOOD_SNAP2] = M("Snap 2", b_snap2, BZ_G_NONE, false),
    [BZ_MOOD_SNAP3] = M("Snap 3", b_open, BZ_G_NONE, false),
};
