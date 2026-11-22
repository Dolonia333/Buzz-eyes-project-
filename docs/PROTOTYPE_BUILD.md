# Prototype Build Plan

Everything needed to build the first working Buzz: every part, the battery and how it goes in, the wiring and pins, the enclosure, step-by-step assembly, testing, and what's still open.

Use it with the interactive teardown (`prototype/take-buzz-apart.html`) and [KEYS_AND_SOFTWARE.md](KEYS_AND_SOFTWARE.md).

**Plan to build two units.** The first one is for learning and fixing mistakes; the second should be the clean one.

---

## 1. What the prototype does

- Snaps onto a MagSafe iPhone with a magnet ring
- 1.27" full-color screen: **blue with black eyes**, screen color changes with the mood
- 12-key BlackBerry-style keypad
- Motion sensor (Surprised, Dizzy, Upside down...) and magnet sensor (Snap on, Bye for now)
- Antenna light, vibration, buzzer chirp
- Bluetooth to the phone (music controls, notifications)
- Rechargeable battery, **USB-C charging on the bottom edge**, power switch

---

## 2. Complete parts list (per unit)

Prices are each retailer's listed single-unit price at the time of writing, before shipping. "See site" means check the current price. "Kit" means it's in any basic electronics starter kit.

### Electronics

| # | Part | Qty | Why we need it | Why this one | Price |
|---|---|---|---|---|---|
| 1 | [Waveshare 1.27" RGB OLED, 128×96, SSD1351](https://www.waveshare.com/1.27inch-rgb-oled-module.htm) | 1 | The screen | Same 128-px width as the eye design, true color, 42.2 × 29 mm fits the shell, includes its 7-pin cable | $16.99 |
| 2 | [Seeed XIAO ESP32C3](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) | 1 | Brain, Bluetooth, battery charger | Built-in LiPo charging and USB-C, 21 × 17.8 mm, about $5 | $4.99 |
| 3 | [Adafruit LSM6DSOX IMU](https://www.adafruit.com/products/4438) | 1 | Motion: shake, tilt, free fall | Qwiic plug-in, tap and free-fall detection built in | $11.95 |
| 4 | [Adafruit TMAG5273 hall sensor](https://learn.adafruit.com/adafruit-tmag5273-3d-hall-effect-magnetometer-breakout/overview) | 1 | Knows when Buzz is snapped on or off | 3D sensing tells the phone's magnets from Buzz's own ring, 3.3V, Qwiic | See site |
| 5 | Adafruit TCA8418 keypad matrix breakout (Qwiic) | 1 | Reads all 12 keys | Frees XIAO pins (see section 4): it scans and debounces the keypad itself and sends key events over I2C | See site |
| 6 | 6 mm tactile switches | 12 | The keys | Standard and clicky, fit under round printed caps | Kit |
| 7 | 1N4148 diodes | 12 | One per key | Stops "ghost" presses when two keys are held | Kit |
| 8 | Small perfboard | 1 | Mounts the 12 switches behind the keys | Easy to cut to the keypad size | Kit |
| 9 | Mini RGB LED, NeoPixel type (SK6812 or WS2812B mini) | 1 | Antenna light | Any color from **one** data pin | Kit |
| 10 | [Adafruit vibration motor disc](https://www.adafruit.com/product/1201) | 1 | Haptic tap on key presses and alerts | 10 × 2.7 mm, 2 to 5V | $1.95 |
| 11 | PN2222 transistor + 1N4148 diode + 1 kΩ resistor | 1 set | Drives the motor | The motor draws 60 to 100 mA, more than a pin can give; the diode protects against motor spikes | Kit |
| 12 | Passive piezo buzzer, about 12 mm | 1 | Snap-on chirp, key sounds | Passive plays real notes; active only beeps | Kit |
| 13 | Qwiic / STEMMA QT cables, 50 mm | 3 | Chain IMU, hall sensor and keypad chip | No soldering, short and tidy | Small |

### Power

| # | Part | Qty | Why | Why this one | Price |
|---|---|---|---|---|---|
| 14 | [Adafruit LiPo 3.7V 150 mAh](https://www.adafruit.com/products/1317) | 1 | Powers Buzz | 19.75 × 26.02 × 3.8 mm fits beside the motor and buzzer | $5.95 |
| 15 | Bigger LiPo, about 350 mAh (backup) | 1 | If the lit blue screen drains the 150 mAh too fast | Longer battery life, a bit thicker. Buy one to test | See site |
| 16 | JST-PH 2-pin socket with wires | 1 | Lets the battery unplug | You can swap or remove the battery without desoldering | Kit |
| 17 | Mini slide switch (SPDT) | 1 | Power on/off | Cuts the battery so it doesn't drain in a drawer | Kit |
| 18 | 2 × 220 kΩ resistors | 1 set | Battery level reading | The XIAO has no built-in battery gauge; this lets Buzz show Low battery | Kit |

### Enclosure and assembly

| # | Part | Qty | Why | Why this one | Price |
|---|---|---|---|---|---|
| 19 | 3D-printed front and back shell | 1 set | The body | Silk silver PLA looks metallic off the printer; sand + chrome paint for the mirror look | Print |
| 20 | 3D-printed key caps (12), antenna stem, bead diffuser | 1 set | Keys and antenna | Print the bead in white or clear PLA so the LED glows through | Print |
| 21 | Clear acrylic, 1 to 2 mm, cut to 36 × 21 mm | 1 | Screen lens | Protects the screen, looks like glass | Small |
| 22 | [DIY magnetic ring kit](https://www.newegg.com/nakedcellphone-diy-mount-compatible-with-magsafe/p/0SB-019N-000C2) | 1 | Snaps onto MagSafe | Use the ring with magnets inside, not the plain metal one | See site |
| 23 | M2 heat-set inserts + M2 × 6 mm screws | 4 | Back shell opens for fixes | Inserts survive opening many times | Small |
| 24 | Kapton tape | 1 roll | Insulates the battery and boards | Thin and heat-proof | Small |
| 25 | Thin double-sided foam tape | 1 | Holds the battery in place | Cushions the battery, no glue on the pouch | Small |
| 26 | E6000 or 2-part epoxy | 1 | Magnet ring and lens | Grips PLA and acrylic | Small |
| 27 | 30 AWG silicone wire (several colors) | 1 | All internal wiring | Thin and flexible so it folds into a 9 to 18 mm case | Kit |
| 28 | Jumper wires + breadboard | 1 | Desk testing before it goes in the case | Test everything before soldering | Kit |

### Tools (one-time)

Soldering iron and stand, solder and flux, flush cutters, tweezers, wire strippers, a multimeter (to check polarity and voltage), USB-C cable, 3D printer or print service, heat-set insert tip, sandpaper, safety glasses.

### Cost

The listed parts with known prices (screen, XIAO, IMU, battery, motor) come to about $42. Adding the hall sensor, keypad chip, magnet ring, kit parts, printing and shipping, **budget roughly $90 to $130 for the first unit**, less per unit after that. This is an estimate; prices change.

---

## 3. The battery: which one and how it goes in

### Which battery

- **Start with the 150 mAh LiPo** (19.75 × 26.02 × 3.8 mm). It fits the planned layout.
- **Also buy a 350 mAh** to test. Buzz's screen is lit blue most of the time, which uses more power than a black screen, so the 150 mAh may only last a few hours. Measure before deciding.
- Only use LiPo cells with **built-in protection** (Adafruit's have it).

### How it connects

```
LiPo +  ──► slide switch ──► XIAO BAT+ pad (back of the XIAO)
LiPo −  ─────────────────────► XIAO BAT− pad
XIAO BAT+ ─ 220k ─┬─ 220k ─ GND
                  └──► A0 (battery level)
```

1. The XIAO ESP32C3 has **battery pads on its underside**, not a plug. Solder a short red and black wire pair to BAT+ and BAT−.
2. Put the **slide switch** in the red (+) wire.
3. End the wires in a **JST-PH socket**, so the battery plugs in and out.
4. **Check polarity with a multimeter before plugging in.** Different brands wire JST plugs differently, and reversing them can destroy the battery.
5. Add the two 220 kΩ resistors from BAT+ to GND with the middle point to A0 so the firmware can read the battery level.

Note: with the switch in the battery line, Buzz only charges when the switch is **on**. That's normal for this simple setup.

### Where it sits

- In the **battery layer** of the stack: on top of the XIAO and IMU, under the back shell (see the teardown).
- **Kapton tape** on the boards under it so no solder points touch the pouch.
- **Foam tape** to hold it. Never glue directly onto the battery.
- Leave about 0.5 to 1 mm of space around it. LiPo pouches can swell slightly over time.
- Route the wires so nothing sharp presses on the pouch.

### Charging

- Plug USB-C into the **bottom edge**. The XIAO's own USB-C port lines up with the slot in the shell, so no extra charging board is needed.
- The XIAO's orange charge LED shows charging. A light pipe in the shell can carry it to the outside.

### Battery safety

- Never puncture, bend or squeeze the pouch.
- Don't leave early prototypes charging unattended.
- If a battery puffs up, stop using it and recycle it properly.

---

## 4. Pin plan

The XIAO ESP32C3 has **11 usable pins (D0 to D10)**. Everything wired directly would need about 19, so the plan saves pins:

- The **keypad** goes on the **TCA8418 chip** over I2C (0 extra pins instead of 7).
- The **antenna LED** is a **NeoPixel-type** LED (1 pin instead of 3).
- IMU, hall sensor and keypad chip all share the **same two I2C pins**.

| XIAO pin | Connects to | Notes |
|---|---|---|
| D8 (SCK) | OLED SCL/CLK | SPI clock |
| D10 (MOSI) | OLED DIN | SPI data |
| D1 | OLED DC | Data/command |
| D2 | OLED CS | Chip select |
| D3 | OLED RST | Reset |
| D4 (SDA) | IMU, hall sensor, keypad chip | Shared I2C, via Qwiic |
| D5 (SCL) | IMU, hall sensor, keypad chip | Shared I2C, via Qwiic |
| D6 | Motor transistor (through 1 kΩ) | Vibration |
| D7 | Piezo buzzer | Tones |
| D9 | NeoPixel data | Antenna light |
| D0 (A0) | Battery divider | Battery level |
| 3V3 / GND | Screen, sensors, LED power | |

**Check before soldering:** D8 and D9 are also the ESP32-C3's boot pins. They usually work fine for SPI clock and a NeoPixel, but if the board won't start or upload, swap D9 with another pin and test on the breadboard first.

---

## 5. Enclosure

- **Size:** about 56 × 63 mm. Plan for about **18 mm thick** with these off-the-shelf boards (the product target is 9 mm with a custom board later).
- **Material:** silk silver PLA, or PLA sanded and chrome-painted for the mock-up's mirror finish.
- **Front shell:** screen window (36 × 21 mm) for the lens, 12 round key holes in a 4 × 3 grid, antenna stem hole top right.
- **Bottom edge:** a slot for the XIAO's **USB-C port**. Position the XIAO so its port sits flush with this slot.
- **Back shell:** 4 heat-set inserts and M2 screws, flat outside for the magnet ring, a small hole for the power switch.
- **Antenna:** printed stem with the NeoPixel at the top inside a white or clear printed bead.
- **Print settings (starting point):** 0.12 to 0.16 mm layers, 3 walls, 20% infill, print the front face down for a smooth surface.

Order of layers, front to back: shell and lens, keys on their perfboard, screen, XIAO + IMU + hall + keypad chip, battery with motor and buzzer beside it, back shell, magnet ring.

---

## 6. Assembly steps

### Phase A: Breadboard (no case)

1. Flash a blink test to the XIAO over USB-C.
2. Wire the screen with jumpers. Fill it blue and draw the eyes with the engine from this repo.
3. Plug in the IMU, hall sensor and keypad chip over Qwiic. Read each one.
4. Wire 3 or 4 test keys to the keypad chip, then all 12 with diodes.
5. Add the motor driver, buzzer and NeoPixel.
6. Run on battery: connect it through the switch and JST socket and check charging.

### Phase B: First fit

7. Print the shell, lens frame, caps, antenna and back.
8. Melt in the heat-set inserts.
9. Solder the 12 switches and diodes onto the perfboard, sized to the key holes.
10. Replace jumpers with short silicone wires.
11. Fit the layers in order. Kapton under the battery, foam tape to hold it.
12. Check the USB-C port lines up with the bottom slot. Close the back with 4 screws.
13. Glue the magnet ring to the outside of the back. Let it cure fully.

### Phase C: Second, cleaner unit

14. Fix what didn't fit on unit one: hole sizes, wire lengths, slot position.
15. Build unit two with the corrections.

---

## 7. Testing checklist

- [ ] Screen shows all 32 expressions with the right colors
- [ ] All 12 keys register short, long and double presses, with no ghost presses when two are held
- [ ] Every press gives a face reaction and a haptic tap
- [ ] Snap on and Bye for now trigger from the magnet sensor on a real MagSafe iPhone
- [ ] Motion faces trigger: shake, spin, flip, free fall (drop onto something soft)
- [ ] Music: play/pause and skip work on iPhone and Android; Vibing starts with music
- [ ] Notifications trigger Alert on iPhone (ANCS)
- [ ] Battery: measure hours of use on 150 mAh vs 350 mAh with the blue screen on
- [ ] Charges over USB-C from the bottom edge; power switch fully turns it off
- [ ] Holds onto the phone when shaken; doesn't block the cameras
- [ ] Nothing gets hot during charging or use

---

## 8. Open questions and risks

- **Battery life** with a fully lit screen. Measure it; this decides 150 vs 350 mAh and the case thickness.
- **Screen module thickness:** only published in Waveshare's 3D drawing file. Measure the real part.
- **Magnet ring vs. magnet sensor:** Buzz's own ring sits near the sensor. The 3D sensor plus calibration should tell the phone's magnets apart, but test early.
- **Thickness:** about 18 mm now. Getting to about 9 mm needs a custom circuit board and a bare screen panel (Phase 4 in the roadmap).
- **iPhone limits** on camera, replies and Focus (see KEYS_AND_SOFTWARE.md).
- **"MagSafe" name:** check trademark rules before using it on packaging.

---

## 9. Suggested timeline

| Week | Goal |
|---|---|
| 1 | Order parts. Eyes on the Flipper. Blink test on the XIAO. |
| 2 | Breadboard: screen, sensors, keypad, feedback, battery |
| 3 | Print and fit unit one |
| 4 | Fix the fit, build unit two, run the full testing checklist |
| 5+ | Music and notifications over Bluetooth, then the companion app |
