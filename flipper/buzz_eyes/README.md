# Buzz Eyes for Flipper Zero

A Flipper Zero app that plays Buzz's **Signal eyes** on the Flipper's 128x64 screen. That's the same resolution as the 1.3" SH1106 OLED planned for the real device, so every face here maps to the hardware pixel for pixel.

Each eye is three vertical bars. Emotion comes from the bar heights, and a small pixel symbol only shows up when the bars can't say it alone.

## What's in it

- **24 expressions:** Idle, Happy, Excited, Love, Listening, Vibing, Surprised, Curious, Thinking, Confused, Sleepy, Bored, Sad, Angry, Scared, Dizzy, Focused, Wink, Shy, Smug, Alert, Charging, Low battery, Glitch
- **8 MagSafe moments:** Snap on, Bye for now, Slipping, My turn, Say cheese, Pocket mode, Find me, Upside down
- **Snap-on boot:** plays when the app opens. The bars light up one by one like a phone finding signal, with a rising chirp and the LED flashing like the antenna bead.
- **Equalizer peak caps** on Listening and Vibing that jump up with the bars and fall back slowly
- **Random blinks** every 3 to 5 s (40 ms half, 60 ms closed, 40 ms half)
- **Flipper extras:** LED glow on Alert and Love, vibration on Angry and Scared, chirp on Snap on and Find me

## Controls

| Button | Action |
|---|---|
| Left / Right | Previous / next expression |
| OK | Replay the snap-on boot |
| Up | Auto demo on/off (new expression every 4 s) |
| Down | OLED look on/off (lit pixels on black vs. dark pixels on orange) |
| Back | Exit |

## Build and install

You need [ufbt](https://github.com/flipperdevices/flipperzero-ufbt), the official Flipper app build tool.

```bash
pip install ufbt
cd flipper/buzz_eyes
ufbt            # builds dist/buzz_eyes.fap
ufbt launch     # builds, installs and opens it on a Flipper plugged in over USB
```

Or copy `dist/buzz_eyes.fap` to `apps/Media/` on the Flipper's SD card with qFlipper, then open it from **Apps > Media > Buzz Eyes**.

## Files

| File | What it does |
|---|---|
| `buzz_faces.h / .c` | Every expression as data: bar positions, heights and which animation each one uses |
| `buzz_engine.h / .c` | The animation engine. Give it the time in ms, get back a list of rectangles to draw |
| `buzz_eyes_app.c` | Flipper-only code: screen drawing, buttons, LED, sound, vibration |
| `application.fam` | Flipper app manifest |

`buzz_faces` and `buzz_engine` are plain C with no Flipper code in them, so they can move to the ESP32 as-is. On the ESP32 you only rewrite the drawing part, for example with U8g2:

```c
BzDrawRect rects[BZ_MAX_DRAW];
uint8_t n = bz_engine_render(&engine, millis(), rects, BZ_MAX_DRAW);
u8g2.clearBuffer();
for (uint8_t i = 0; i < n; i++) {
    const BzDrawRect* r = &rects[i];
    if (r->style == BZ_STYLE_BAR) u8g2.drawRBox(r->x, r->y, r->w, r->h, 1);
    else if (r->style == BZ_STYLE_FRAME) u8g2.drawFrame(r->x, r->y, r->w, r->h);
    else u8g2.drawBox(r->x, r->y, r->w, r->h);
}
u8g2.sendBuffer();
```

(Clip negative coordinates first, like `draw_rect` in `buzz_eyes_app.c` does.)

## Adding an expression

1. Add an ID to `BzMoodId` in `buzz_faces.h`, before `BZ_MOOD_HALF` so you can browse to it.
2. Add its bars (and pixel symbols, if any) in `buzz_faces.c`. `EYE(x, y1, h1, y2, h2, y3, h3, anim, slot)` makes one three-bar eye. The left eye starts at x=32 and the right at x=80.
3. Add a row to the `bz_moods` table with its name and animations.
4. Optional: give it a sound, light or vibration in `play_effects` in `buzz_eyes_app.c`.

## Notes

- The Flipper screen is an orange-backlit LCD, not an OLED. Use it to judge timing and motion. Wire up the real SH1106 OLED to judge the glow.
- The Flipper has no motion or magnet sensors, so the MagSafe moments are browsed with the buttons for now. On Buzz they'd be triggered by the IMU and a hall-effect sensor.
