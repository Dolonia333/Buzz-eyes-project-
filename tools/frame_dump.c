/*
 * Dumps Buzz eye frames as raw 1-bit pixels so docs can show real previews.
 * Uses the exact same engine as the Flipper app.
 *
 * Usage: frame_dump <mood_index> <duration_ms> <fps>
 * Output: frames of 128*64 bytes (0 = off, 1 = lit) on stdout.
 * Run with "list" to print "index<TAB>name" for every expression.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buzz_engine.h"

static unsigned char fb[BZ_SCREEN_H][BZ_SCREEN_W];

static void plot(const BzDrawRect* r) {
    for(int y = r->y; y < r->y + r->h; y++) {
        for(int x = r->x; x < r->x + r->w; x++) {
            if(x < 0 || x >= BZ_SCREEN_W || y < 0 || y >= BZ_SCREEN_H) continue;
            int edge = y == r->y || y == r->y + r->h - 1 || x == r->x || x == r->x + r->w - 1;
            if(r->style == BZ_STYLE_FRAME && !edge) continue;
            /* Round the corners of eye bars the same way canvas_draw_rbox(radius 1) does. */
            if(r->style == BZ_STYLE_BAR && r->w >= 3 && r->h >= 3) {
                int cx = (x == r->x || x == r->x + r->w - 1);
                int cy = (y == r->y || y == r->y + r->h - 1);
                if(cx && cy) continue;
            }
            fb[y][x] = 1;
        }
    }
}

int main(int argc, char** argv) {
    if(argc >= 2 && strcmp(argv[1], "list") == 0) {
        for(int i = 0; i < BZ_MOOD_COUNT; i++) printf("%d\t%s\n", i, bz_moods[i].name);
        return 0;
    }
    if(argc < 4) {
        fprintf(stderr, "usage: frame_dump <mood_index> <duration_ms> <fps>\n");
        return 1;
    }
    int mood = atoi(argv[1]);
    int duration = atoi(argv[2]);
    int fps = atoi(argv[3]);
    int step = 1000 / fps;

    BzEngine engine;
    bz_engine_init(&engine, 0);
    bz_engine_set_mood(&engine, (uint8_t)mood, 0);

    BzDrawRect rects[BZ_MAX_DRAW];
    for(int t = 0; t < duration; t += step) {
        uint8_t n = bz_engine_render(&engine, (uint32_t)t, rects, BZ_MAX_DRAW);
        memset(fb, 0, sizeof(fb));
        for(uint8_t i = 0; i < n; i++) plot(&rects[i]);
        fwrite(fb, 1, sizeof(fb), stdout);
    }
    return 0;
}
