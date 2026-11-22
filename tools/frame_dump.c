/*
 * Dumps Buzz eye frames as raw 1-bit pixels so docs can show real previews.
 * Uses the exact same engine as the Flipper app.
 *
 * Usage: frame_dump <mood_index> <duration_ms> <fps>
 * Output: frames of 128*96*3 bytes (RGB, the color panel) on stdout:
 * the mood's screen color with black eyes.
 * Run with "list" to print "index<TAB>name" for every expression.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buzz_engine.h"

static unsigned char fb[BZ_PANEL_H][BZ_PANEL_W][3];

static void plot(const BzDrawRect* r) {
    for(int y = r->y; y < r->y + r->h; y++) {
        for(int x = r->x; x < r->x + r->w; x++) {
            int py = y + BZ_FACE_OFFSET_Y;
            if(x < 0 || x >= BZ_PANEL_W || py < 0 || py >= BZ_PANEL_H) continue;
            int edge = y == r->y || y == r->y + r->h - 1 || x == r->x || x == r->x + r->w - 1;
            if(r->style == BZ_STYLE_FRAME && !edge) continue;
            /* Round the corners of eye bars the same way canvas_draw_rbox(radius 1) does. */
            if(r->style == BZ_STYLE_BAR && r->w >= 3 && r->h >= 3) {
                int cx = (x == r->x || x == r->x + r->w - 1);
                int cy = (y == r->y || y == r->y + r->h - 1);
                if(cx && cy) continue;
            }
            fb[py][x][0] = (unsigned char)(r->rgb >> 16);
            fb[py][x][1] = (unsigned char)(r->rgb >> 8);
            fb[py][x][2] = (unsigned char)r->rgb;
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
        uint32_t bg = bz_engine_screen_rgb(&engine, (uint32_t)t);
        for(int y = 0; y < BZ_PANEL_H; y++)
            for(int x = 0; x < BZ_PANEL_W; x++) {
                fb[y][x][0] = (unsigned char)(bg >> 16);
                fb[y][x][1] = (unsigned char)(bg >> 8);
                fb[y][x][2] = (unsigned char)bg;
            }
        for(uint8_t i = 0; i < n; i++) plot(&rects[i]);
        fwrite(fb, 1, sizeof(fb), stdout);
    }
    return 0;
}
