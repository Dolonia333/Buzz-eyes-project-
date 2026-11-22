#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>

#define TAG "BuzzEyes"

typedef enum {
    MoodNeutral,
    MoodHappy,
    MoodMad,
    MoodSad,
    MoodSleepy,
    MoodLove,
    MoodPlayful,
    MoodCount
} Mood;

static const char* mood_names[] = {
    "Neutral",
    "Happy",
    "Mad",
    "Sad",
    "Sleepy",
    "Love",
    "Playful",
};

typedef struct {
    Gui* gui;
    ViewPort* view_port;
    FuriMessageQueue* event_queue;
    Mood mood;
    int8_t look_x;
    bool blink;
    bool running;
    uint32_t last_idle;
} BuzzEyesApp;

static void draw_eye(Canvas* canvas, int x, int y, Mood mood, bool is_right, bool blink) {
    canvas_set_color(canvas, ColorBlack);

    if(blink) {
        canvas_draw_box(canvas, x, y - 1, 12, 2);
        return;
    }

    switch(mood) {
    case MoodNeutral:
        // 12 x 14 so the eyes read on a 128x64 screen
        canvas_draw_rbox(canvas, x, y - 7, 12, 14, 4);
        break;

    case MoodHappy:
        // Upward smile (circle masked by white box)
        canvas_draw_circle(canvas, x + 6, y + 3, 7);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x - 2, y - 5, 16, 8);
        canvas_set_color(canvas, ColorBlack);
        break;

    case MoodMad:
        // Half-circle bottom curve
        canvas_draw_circle(canvas, x + 6, y + 2, 8);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x - 3, y - 8, 18, 9);
        canvas_set_color(canvas, ColorBlack);
        break;

    case MoodSad:
        canvas_draw_circle(canvas, x + 6, y + 3, 7);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x - 2, y - 4, 16, 7);
        canvas_set_color(canvas, ColorBlack);
        break;

    case MoodSleepy:
        canvas_draw_box(canvas, x, y - 2, 14, 4);
        break;

    case MoodLove:
        canvas_draw_circle(canvas, x + 3, y - 2, 4);
        canvas_draw_circle(canvas, x + 9, y - 2, 4);
        canvas_draw_box(canvas, x + 2, y + 1, 8, 6);
        break;

    case MoodPlayful:
        if(is_right) {
            // Wink
            canvas_draw_line(canvas, x, y, x + 12, y);
            canvas_draw_line(canvas, x + 1, y - 1, x + 11, y - 1);
        } else {
            canvas_draw_rbox(canvas, x, y - 6, 12, 12, 4);
        }
        break;

    default:
        canvas_draw_rbox(canvas, x, y - 7, 12, 14, 4);
        break;
    }
}

static void draw_callback(Canvas* canvas, void* context) {
    BuzzEyesApp* app = context;
    canvas_clear(canvas);

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 10, mood_names[app->mood]);

    int eye_y = 37;
    int left_x = 40 + app->look_x;
    int right_x = 76 + app->look_x;

    draw_eye(canvas, left_x, eye_y, app->mood, false, app->blink);
    draw_eye(canvas, right_x, eye_y, app->mood, true, app->blink);
}

static void input_callback(InputEvent* input_event, void* context) {
    BuzzEyesApp* app = context;
    // Short only: Press and Short both fire for one tap, which would skip moods.
    if(input_event->type == InputTypeShort) {
        furi_message_queue_put(app->event_queue, input_event, 0);
    }
}

static BuzzEyesApp* buzz_eyes_app_alloc(void) {
    BuzzEyesApp* app = malloc(sizeof(BuzzEyesApp));
    app->mood = MoodNeutral;
    app->look_x = 0;
    app->blink = false;
    app->running = true;
    app->last_idle = 0;

    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, draw_callback, app);
    view_port_input_callback_set(app->view_port, input_callback, app);

    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    return app;
}

static void buzz_eyes_app_free(BuzzEyesApp* app) {
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_message_queue_free(app->event_queue);
    furi_record_close(RECORD_GUI);
    free(app);
}

int32_t buzz_eyes_app(void* p) {
    UNUSED(p);
    BuzzEyesApp* app = buzz_eyes_app_alloc();

    InputEvent event;
    while(app->running) {
        if(furi_message_queue_get(app->event_queue, &event, 80) == FuriStatusOk) {
            if(event.key == InputKeyBack) {
                app->running = false;
            } else if(event.key == InputKeyOk) {
                app->mood = (app->mood + 1) % MoodCount;
            } else if(event.key == InputKeyLeft) {
                app->look_x = (app->look_x > -6) ? app->look_x - 2 : -6;
            } else if(event.key == InputKeyRight) {
                app->look_x = (app->look_x < 6) ? app->look_x + 2 : 6;
            } else if(event.key == InputKeyUp) {
                app->blink = true;
            } else if(event.key == InputKeyDown) {
                app->blink = false;
            }
        }

        // Idle behavior
        uint32_t now = furi_get_tick();
        if(now - app->last_idle > 2000) {
            app->last_idle = now;
            if((rand() % 4) == 0) {
                app->blink = true;
            } else {
                app->blink = false;
                app->look_x = (rand() % 7) - 3;
            }
        }

        view_port_update(app->view_port);
    }

    buzz_eyes_app_free(app);
    return 0;
}
