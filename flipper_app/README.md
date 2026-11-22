# Buzz Eyes — Flipper Zero

Expressive eyes for the Buzz character, drawn procedurally on the Flipper Zero 128×64 screen.

## Controls

| Button | Action |
|--------|--------|
| **OK** | Cycle mood |
| **Left / Right** | Look left / right |
| **Up** | Blink |
| **Down** | Open eyes |
| **Back** | Exit |

Moods: Neutral → Happy → Mad → Sad → Sleepy → Love → Playful

## Build & install

1. Copy this folder to `applications_user/buzz_eyes` inside your Flipper firmware source.
2. From the firmware root:

```bash
./fbt fap_buzz_eyes
```

Or with uFBT (recommended):

```bash
cd applications_user/buzz_eyes
ufbt
```

3. Copy the generated `.fap` to your Flipper SD card under `apps/Games/`.

## Notes

- Pure Canvas drawing — no image assets.
- Flipper Canvas has no rotation or anti-aliasing, so Mad and Happy use circle + white-box masking to approximate arcs.
- Idle behavior: occasional look drift and rare blinks.
- `icon.png` is optional; the app will still build and install without it.

## Next

Once running on device, tweak the draw coordinates in `draw_eye()` to better match the browser prototype and your reference photos.
