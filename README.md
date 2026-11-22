# Buzz Eyes

Animated expressive eyes for a small hardware character (Buzz), currently prototyped in the browser.

This is a visual and animation layer meant to run on constrained screens (ESP32, Flipper Zero, or similar) while staying simple enough to port. The HTML demo is the current source of truth for shapes, timing, and personality.

## What this is

A parametric eye system that draws two eyes on a light-blue screen using Canvas 2D. Eyes morph between moods (neutral, happy, mad, sad, cute, love, playful, etc.) with smooth interpolation. Idle behavior includes looking around, occasional leans, and rare blinks so the character feels alive even when nothing is happening.

The target look is inspired by simple stylized eyes (EVE from WALL-E, lilguynet-style hardware) rather than realistic eyes: clear shapes, strong silhouettes, and readable emotion at small size.

## Why it exists

Buzz is a small physical character. The eyes are the main way it shows personality. We need:

- Distinct, readable emotions
- Smooth transitions so mood changes don't pop
- Idle motion that feels curious instead of mechanical
- Something simple enough to port to an ESP32 or similar without a full rendering engine

The browser demo lets us iterate on shapes and timing before writing the hardware version.

## How it works (short)

- Each mood is a set of numbers: eye width/height, corner radius, gap, vertical offset, rotation, top/bottom pinch.
- Every frame the current values lerp toward the target mood.
- A `drawEye` function picks a shape based on those values (rounded rect, half-circle, heart, smile arc, wink, etc.).
- Idle logic slowly drifts the gaze, occasionally leans one eye in, and triggers rare blinks (skipped on happy/love).

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for the full parameter list and shape rules.

## Quick start

1. Open `buzz_eyes_demo.html` in a browser.
2. Click the mood buttons to switch expressions.
3. Leave it idle to see looking-around and blinks.

No build step. No dependencies.

## Repo layout

```
.
├── README.md                 ← you are here
├── buzz_eyes_demo.html       ← live prototype (source of truth)
├── flipper/
│   └── buzz_eyes/            ← Flipper Zero app (Signal eyes)
└── docs/
    ├── SCOPE.md              ← what is in / out of scope
    ├── ROADMAP.md            ← phases and next work
    ├── ARCHITECTURE.md       ← how the system is structured
    └── CONTRIBUTING.md       ← how to add moods or improve shapes
```

## Flipper Zero prototype

`flipper/buzz_eyes/` is a Flipper Zero app that plays the **Signal eyes** direction (three vertical bars per eye) on the Flipper's 128x64 screen, with 24 expressions, 8 MagSafe moments and the snap-on boot sequence. Its animation engine is plain C, so it can be reused on the ESP32. See [flipper/buzz_eyes/README.md](flipper/buzz_eyes/README.md) to build and install it.

## Contributing

Read [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) before changing shapes or adding moods. The goal is to keep the system parametric and the demo self-contained so hardware ports stay easy.

## Related

Part of the broader DENIZEN / Dolonia work on expressive AI characters. This repo is focused only on the Buzz eye animation layer.
