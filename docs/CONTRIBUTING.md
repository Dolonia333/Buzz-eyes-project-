# Contributing

Thanks for helping with Buzz's eyes. The system is intentionally simple so changes stay easy to review and port.

## Before you start

1. Open `buzz_eyes_demo.html` in a browser and click through the moods.
2. Read [ARCHITECTURE.md](ARCHITECTURE.md) so you know what the parameters do.
3. Check [ROADMAP.md](ROADMAP.md) for current priorities.

## Adding a new mood

1. Add an entry to the `moods` object with the eight parameters.
2. Add a button in the HTML controls that calls `setMood('yourMood')`.
3. If the existing shape branches don't cover it, add a new `else if` in `drawEye` **before** the generic rounded-rect fallback.
4. Test that it doesn't collide with nearby moods (happy/playful/love/cute share the `bottomPinch` range).

## Improving an existing shape

- Prefer adjusting the numbers in the `moods` object first.
- Only change the path code in `drawEye` if the silhouette is fundamentally wrong.
- Keep the shape readable at small size. Extreme detail usually gets lost on the target hardware.
- After a change, click the mood several times and let it idle so you can see transitions and blinks.

## Style notes

- The demo is a single HTML file on purpose. Don't split it into modules unless we decide to change the porting strategy.
- Comments should explain *why* a threshold or offset exists, not just what the code does.
- Match the existing naming and parameter style so the file stays scannable.

## Testing

- Open the file directly in a browser (no server required).
- Verify every mood button still works.
- Check that idle looking-around and blinks still feel natural.
- If you change screen proportions, confirm the eyes stay centered and don't clip.

## Pull requests

- One mood or one clear improvement per PR is easier to review.
- Include a short description of what changed and why (e.g. "mad half-circles were reading as sad under rotation").
- Screenshots or a short recording of the before/after are helpful.

## Questions

If something is unclear, open an issue. The architecture doc and this file are the source of truth for how the system is meant to grow.
