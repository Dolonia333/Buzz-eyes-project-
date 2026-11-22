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
| 19 | Front and back shell in **clear resin (SLA), frosted** | 1 set | The see-through body | Only resin prints come out truly clear. Wet-sand and matte-coat for the frosted look, which hides fingerprints and small scratches | Print |
| 20 | Matte clear spray, UV-resistant | 1 | Frosted finish and protection | Stops clear resin from yellowing in sunlight | Small |
| 20a | Black inner chassis (printed, opaque) | 1 | Holds every part in place inside the see-through shell | Makes the inside look organized, with wire channels | Print |
| 20b | Silver key caps with engraved icons | 12 | The keys | Opaque silver matches the mock-up and hides the switches; icons paint-filled | Print |
| 20c | Antenna stem and clear bead diffuser | 1 set | Antenna light | Clear resin bead glows with the LED | Print |
| 20d | Black screen mask (paint or thin printed frame) | 1 | Hides the screen's edges behind the frosted shell | Makes the blue screen look crisp | Small |
| 20e | Branded battery wrap or sticker | 1 | Covers the silver battery pouch, which shows through | Turns the battery into a branded detail | Small |
| 21 | Clear acrylic, 1 to 2 mm, cut to 36 × 21 mm | 1 | Screen lens | Protects the screen, looks like glass | Small |
| 22 | [DIY magnetic ring kit](https://www.newegg.com/nakedcellphone-diy-mount-compatible-with-magsafe/p/0SB-019N-000C2) | 1 | Snaps onto MagSafe | Use the ring with magnets inside, not the plain metal one | See site |
| 23 | Self-tapping M2 × 6 mm screws | 4 | Back shell opens for fixes | Safer than heat-set inserts in resin, which can crack. Give the screw posts thick walls | Small |
| 24 | Black Kapton tape | 1 roll | Insulates the battery and boards | Black disappears behind the frosted shell; amber shows | Small |
| 25 | Thin double-sided foam tape | 1 | Holds the battery in place | Cushions the battery, no glue on the pouch | Small |
| 26 | E6000 or 2-part epoxy | 1 | Magnet ring and lens | Grips PLA and acrylic | Small |
| 27 | 30 AWG silicone wire in matched colors (black + blue) | 1 | All internal wiring, visible through the shell | Matched colors in the chassis channels look intentional | Kit |
| 28 | Jumper wires + breadboard | 1 | Desk testing before it goes in the case | Test everything before soldering | Kit |

### Tools (one-time)

Soldering iron and stand, solder and flux, flush cutters, tweezers, wire strippers, a multimeter (to check polarity and voltage), USB-C cable, resin print service or resin printer (plus nitrile gloves and isopropyl alcohol if printing at home), wet sandpaper from 400 to 2000 grit, safety glasses.

### Cost

The listed parts with known prices (screen, XIAO, IMU, battery, motor) come to about $42. Adding the hall sensor, keypad chip, magnet ring, kit parts, printing and shipping, **budget roughly $110 to $160 for the first unit with clear resin printing and finishing**, less per unit after that. This is an estimate; prices change.

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

## 5. Enclosure: frosted see-through shell

The shell is **frosted clear resin**, so you can see the parts inside. That means the inside has to look as designed as the outside.

### Shell

- **Size:** about 56 × 63 mm, about 18 mm thick with these off-the-shelf boards (the product target is 9 mm with a custom board later).
- **Material:** clear resin printed on a resin (SLA) printer. Filament printers can only make cloudy translucent PETG, which is fine for fit tests only.
- **Where to print:** a resin print service such as JLCPCB or Craftcloud, or a home resin printer.
- **Finish for frosted:** wash and cure, wet-sand from 400 up to 2000 grit, then a **matte, UV-resistant clear coat**. The matte coat gives the frosted look; the UV protection stops yellowing.
- **Resin shrink:** add about 0.2 mm to the key holes, USB-C slot and screen window.
- **Fit test first:** print one shell in cheap gray resin before the clear one.

### Front shell

- Screen window, 36 × 21 mm, for the lens
- **Black screen mask:** paint the inside border of the window black, or add a thin printed black frame, so the screen's edges and ribbon cable are hidden
- 12 round key holes in a 4 × 3 grid
- Antenna stem hole, top right
- **Bottom edge:** slot for the XIAO's USB-C port, flush with the edge

### Inside: make it look intentional

- **Black inner chassis:** an opaque printed tray that holds the screen, boards and battery in fixed spots. Through the frosted shell you see a tidy skeleton.
- **Wire channels** in the chassis, with **matched-color wire** (black and blue) cut short.
- **Black circuit boards** where there's a choice.
- **Black Kapton tape** instead of amber.
- **Branded battery wrap or sticker** over the battery's silver pouch. Never glue to the pouch.
- **Antenna light:** with a see-through shell, the LED glows through the whole case. To light only the bead, paint a black ring around the stem inside.

### Keys

- **Silver key caps** (opaque) with **engraved icons filled with paint**: heart (Pet), face (Mood), music note, camera, 1, 2, 3, Find, Focus, Mute, Game, Back.
- Each cap has a **lip inside the shell** so it can't fall out.
- About **0.2 mm clearance** around each cap, and switches mounted so a cap travels about **0.3 mm** before it clicks.
- **For unit two:** try a one-piece **cast silicone keypad** (frosted or clear silicone poured into a 3D-printed mold) for a softer, premium feel.
- **Optional backlit keys:** with frosted or clear caps, a few NeoPixels under the keypad can light the keys in the mood's color. Costs more battery and wiring.

### Back shell

- Same frosted resin. The magnet ring on the outside shows through as a design detail.
- **Four self-tapping M2 screws** into thick screw posts (heat-set inserts can crack resin).
- Small hole for the power switch.

### Print settings (starting point)

Resin at 0.05 mm layers, front face up so the visible surface is smooth, supports on the inside faces only.

Order of layers, front to back: frosted shell with mask and lens, keys on their perfboard, screen, black chassis, XIAO + IMU + hall + keypad chip, battery (wrapped) with motor and buzzer beside it, frosted back shell, magnet ring.

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

7. Print a fit-test shell in gray resin, then the clear shell, chassis, caps, antenna and back.
8. Wash, cure, wet-sand and matte-coat the clear parts. Paint the screen mask and fill the key icons.
9. Solder the 12 switches and diodes onto the perfboard, sized to the key holes.
10. Replace jumpers with short matched-color silicone wires, routed in the chassis channels.
11. Fit the layers into the chassis in order. Black Kapton under the battery, foam tape to hold it, battery wrap on top.
12. Check the USB-C port lines up with the bottom slot. Close the back with 4 self-tapping screws (don't overtighten in resin).
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
