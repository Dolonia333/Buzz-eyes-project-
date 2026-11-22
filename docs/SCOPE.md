# Scope

## In scope

- Expressive eye animation for Buzz (two eyes on a small screen)
- Browser prototype that matches the intended hardware look and feel
- Parametric mood system so new emotions can be added without rewriting the renderer
- Idle behaviors that make the character feel alive (gaze, lean, blink)
- Documentation so other people can understand and extend the system
- Clear path toward a port to ESP32 / Flipper Zero or similar constrained hardware

## Out of scope (for now)

- Full body animation or the keypad itself (the keypad in the demo is visual only)
- Voice, speech, or audio
- Networking, AI decision-making, or mood selection logic (that lives elsewhere)
- Pixel-perfect match to a specific physical screen resolution (we match proportions and style)
- A full game engine or scene graph

## Success criteria

- Moods are visually distinct at a glance
- Transitions feel smooth, not abrupt
- Idle state looks alive without constant motion
- A new contributor can add a mood by editing a few numbers and (if needed) one shape branch
- The HTML file remains self-contained and easy to open

## Constraints

- Must stay simple enough to reimplement in C / Arduino-style code or a lightweight graphics library
- Prefer a small number of parameters over complex path animation
- Keep the demo as a single HTML file for easy sharing and testing
