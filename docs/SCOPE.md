# Scope

## In scope

- Expressive eye animation for Buzz, the MagSafe OLED robot: two eyes on a small screen
- **Signal eyes**: the chosen eye style, three vertical bars per eye
- 24 core expressions and 8 MagSafe moments, plus the snap-on boot sequence
- A portable C engine that runs on the Flipper Zero now and the ESP32 next
- Color for the product screen (1.27" 128x96 SSD1351 OLED), with a color per mood
- Previews generated from the real engine (`docs/expressions/`) so docs always match the device
- The interactive prototype teardown and parts list (`prototype/take-buzz-apart.html`)
- The brand kit (`docs/BRAND.md`, `docs/brand/`)
- The original browser prototype (`buzz_eyes_demo.html`), kept for reference
- Documentation so other people can understand and extend the system

## Out of scope (for now)

- ESP32 firmware beyond the eye engine: Bluetooth, the phone app, sensor drivers and power management live elsewhere
- Deciding *when* moods change (AI, notification rules); this repo only shows them
- Circuit board and enclosure CAD files
- Voice or speech
- A full game engine or scene graph

## Success criteria

- Moods are readable at a glance on a 1.27" screen and recognizably Buzz
- Animations feel alive at about 30 fps on the target hardware
- A new expression is a few lines in `buzz_faces.c` and shows up everywhere after `python3 tools/export_expressions.py`
- The engine compiles with no platform-specific code and no warnings
- The Flipper app builds with `ufbt` against the official SDK

## Constraints

- Engine stays plain C (C99), no dynamic memory, no platform headers
- Faces are drawn on the 128x64 grid so they work on both the Flipper and the color panel
- Keep the brand rules: bars carry the emotion, symbols are a last resort, always glow on black
