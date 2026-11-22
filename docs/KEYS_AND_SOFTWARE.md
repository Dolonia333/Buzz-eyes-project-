# Keys, Phone Features and Software

How Buzz's 12 keys work, what software each feature needs, and what iPhone and Android allow. Notes for planning the firmware and the companion app.

## Summary

The keys need two pieces of software:

1. **Firmware on Buzz** (the XIAO ESP32C3): reads the keys, reacts instantly with a face and a haptic tap, and talks to the phone over Bluetooth.
2. **A companion app on the phone**: handles the features the phone won't give an accessory directly.

On iPhone, Apple limits what Bluetooth accessories can do, so a few features work differently there. Music and notifications work with no app at all, so build those first.

## The keypad

Twelve round keys in a 4 × 3 grid, BlackBerry style.

| | | | |
|---|---|---|---|
| **Pet** · Happy, hold for Love | **Mood** · cycle Buzz's face | **Music** · play/pause, hold to skip | **Camera** · camera / shutter |
| **Reply 1** | **Reply 2** | **Reply 3** | **Find my phone** |
| **Focus** · Buzz goes Focused | **Mute** · silence Buzz | **Game** · mini games | **Back** · cancel, hold for settings |

## 1. Mechanics on Buzz (firmware)

- **Key matrix scanning:** read the 4 × 3 grid about every 5 ms. Twelve keys use only 7 XIAO pins, with one 1N4148 diode per key so pressing two keys at once is never misread.
- **Debounce:** ignore contact bounce (about 5 to 20 ms) so one press counts once.
- **Press types:** short press, long press (about 500 ms) and double tap. Twelve keys cover 24 or more actions.
- **Instant feedback:** every press goes straight to the eye engine (a face reaction) and the vibration motor (a short tap) before the phone answers, so Buzz always feels responsive.
- **Event queue:** key presses become events that the face engine, the sound and the Bluetooth link each handle.
- **State machine:** normal, game mode, number-pad mode (hold Back + Mood) and muted.

### Firmware stack

| Piece | Suggested software |
|---|---|
| Framework | Arduino core for ESP32, or ESP-IDF |
| Bluetooth | NimBLE (BLE HID, AMS and ANCS clients, a custom Buzz service) |
| Color screen | Adafruit_SSD1351 or TFT_eSPI |
| Motion sensor | Adafruit LSM6DS library |
| Magnet sensor | Adafruit TMAG5273 library |
| Eyes | `flipper/buzz_eyes/buzz_engine.*` and `buzz_faces.*` from this repo, unchanged |

## 2. Music

Works on iPhone and Android **with no app**, so it's the best first feature.

- **Controls:** Buzz acts as a Bluetooth **HID** device sending media keys: play/pause, next and previous track, volume. Phones already understand these, like the media keys on a Bluetooth keyboard.
- **Now playing (iPhone):** Apple's **Media Service (AMS)** Bluetooth profile, the one smartwatches use, tells Buzz when music is playing and what's on. Buzz switches to **Vibing** when music starts.
- **Now playing (Android):** the companion app reads the active media session and sends it to Buzz.

## 3. Camera

Buzz sits on the back of the phone, so it faces whoever is being photographed through the **rear camera**. That's the point of **Say cheese**: Buzz gets people to look.

- **iPhone:** an accessory **can't open the Camera app** by itself; Apple doesn't allow it. What works:
  - **As a shutter:** with the camera open, Buzz sends "volume up" over HID, which takes the photo, like Bluetooth selfie remotes.
  - **In the companion app:** the app has its own rear-camera screen. Buzz plays Say cheese, counts down and takes the photo. This only works while the app is open on screen.
- **Android:** the companion app can open the camera from a key press, so the full flow works.

## 4. The other keys

| Key | iPhone | Android |
|---|---|---|
| **Notifications** (triggers Alert) | **ANCS**, Apple's notification service for accessories, sends Buzz every notification. No app needed | Companion app with notification access |
| **Quick replies** | No silent texting from accessories. Buzz can trigger an Apple Shortcut, but you usually have to confirm before it sends | Companion app uses notification reply actions to send directly |
| **Find my phone** | Buzz tells the companion app, which plays a sound. Ringing through silent mode needs Apple's Critical Alerts permission | Companion app plays a sound, even on silent |
| **Focus** | No public way for apps to turn Focus on. Use a Shortcut, or just show Focused when you turn it on yourself | Companion app can toggle Do Not Disturb with permission |
| **Mute, Game, Mood, Pet, Back** | Run on Buzz, no phone needed | Same |

## 5. Companion app

| Option | Tools |
|---|---|
| iPhone only | Swift + CoreBluetooth |
| Android only | Kotlin |
| Both with one codebase | Flutter |

What the app does:

- Assign what each key and long press does
- Save the three quick replies
- The camera screen for Say cheese
- Find my phone
- Firmware updates for Buzz over Bluetooth
- A custom **Buzz Bluetooth service** for commands like "show this mood" and "key pressed", on top of the standard HID, AMS and ANCS profiles

## Suggested build order

1. **Music and notifications:** HID media keys, AMS and ANCS. No app needed, so it's quick to demo.
2. **Key feedback and games:** everything that runs on Buzz alone.
3. **Companion app:** camera, quick replies, Find my phone, Focus and key settings.

## Caveats

- Apple's rules for accessories change over time, so check the current Bluetooth accessory guidelines before building each feature.
- Official MagSafe branding and some Apple features require Apple's accessory program. See the notes on the "MagSafe" name in the README.
