# Roadmap

## Done

- [x] Pick the eye style: **Signal eyes** (three bars per eye)
- [x] 24 core expressions and 8 MagSafe moments
- [x] Snap-on boot sequence (bars light up like signal, then a happy hello)
- [x] Portable C engine with blinks, equalizer peak caps and per-bar stagger
- [x] Flipper Zero app with LED, chirp and vibration
- [x] Pick the product screen: 1.27" 128x96 full-color OLED (SSD1351)
- [x] Color per mood, rainbow for Vibing and Dizzy
- [x] Generated GIF previews, contact sheet and hero banner
- [x] Brand kit: logo, app icons, palette, type, voice
- [x] Interactive teardown and full parts list (`prototype/take-buzz-apart.html`)

## Phase 1: Breadboard prototype (now)

- [ ] Order the parts in the teardown's parts list
- [ ] Test the eyes on the Flipper and tune timing on real hardware
- [ ] Wire the XIAO ESP32C3 + 1.27" color OLED and draw the engine output in color
- [ ] Measure the color OLED module's real thickness and update the stack numbers
- [ ] Add the IMU and hall sensor and trigger Surprised, Dizzy, Snap on and Bye for now from them
- [ ] Check battery life with the color screen (150 mAh vs 350 mAh)

## Phase 2: First enclosure

- [ ] 3D-print the shell, fit the parts, and test the magnet ring on a MagSafe phone
- [ ] Wire the keys, RGB antenna LED, buzzer and vibration motor
- [ ] Tune the hall sensor so it tells the phone's magnets from Buzz's own ring

## Phase 3: Phone connection

- [ ] Bluetooth link to a companion app
- [ ] Map phone events to moods: notifications, music, calls, focus mode, camera
- [ ] Simple message format so the app can request a mood

## Phase 4: Toward a product

- [ ] Custom circuit board with an ESP32-C3 module and a bare OLED panel to get to about 9 mm
- [ ] Battery and power-switch design for shipping
- [ ] FCC certification path (pre-certified module) and a trademark check on "MagSafe" wording
- [ ] Marketing clips of Snap on, Vibing, My turn and Say cheese

## How to contribute to the roadmap

Pick an unchecked item, open an issue to say you're on it, and keep PRs to one item each. For bigger changes (new eye style, new screen, new board), discuss first.
