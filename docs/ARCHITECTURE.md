# Architecture

The repo has two eye systems:

1. **Signal eyes** (`flipper/buzz_eyes/`): the chosen direction for the product. Portable C, runs on the Flipper Zero today and is built to move to the ESP32.
2. **Browser prototype** (`buzz_eyes_demo.html`): the original parametric eye demo, kept for reference.

Everything else (`tools/`, `docs/expressions/`, `prototype/`) is built from the Signal eyes engine.

## Signal eyes

### Core ideas

1. **Each eye is three vertical bars.** Emotion is bar height, energy is motion, and a small pixel symbol (heart, Zz, !, ?) is added only when the bars can't say it alone.
2. **Faces are data.** `buzz_faces.c` describes every expression as a list of rectangles (bars and symbols) plus which animation each one uses. No drawing code lives there.
3. **The engine returns rectangles.** `bz_engine_render(engine, now_ms, out, max)` returns a list of `BzDrawRect` (x, y, w, h, style, rgb). Any screen library can draw them.
4. **Tiny frames.** A frame is a handful of rectangles, so animations stay smooth without storing images.
5. **No platform code in the engine.** `buzz_faces.*` and `buzz_engine.*` only need `stdint`, `stdbool` and `math.h`.

### Files

| File | Role |
|---|---|
| `buzz_faces.h` | Animation enums, `BzRect`, `BzMood`, and the list of expression IDs |
| `buzz_faces.c` | The `bz_moods` table: every expression's bars, symbols and animations |
| `buzz_engine.h/.c` | Timing, blinks, animations, peak caps, colors. Produces `BzDrawRect` lists |
| `buzz_eyes_app.c` | Flipper-only: drawing with Canvas, buttons, LED, sound, vibration, boot sequence |
| `application.fam` | Flipper app manifest |

### Coordinate system and screens

- Faces are drawn on a **128x64 grid**. Left eye bars start at x=32, right eye at x=80. Bars are 4 px wide with a 2 px gap, so the eyes sit 48 px apart.
- **Flipper Zero:** 128x64 monochrome, drawn as-is. `rgb` is ignored.
- **Product screen:** 1.27" 128x96 full-color OLED (SSD1351). Add `BZ_FACE_OFFSET_Y` (16) to every y so the face sits in the middle, with room above and below for symbols.

### Animation layers

Each expression combines three kinds of motion:

| Layer | Field | Examples |
|---|---|---|
| Per-bar | `BzRect.anim` + `slot` | blink, equalizer (`EQ`, `EQ_FAST`), zig, build, drop, dot, fill |
| Whole face | `BzMood.group_anim` | look, hop, bob, pulse, pop, shake, shiver, tilt, dim, glitch |
| Symbols | `BzMood.deco_anim` | float, flash, tear, bob |

- `slot` staggers per-bar animations by 150 ms each, which makes the equalizer ripple and the snap-on build left to right.
- **Blinks** are shared and random every 3 to 5 s: 40 ms half, 60 ms closed, 40 ms half.
- **Peak caps** (`BzMood.caps`) sit above equalizer bars, jump up with them and fall back at about 14 px/s.

### Color

`bz_mood_color(mood)` gives each mood its color, copied onto every rectangle's `rgb`:

| Color | Hex | Moods |
|---|---|---|
| Signal Blue | `#6ADCFF` | default (idle, listening, thinking, alert, snap on...) |
| Sun | `#FFD166` | happy, excited, wink, smug, my turn, say cheese |
| Coral | `#FF7A70` | love, shy |
| Red | `#FF5A4E` | angry, glitch, low battery |
| Lavender | `#9C8CFF` | sad, scared, sleepy, bored, pocket mode |
| Mint | `#7CFFB2` | charging |
| Rainbow | moving hue | vibing, dizzy |

For RGB565 panels: `((rgb >> 8) & 0xF800) | ((rgb >> 5) & 0x07E0) | ((rgb >> 3) & 0x001F)`.

### Snap-on boot

The app steps through static frames `OFF`, `SNAP1`, `SNAP2`, `SNAP3`, then `HAPPY`, then `IDLE` (0, 80, 200, 380, 600 and 1800 ms), with a rising chirp and the LED flash. These helper frames come after `BZ_BROWSABLE_COUNT` so they don't show up when browsing.

### Tools built on the engine

- `tools/frame_dump.c` renders frames as RGB on the 128x96 panel.
- `tools/export_expressions.py` compiles it and writes every GIF in `docs/expressions/`, the boot GIF and the contact sheet.
- `prototype/take-buzz-apart.html` carries a JavaScript copy of the faces and engine. **If you change a face in C, update the same face there too.**

### Porting to the ESP32

1. Copy `buzz_faces.*` and `buzz_engine.*` unchanged.
2. Call `bz_engine_render()` each frame with `millis()`.
3. Draw each rectangle: `BAR` as a filled box with 1 px rounded corners, `BOX` filled, `FRAME` outlined, at `y + BZ_FACE_OFFSET_Y` in its `rgb`.
4. Map sensors to `bz_engine_set_mood()` (IMU, hall sensor, phone events over Bluetooth).

## Browser prototype (parametric eyes)

The demo is a single self-contained HTML file. All logic lives in the `<script>` block.

### Core ideas

1. **Moods are data**  
   Each mood is an object with numeric parameters. Changing a mood means setting a new target object. The renderer never hard-codes mood names except when choosing a special shape.

2. **Lerp toward target**  
   Every frame, `current` values ease toward `target`. This gives smooth morphs without keyframes.

3. **Shape selection by thresholds**  
   Inside `drawEye`, the code looks at `topPinch` and `bottomPinch` (and sometimes height) to decide which path to draw: half-circle, heart, smile arc, wink, thin bar, or rounded rectangle.

4. **Idle is separate from mood**  
   Gaze offset, lean scale, and blink are applied on top of the current mood so the character keeps moving even when the mood is fixed.

### Mood parameters

| Parameter     | Meaning                                      | Typical range |
|---------------|----------------------------------------------|---------------|
| `eyeW`        | Eye width relative to screen width           | 0.07 – 0.24   |
| `eyeH`        | Eye height relative to screen height         | 0.06 – 0.42   |
| `radius`      | Corner roundness (used by rounded-rect path) | 0.01 – 0.55   |
| `gap`         | Space between the two eyes                   | 0.07 – 0.32   |
| `yOffset`     | Vertical shift of both eyes                  | -0.08 – 0.12  |
| `angle`       | Rotation in degrees (eyes tilt opposite ways)| -32 – 22      |
| `topPinch`    | How much the top is compressed (mad/sad)     | 0 – 0.75      |
| `bottomPinch` | How much the bottom is pinched (happy/love)  | 0 – 0.58      |

### Shape rules (simplified)

- `topPinch > 0.55` → mad (sharp half-circle)
- `topPinch > 0.28` → sad (softer droop)
- `bottomPinch > 0.5` → love (hearts)
- `bottomPinch > 0.42` → happy (upward smile arc)
- `bottomPinch > 0.28` → playful (asymmetric wink)
- `bottomPinch > 0.18` → cute (softer hearts)
- `eyeH` very small → sleepy (thin bar)
- otherwise → rounded rectangle / capsule (neutral, surprised, focused, etc.)

Exact thresholds are in the demo. Change them carefully so moods don't collide.

### Idle systems

- **Gaze**: random left/right targets with slow lerp. Small independent offset per eye.
- **Lean**: occasionally scales one eye larger to feel like the character is leaning closer.
- **Breath / look scale**: very subtle size change based on how far the eyes are looking.
- **Blink**: probabilistic, skipped when `bottomPinch` is high (happy/love).

### Screen and device

`SCREEN` defines the light-blue display rectangle. The rest of the device (body, antenna, keypad) is drawn around it for visual context only. The eyes are always positioned relative to `SCREEN`.

### Porting notes

To move this to hardware:

1. Replace `roundedRect`, `arc`, `ellipse`, and `bezierCurveTo` with whatever your graphics library provides.
2. Keep the same mood objects and lerp logic.
3. The shape branches can stay as-is; just translate the path commands.
4. Idle timers can become simple counters or use the same `requestAnimationFrame`-style loop.

The goal is that a port only has to reimplement the drawing primitives, not redesign the personality system.
