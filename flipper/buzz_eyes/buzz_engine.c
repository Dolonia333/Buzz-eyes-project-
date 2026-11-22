#include "buzz_engine.h"

#include <math.h>

#define BZ_PI 3.14159265f
#define CX (BZ_SCREEN_W / 2)
#define CY (BZ_SCREEN_H / 2)

/* ---------- helpers ---------- */

static uint32_t rng_next(BzEngine* e) {
    /* xorshift32: tiny and good enough for blink timing */
    uint32_t x = e->rng;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    e->rng = x ? x : 0x1234567u;
    return e->rng;
}

/* 0..1 position inside a looping animation, with an optional start delay. */
static float phase(uint32_t t, uint32_t period_ms, uint32_t delay_ms) {
    uint32_t shifted = t + period_ms * 64u - (delay_ms % period_ms);
    return (float)(shifted % period_ms) / (float)period_ms;
}

/* Piecewise linear keyframes, like a CSS animation with linear easing. */
static float keys(float p, const float* kx, const float* ky, int n) {
    if(p <= kx[0]) return ky[0];
    for(int i = 1; i < n; i++) {
        if(p <= kx[i]) {
            float span = kx[i] - kx[i - 1];
            float k = span > 0.0f ? (p - kx[i - 1]) / span : 1.0f;
            return ky[i - 1] + (ky[i] - ky[i - 1]) * k;
        }
    }
    return ky[n - 1];
}

static int16_t round_i16(float v) {
    return (int16_t)(v >= 0.0f ? v + 0.5f : v - 0.5f);
}

static float blink_scale(BzEngine* e, uint32_t now) {
    if(!e->blinking && (int32_t)(now - e->next_blink_ms) >= 0) {
        e->blinking = true;
        e->blink_start_ms = now;
    }
    if(!e->blinking) return 1.0f;

    uint32_t b = now - e->blink_start_ms;
    if(b < 40) return 1.0f - 0.6f * (float)b / 40.0f; /* to half */
    if(b < 100) return 0.12f; /* closed */
    if(b < 140) return 0.4f + 0.6f * (float)(b - 100) / 40.0f; /* back open */

    e->blinking = false;
    e->next_blink_ms = now + 3000u + (rng_next(e) % 2000u);
    return 1.0f;
}

/* ---------- color ---------- */

#define C_BLUE 0x6ADCFFu /* Signal Blue: default */
#define C_SUN 0xFFD166u /* happy moments */
#define C_CORAL 0xFF7A70u /* love, shy */
#define C_RED 0xFF5A4Eu /* angry, errors */
#define C_LAV 0x9C8CFFu /* sad, sleepy, quiet */
#define C_MINT 0x7CFFB2u /* charging */

uint32_t bz_mood_color(uint8_t mood) {
    switch(mood) {
    case BZ_MOOD_HAPPY:
    case BZ_MOOD_EXCITED:
    case BZ_MOOD_WINK:
    case BZ_MOOD_SMUG:
    case BZ_MOOD_SPOTLIGHT:
    case BZ_MOOD_CAMERA:
        return C_SUN;
    case BZ_MOOD_LOVE:
    case BZ_MOOD_SHY:
        return C_CORAL;
    case BZ_MOOD_ANGRY:
    case BZ_MOOD_GLITCH:
    case BZ_MOOD_LOWBAT:
        return C_RED;
    case BZ_MOOD_SAD:
    case BZ_MOOD_SCARED:
    case BZ_MOOD_SLEEPY:
    case BZ_MOOD_BORED:
    case BZ_MOOD_POCKET:
        return C_LAV;
    case BZ_MOOD_CHARGING:
        return C_MINT;
    default:
        return C_BLUE;
    }
}

/* Hue (0-360) at full saturation, 65% value-ish, for the rainbow moods. */
static uint32_t hue_rgb(float hue) {
    float h = hue / 60.0f;
    int i = (int)h;
    float f = h - (float)i;
    float hi = 255.0f, lo = 90.0f;
    float up = lo + (hi - lo) * f, down = hi - (hi - lo) * f;
    float r, g, b;
    switch(i % 6) {
    case 0: r = hi; g = up; b = lo; break;
    case 1: r = down; g = hi; b = lo; break;
    case 2: r = lo; g = hi; b = up; break;
    case 3: r = lo; g = down; b = hi; break;
    case 4: r = up; g = lo; b = hi; break;
    default: r = hi; g = lo; b = down; break;
    }
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

/* ---------- public ---------- */

void bz_engine_init(BzEngine* engine, uint32_t now_ms) {
    engine->rng = now_ms ^ 0xB0225u;
    engine->blinking = false;
    engine->next_blink_ms = now_ms + 2500u;
    bz_engine_set_mood(engine, BZ_MOOD_IDLE, now_ms);
}

void bz_engine_set_mood(BzEngine* engine, uint8_t mood, uint32_t now_ms) {
    if(mood >= BZ_MOOD_COUNT) mood = BZ_MOOD_IDLE;
    engine->mood = mood;
    engine->mood_start_ms = now_ms;
    engine->caps_ready = false;
    engine->last_render_ms = now_ms;
}

typedef struct {
    float dx;
    float dy;
    float scale;
    float shear; /* vertical offset per px of distance from center (tilt) */
    bool dim;
} GroupXf;

static GroupXf group_transform(uint8_t anim, uint32_t t) {
    GroupXf g = {0.0f, 0.0f, 1.0f, 0.0f, false};
    float p;
    switch(anim) {
    case BZ_G_LOOK: {
        static const float kx[] = {0.0f, 0.30f, 0.40f, 0.55f, 0.65f, 0.80f, 1.0f};
        static const float ky[] = {0.0f, 0.0f, -5.0f, -5.0f, 5.0f, 5.0f, 0.0f};
        g.dx = keys(phase(t, 6000, 0), kx, ky, 7);
        break;
    }
    case BZ_G_HOP:
        g.dy = -2.5f * sinf(BZ_PI * phase(t, 1400, 0));
        break;
    case BZ_G_HOP_FAST:
        g.dy = -2.5f * sinf(BZ_PI * phase(t, 550, 0));
        break;
    case BZ_G_BOB:
        g.dy = -1.6f * sinf(BZ_PI * phase(t, 1800, 0));
        break;
    case BZ_G_PULSE: {
        static const float kx[] = {0.0f, 0.15f, 0.30f, 0.45f, 1.0f};
        static const float ky[] = {1.0f, 1.07f, 1.0f, 1.05f, 1.0f};
        g.scale = keys(phase(t, 1200, 0), kx, ky, 5);
        break;
    }
    case BZ_G_POP:
        g.scale = 1.0f + 0.05f * sinf(BZ_PI * phase(t, 1600, 0));
        break;
    case BZ_G_SHAKE: {
        static const float kx[] = {0.0f, 0.25f, 0.75f, 1.0f};
        static const float ky[] = {0.0f, -2.0f, 2.0f, 0.0f};
        g.dx = keys(phase(t, 350, 0), kx, ky, 4);
        break;
    }
    case BZ_G_SHIVER:
        g.dx = (phase(t, 120, 0) < 0.5f) ? -1.0f : 1.0f;
        break;
    case BZ_G_TILT: {
        /* Shear instead of rotate: cheap on a 1-bit screen and reads the same. */
        float angle = -6.0f * cosf(2.0f * BZ_PI * phase(t, 1100, 0));
        g.shear = tanf(angle * BZ_PI / 180.0f);
        break;
    }
    case BZ_G_DIM:
        g.dim = true;
        break;
    case BZ_G_GLITCH:
        p = phase(t, 1300, 0);
        if(p >= 0.20f && p < 0.24f) {
            g.dx = 4.0f;
        } else if(p >= 0.24f && p < 0.28f) {
            g.dx = -3.0f;
        } else if(p >= 0.70f && p < 0.74f) {
            g.dx = -4.0f;
            g.dy = 1.0f;
        }
        break;
    default:
        break;
    }
    return g;
}

/* How one bar is stretched/moved by its own animation. */
static bool bar_anim(BzEngine* e, const BzRect* r, uint32_t t, float blink, float* sy, float* oy) {
    *sy = 1.0f;
    *oy = 0.0f;
    uint32_t delay = (uint32_t)r->slot * 150u;

    switch(r->anim) {
    case BZ_A_BLINK:
        *sy = blink;
        break;
    case BZ_A_EQ:
    case BZ_A_EQ_FAST: {
        static const float kx[] = {0.0f, 0.35f, 0.70f, 1.0f};
        static const float ky[] = {1.0f, 0.45f, 1.15f, 1.0f};
        uint32_t period = (r->anim == BZ_A_EQ) ? 900u : 450u;
        *sy = keys(phase(t, period, delay), kx, ky, 4);
        break;
    }
    case BZ_A_ZIG: {
        float p = phase(t, 700, (r->slot & 1u) ? 350u : 0u);
        *oy = -0.35f * (float)r->h * cosf(2.0f * BZ_PI * p);
        break;
    }
    case BZ_A_BUILD: {
        static const float kx[] = {0.0f, 0.04f, 0.22f, 0.30f, 0.86f, 1.0f};
        static const float ky[] = {0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 0.0f};
        *sy = keys(phase(t, 2600, delay), kx, ky, 6);
        if(*sy < 0.05f) return false;
        break;
    }
    case BZ_A_DROP: {
        float p = phase(t, 2400, delay);
        if(p < 0.25f) {
            *sy = 1.0f;
        } else if(p < 0.60f) {
            float k = (p - 0.25f) / 0.35f;
            *sy = 1.0f - 0.9f * k;
            *oy = 1.4f * (float)r->h * k * 0.5f;
        } else if(p < 0.85f) {
            return false;
        }
        break;
    }
    default:
        break;
    }
    (void)e;
    return true;
}

static uint32_t g_color;

static bool push(BzDrawRect* out, uint8_t* n, uint8_t max_out, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t style) {
    if(*n >= max_out || w <= 0 || h <= 0) return false;
    out[*n].x = x;
    out[*n].y = y;
    out[*n].w = w;
    out[*n].h = h;
    out[*n].style = style;
    out[*n].rgb = g_color;
    (*n)++;
    return true;
}

uint8_t bz_engine_render(BzEngine* e, uint32_t now, BzDrawRect* out, uint8_t max_out) {
    const BzMood* m = &bz_moods[e->mood];
    uint32_t t = now - e->mood_start_ms;
    uint32_t dt = now - e->last_render_ms;
    e->last_render_ms = now;
    uint8_t n = 0;

    uint32_t base = bz_mood_color(e->mood);
    bool rainbow = (e->mood == BZ_MOOD_MUSIC || e->mood == BZ_MOOD_DIZZY);
    g_color = base;
    float blink = blink_scale(e, now);
    GroupXf g = group_transform(m->group_anim, t);

    /* ----- eye bars ----- */
    for(uint8_t i = 0; i < m->bar_count; i++) {
        const BzRect* r = &m->bars[i];
        float sy, oy;
        if(!bar_anim(e, r, t, blink, &sy, &oy)) continue;

        /* Scale each bar around its own center, then apply the face motion. */
        float h = (float)r->h * sy;
        if(h < 1.0f) h = 1.0f;
        float y = (float)r->y + ((float)r->h - h) * 0.5f + oy;
        float bar_cx = (float)r->x + (float)r->w * 0.5f;

        float x2 = CX + ((float)r->x - CX) * g.scale + g.dx;
        float y2 = CY + (y - CY) * g.scale + g.dy + g.shear * (bar_cx - CX);
        int16_t w2 = round_i16((float)r->w * g.scale);
        int16_t h2 = round_i16(h * g.scale);
        if(h2 < 1) h2 = 1;
        int16_t xi = round_i16(x2);
        int16_t yi = round_i16(y2);

        /* Rainbow moods shift hue across the bars and over time. */
        g_color = rainbow ? hue_rgb((float)(((uint32_t)r->x * 3u + t / 8u) % 360u)) : base;
        push(out, &n, max_out, xi, yi, w2, h2, g.dim ? BZ_STYLE_FRAME : BZ_STYLE_BAR);

        /* Peak caps: snap up with the bar, then fall at ~14 px/s. */
        if(m->caps && i < 16) {
            float top = (float)yi - 3.0f;
            if(!e->caps_ready || top < e->cap_y[i]) {
                e->cap_y[i] = top;
            } else {
                e->cap_y[i] += 0.014f * (float)dt;
                if(e->cap_y[i] > top) e->cap_y[i] = top;
            }
            push(out, &n, max_out, xi, round_i16(e->cap_y[i]), w2, 1, BZ_STYLE_BOX);
        }
    }
    if(m->caps) e->caps_ready = true;

    /* ----- pixel symbols ----- */
    g_color = base;
    if(m->deco_count) {
        float ddy = 0.0f;
        bool visible = true;
        float p;
        switch(m->deco_anim) {
        case BZ_D_FLOAT:
            p = phase(t, 2400, 0);
            ddy = 4.0f - 9.0f * p;
            visible = p > 0.08f && p < 0.90f;
            break;
        case BZ_D_FLASH:
            visible = phase(t, 900, 0) < 0.5f;
            break;
        case BZ_D_TEAR:
            p = phase(t, 2200, 0);
            ddy = -2.5f + 10.0f * p;
            visible = p > 0.15f && p < 0.90f;
            break;
        case BZ_D_BOB:
            ddy = -1.6f * sinf(BZ_PI * phase(t, 1800, 0));
            break;
        default:
            break;
        }

        if(visible) {
            int16_t oyi = round_i16(ddy);
            for(uint8_t i = 0; i < m->deco_count; i++) {
                const BzRect* r = &m->deco[i];
                uint8_t style = BZ_STYLE_BOX;
                if(r->anim == BZ_A_DOT) {
                    float dp = phase(t, 1200, (uint32_t)r->slot * 150u);
                    if(!(dp > 0.10f && dp < 0.45f)) style = BZ_STYLE_FRAME;
                } else if(r->anim == BZ_A_FILL) {
                    float fp = phase(t, 1500, (uint32_t)r->slot * 500u);
                    if(fp < 0.33f) style = BZ_STYLE_FRAME;
                }
                push(out, &n, max_out, r->x, (int16_t)(r->y + oyi), r->w, r->h, style);
            }
        }
    }

    return n;
}
