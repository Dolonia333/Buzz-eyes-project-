# Roadmap

## Current state

Browser demo with:

- Multiple moods (neutral, happy, mad, sad, cute, love, playful, etc.)
- Smooth morphing between moods
- Idle looking-around, lean-in, and rare blinks
- Basic device silhouette (screen, body, antenna, keypad) for visual context

## Phase 1 — Polish the prototype (now)

- [ ] Lock in the final neutral eye shape and scale against the hardware reference
- [ ] Make remaining moods clearly distinct (especially happy / playful / love / cute)
- [ ] Tune idle timing so looking-around matches the desired personality
- [ ] Stabilize the mad and sad shapes so they read correctly under rotation
- [ ] Clean comments and parameter names in the demo

## Phase 2 — Hardware mapping

- Document the exact screen resolution and color depth of the target device
- Convert the Canvas drawing logic into a portable drawing API (or direct pixel writes)
- Test blink timing and frame rate on the real hardware
- Decide which moods are essential for the first hardware version

## Phase 3 — Integration

- Hook mood changes to external triggers (buttons, AI state, sensors)
- Add a simple API or message format so other code can request a mood
- Optional: persist last mood or idle personality across restarts

## Phase 4 — Polish and extras

- More moods if needed (determined, skeptical, etc. already exist in the demo)
- Subtle secondary motion (ear wiggle, antenna, screen brightness) if the hardware supports it
- Documentation and examples for common integrations

## How to contribute to the roadmap

If you want to work on something, open an issue or start a PR against one of the unchecked items in Phase 1. For larger changes (new rendering approach, hardware port), discuss first so we don't diverge from the parametric style.
