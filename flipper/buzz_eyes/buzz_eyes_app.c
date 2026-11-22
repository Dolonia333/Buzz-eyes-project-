/*
 * Buzz Eyes for Flipper Zero
 *
 * Plays Buzz's Signal-eye expressions on the Flipper's 128x64 screen, the
 * same resolution as the 1.3" SH1106 OLED planned for the real device.
 *
 * Controls
 *   Left / Right  previous / next expression
 *   OK            replay the snap-on boot sequence
 *   Up            auto demo on/off (new expression every 4 s)
 *   Down          invert (dark eyes on the lit screen is the default, like Buzz)
 *   Back          exit
 */

#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#include "buzz_engine.h"

#define FRAME_MS 33 /* ~30 fps */
#define LABEL_MS 1500
#define AUTO_MS 4000

/* Snap-on boot: dark, 1 bar, 1 eye, both eyes, then a happy hello. */
#define BOOT_SNAP1_MS 80
#define BOOT_SNAP2_MS 200
#define BOOT_SNAP3_MS 380
#define BOOT_HAPPY_MS 600
#define BOOT_DONE_MS 1800

typedef struct {
    FuriMutex* mutex;
    FuriMessageQueue* input_queue;
    ViewPort* view_port;
    Gui* gui;
    NotificationApp* notifications;
    FuriTimer* frame_timer;

    BzEngine engine;
    uint8_t selected; /* index into the browsable expressions */

    bool booting;
    uint32_t boot_start_ms;

    bool auto_demo;
    uint32_t auto_last_ms;

    bool oled_look;

    char label[24];
    uint32_t label_until_ms;
} BuzzApp;

/* ---------- sound, light, vibration ---------- */

/* Rising chirp with the LED as the antenna bead: Buzz's "hello". */
static const NotificationSequence seq_snap_chirp = {
    &message_blue_255,
    &message_note_c5,
    &message_delay_50,
    &message_note_e5,
    &message_delay_50,
    &message_note_g5,
    &message_delay_50,
    &message_note_c6,
    &message_delay_100,
    &message_sound_off,
    &message_blue_0,
    NULL,
};

static const NotificationSequence seq_antenna_flash = {
    &message_blue_255,
    &message_green_255,
    &message_delay_100,
    &message_blue_0,
    &message_green_0,
    &message_delay_100,
    &message_blue_255,
    &message_green_255,
    &message_delay_100,
    &message_blue_0,
    &message_green_0,
    NULL,
};

static const NotificationSequence seq_love_glow = {
    &message_red_255,
    &message_blue_255,
    &message_delay_250,
    &message_red_0,
    &message_blue_0,
    NULL,
};

static void play_effects(BuzzApp* app, uint8_t mood) {
    switch(mood) {
    case BZ_MOOD_SNAPON:
    case BZ_MOOD_FINDME:
        notification_message(app->notifications, &seq_snap_chirp);
        break;
    case BZ_MOOD_NOTIFY:
        notification_message(app->notifications, &seq_antenna_flash);
        break;
    case BZ_MOOD_LOVE:
        notification_message(app->notifications, &seq_love_glow);
        break;
    case BZ_MOOD_ANGRY:
    case BZ_MOOD_SCARED:
        notification_message(app->notifications, &sequence_single_vibro);
        break;
    default:
        break;
    }
}

/* ---------- state changes (call with mutex held) ---------- */

static void show_label(BuzzApp* app, const char* text, uint32_t now) {
    strlcpy(app->label, text, sizeof(app->label));
    app->label_until_ms = now + LABEL_MS;
}

static void select_mood(BuzzApp* app, uint8_t index, uint32_t now) {
    app->selected = index % BZ_BROWSABLE_COUNT;
    app->booting = false;
    bz_engine_set_mood(&app->engine, app->selected, now);
    show_label(app, bz_moods[app->selected].name, now);
    play_effects(app, app->selected);
}

static void start_boot(BuzzApp* app, uint32_t now) {
    app->booting = true;
    app->boot_start_ms = now;
    bz_engine_set_mood(&app->engine, BZ_MOOD_OFF, now);
    notification_message(app->notifications, &seq_snap_chirp);
}

static void update_boot(BuzzApp* app, uint32_t now) {
    if(!app->booting) return;
    uint32_t e = now - app->boot_start_ms;
    uint8_t want = BZ_MOOD_OFF;
    if(e >= BOOT_DONE_MS) {
        app->booting = false;
        app->selected = BZ_MOOD_IDLE;
        bz_engine_set_mood(&app->engine, BZ_MOOD_IDLE, now);
        return;
    } else if(e >= BOOT_HAPPY_MS) {
        want = BZ_MOOD_HAPPY;
    } else if(e >= BOOT_SNAP3_MS) {
        want = BZ_MOOD_SNAP3;
    } else if(e >= BOOT_SNAP2_MS) {
        want = BZ_MOOD_SNAP2;
    } else if(e >= BOOT_SNAP1_MS) {
        want = BZ_MOOD_SNAP1;
    }
    if(app->engine.mood != want) bz_engine_set_mood(&app->engine, want, now);
}

/* ---------- drawing ---------- */

static void draw_rect(Canvas* canvas, const BzDrawRect* r) {
    /* Clip to the screen so negative coordinates never reach the canvas. */
    int32_t x = r->x, y = r->y, w = r->w, h = r->h;
    if(x < 0) {
        w += x;
        x = 0;
    }
    if(y < 0) {
        h += y;
        y = 0;
    }
    if(x + w > BZ_SCREEN_W) w = BZ_SCREEN_W - x;
    if(y + h > BZ_SCREEN_H) h = BZ_SCREEN_H - y;
    if(w <= 0 || h <= 0) return;

    switch(r->style) {
    case BZ_STYLE_BAR:
        if(w >= 3 && h >= 3) {
            canvas_draw_rbox(canvas, x, y, w, h, 1);
        } else {
            canvas_draw_box(canvas, x, y, w, h);
        }
        break;
    case BZ_STYLE_FRAME:
        if(w >= 2 && h >= 2) {
            canvas_draw_frame(canvas, x, y, w, h);
        } else {
            canvas_draw_box(canvas, x, y, w, h);
        }
        break;
    default:
        canvas_draw_box(canvas, x, y, w, h);
        break;
    }
}

static void draw_callback(Canvas* canvas, void* ctx) {
    BuzzApp* app = ctx;
    if(furi_mutex_acquire(app->mutex, 25) != FuriStatusOk) return;

    uint32_t now = furi_get_tick();
    BzDrawRect rects[BZ_MAX_DRAW];
    uint8_t count = bz_engine_render(&app->engine, now, rects, BZ_MAX_DRAW);

    Color bg = app->oled_look ? ColorBlack : ColorWhite;
    Color fg = app->oled_look ? ColorWhite : ColorBlack;

    canvas_clear(canvas);
    canvas_set_color(canvas, bg);
    canvas_draw_box(canvas, 0, 0, BZ_SCREEN_W, BZ_SCREEN_H);
    canvas_set_color(canvas, fg);
    for(uint8_t i = 0; i < count; i++) {
        draw_rect(canvas, &rects[i]);
    }

    /* Name tag along the bottom after switching. */
    if((int32_t)(app->label_until_ms - now) > 0) {
        canvas_set_font(canvas, FontSecondary);
        canvas_set_color(canvas, fg);
        canvas_draw_box(canvas, 0, 54, BZ_SCREEN_W, 10);
        canvas_set_color(canvas, bg);
        canvas_draw_str_aligned(canvas, 64, 63, AlignCenter, AlignBottom, app->label);
    }

    furi_mutex_release(app->mutex);
}

static void input_callback(InputEvent* event, void* ctx) {
    FuriMessageQueue* queue = ctx;
    furi_message_queue_put(queue, event, FuriWaitForever);
}

static void frame_timer_callback(void* ctx) {
    BuzzApp* app = ctx;
    view_port_update(app->view_port);
}

/* ---------- app lifecycle ---------- */

static BuzzApp* buzz_app_alloc(void) {
    BuzzApp* app = malloc(sizeof(BuzzApp));
    memset(app, 0, sizeof(BuzzApp));

    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    app->input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->oled_look = false; /* dark eyes on a lit screen, like Buzz's blue screen */

    uint32_t now = furi_get_tick();
    bz_engine_init(&app->engine, now);

    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, draw_callback, app);
    view_port_input_callback_set(app->view_port, input_callback, app->input_queue);

    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    notification_message(app->notifications, &sequence_display_backlight_enforce_on);

    app->frame_timer = furi_timer_alloc(frame_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->frame_timer, furi_ms_to_ticks(FRAME_MS));

    return app;
}

static void buzz_app_free(BuzzApp* app) {
    furi_timer_stop(app->frame_timer);
    furi_timer_free(app->frame_timer);

    notification_message(app->notifications, &sequence_display_backlight_enforce_auto);
    notification_message(app->notifications, &sequence_reset_rgb);
    furi_record_close(RECORD_NOTIFICATION);

    gui_remove_view_port(app->gui, app->view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(app->view_port);

    furi_message_queue_free(app->input_queue);
    furi_mutex_free(app->mutex);
    free(app);
}

int32_t buzz_eyes_app(void* p) {
    UNUSED(p);
    BuzzApp* app = buzz_app_alloc();

    furi_mutex_acquire(app->mutex, FuriWaitForever);
    start_boot(app, furi_get_tick());
    furi_mutex_release(app->mutex);

    InputEvent event;
    bool running = true;
    while(running) {
        /* Short timeout so the boot sequence and auto demo stay on time. */
        bool got_input = furi_message_queue_get(app->input_queue, &event, 20) == FuriStatusOk;

        furi_mutex_acquire(app->mutex, FuriWaitForever);
        uint32_t now = furi_get_tick();

        if(got_input && (event.type == InputTypeShort || event.type == InputTypeRepeat)) {
            switch(event.key) {
            case InputKeyRight:
                select_mood(app, (uint8_t)(app->selected + 1), now);
                app->auto_last_ms = now;
                break;
            case InputKeyLeft:
                select_mood(app, (uint8_t)(app->selected + BZ_BROWSABLE_COUNT - 1), now);
                app->auto_last_ms = now;
                break;
            case InputKeyOk:
                if(event.type == InputTypeShort) start_boot(app, now);
                break;
            case InputKeyUp:
                if(event.type == InputTypeShort) {
                    app->auto_demo = !app->auto_demo;
                    app->auto_last_ms = now;
                    show_label(app, app->auto_demo ? "Auto demo: on" : "Auto demo: off", now);
                }
                break;
            case InputKeyDown:
                if(event.type == InputTypeShort) {
                    app->oled_look = !app->oled_look;
                    show_label(app, app->oled_look ? "Inverted" : "Buzz look", now);
                }
                break;
            case InputKeyBack:
                if(event.type == InputTypeShort) running = false;
                break;
            default:
                break;
            }
        }

        update_boot(app, now);

        if(app->auto_demo && !app->booting && now - app->auto_last_ms >= AUTO_MS) {
            select_mood(app, (uint8_t)(app->selected + 1), now);
            app->auto_last_ms = now;
        }

        furi_mutex_release(app->mutex);
    }

    buzz_app_free(app);
    return 0;
}
