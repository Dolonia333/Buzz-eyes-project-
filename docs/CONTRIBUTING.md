# Contributing

Thanks for helping with Buzz's eyes. Read [ARCHITECTURE.md](ARCHITECTURE.md) first, then check [ROADMAP.md](ROADMAP.md) for what's next.

## Before you start

1. Look at every expression in the README (or `docs/expressions/all.png`).
2. If you have a Flipper Zero: `cd flipper/buzz_eyes && ufbt launch` and click through the faces.
3. Open `prototype/take-buzz-apart.html` in a browser to see the hardware the eyes run on.

## Adding or changing an expression

1. Add an ID to `BzMoodId` in `flipper/buzz_eyes/buzz_faces.h`, **before** `BZ_MOOD_HALF` so it can be browsed.
2. Add its bars and symbols in `buzz_faces.c`. `EYE(x, y1, h1, y2, h2, y3, h3, anim, slot)` makes one three-bar eye; the left eye starts at x=32, the right at x=80. Shorthands like `OPEN`, `ARCH` and `LINES` work inside `EYE`.
3. Add a row to the `bz_moods` table with its name and animations.
4. Give it a color in `bz_mood_color()` in `buzz_engine.c` if it shouldn't be Signal Blue.
5. Optional: add a sound, light or vibration in `play_effects()` in `buzz_eyes_app.c`.
6. Mirror the change in `prototype/take-buzz-apart.html` (the `M` and `COL` tables).
7. Run `python3 tools/export_expressions.py`, then add the new GIF to the README tables.

## Design rules

- **Bars carry the emotion.** Change heights and motion before adding symbols.
- **Symbols are a last resort**, and stay small and in a corner.
- **Readable at 1.27".** If it needs squinting on the Flipper, it's too detailed.
- **Black eyes on a colored screen.** Eyes are always black; the screen color carries the mood.
- Keep colors to the palette in ARCHITECTURE.md unless the brand changes.

## Code style

- Engine code (`buzz_faces.*`, `buzz_engine.*`) is plain C99 with no platform headers and no `malloc`.
- Comments explain *why* a value or threshold exists.
- Match the existing naming: `bz_` for functions, `BZ_` for constants, `Bz` for types.

## Testing

- `cd flipper/buzz_eyes && ufbt` must build with no warnings (the SDK treats warnings as errors).
- `python3 tools/export_expressions.py` must run cleanly; look at `docs/expressions/all.png` for anything clipped or off-center.
- On a Flipper, check the boot, blinks and every face you touched.
- Open the teardown page and check the changed face on its screen.

## Browser prototype

`buzz_eyes_demo.html` is the original parametric demo. To add a mood there: add it to the `moods` object, add a button that calls `setMood('yourMood')`, and only add a new `drawEye` branch if the existing shapes can't cover it. Keep it a single self-contained file.

## Pull requests

- One expression or one clear improvement per PR.
- Say what changed and why, and attach the regenerated GIF.
- Don't commit build output (`dist/`, `.vscode/`); the `.gitignore` covers it.

## Questions

Open an issue. ARCHITECTURE.md and this file are the source of truth for how the system grows.
