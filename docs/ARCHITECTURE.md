# Architecture

The demo is a single self-contained HTML file. All logic lives in the `<script>` block.

## Core ideas

1. **Moods are data**  
   Each mood is an object with numeric parameters. Changing a mood means setting a new target object. The renderer never hard-codes mood names except when choosing a special shape.

2. **Lerp toward target**  
   Every frame, `current` values ease toward `target`. This gives smooth morphs without keyframes.

3. **Shape selection by thresholds**  
   Inside `drawEye`, the code looks at `topPinch` and `bottomPinch` (and sometimes height) to decide which path to draw: half-circle, heart, smile arc, wink, thin bar, or rounded rectangle.

4. **Idle is separate from mood**  
   Gaze offset, lean scale, and blink are applied on top of the current mood so the character keeps moving even when the mood is fixed.

## Mood parameters

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

## Shape rules (simplified)

- `topPinch > 0.55` → mad (sharp half-circle)
- `topPinch > 0.28` → sad (softer droop)
- `bottomPinch > 0.5` → love (hearts)
- `bottomPinch > 0.42` → happy (upward smile arc)
- `bottomPinch > 0.28` → playful (asymmetric wink)
- `bottomPinch > 0.18` → cute (softer hearts)
- `eyeH` very small → sleepy (thin bar)
- otherwise → rounded rectangle / capsule (neutral, surprised, focused, etc.)

Exact thresholds are in the demo. Change them carefully so moods don't collide.

## Idle systems

- **Gaze**: random left/right targets with slow lerp. Small independent offset per eye.
- **Lean**: occasionally scales one eye larger to feel like the character is leaning closer.
- **Breath / look scale**: very subtle size change based on how far the eyes are looking.
- **Blink**: probabilistic, skipped when `bottomPinch` is high (happy/love).

## Screen and device

`SCREEN` defines the light-blue display rectangle. The rest of the device (body, antenna, keypad) is drawn around it for visual context only. The eyes are always positioned relative to `SCREEN`.

## Porting notes

To move this to hardware:

1. Replace `roundedRect`, `arc`, `ellipse`, and `bezierCurveTo` with whatever your graphics library provides.
2. Keep the same mood objects and lerp logic.
3. The shape branches can stay as-is; just translate the path commands.
4. Idle timers can become simple counters or use the same `requestAnimationFrame`-style loop.

The goal is that a port only has to reimplement the drawing primitives, not redesign the personality system.
