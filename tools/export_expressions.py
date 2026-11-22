#!/usr/bin/env python3
"""
Renders every Buzz expression to an animated GIF for the docs, using the real
animation engine from flipper/buzz_eyes (compiled with your system gcc).

    python3 tools/export_expressions.py

Writes docs/expressions/<name>.gif and docs/expressions/all.png.
Needs Pillow (pip install pillow) and gcc.
"""
import os
import re
import subprocess
import tempfile

from PIL import Image, ImageDraw, ImageFont

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ENGINE = os.path.join(ROOT, "flipper", "buzz_eyes")
OUT = os.path.join(ROOT, "docs", "expressions")

W, H = 128, 64
SCALE = 3  # each OLED pixel becomes a 3x3 cell with a 1 px gap, like a real panel
FPS = 20
BG = (5, 7, 10)
LIT = (106, 220, 255)  # Signal Blue
GHOST = (13, 20, 27)  # faint unlit pixel grid

# Longer clips for faces whose motion takes longer to show.
DURATION_MS = {"idle": 6000, "curious": 5000, "bored": 5000, "wink": 5000}
DEFAULT_MS = 3600


def slug(name):
    return re.sub(r"[^a-z0-9]+", "-", name.lower()).strip("-")


def build_dumper(tmp):
    exe = os.path.join(tmp, "frame_dump")
    subprocess.run(
        [
            "gcc", "-O2", "-I", ENGINE,
            os.path.join(ROOT, "tools", "frame_dump.c"),
            os.path.join(ENGINE, "buzz_engine.c"),
            os.path.join(ENGINE, "buzz_faces.c"),
            "-lm", "-o", exe,
        ],
        check=True,
    )
    return exe


def to_image(frame):
    img = Image.new("RGB", (W * SCALE, H * SCALE), BG)
    d = ImageDraw.Draw(img)
    for y in range(H):
        row = frame[y * W:(y + 1) * W]
        for x in range(W):
            color = LIT if row[x] else GHOST
            d.rectangle([x * SCALE, y * SCALE, x * SCALE + SCALE - 2, y * SCALE + SCALE - 2], fill=color)
    return img


def main():
    os.makedirs(OUT, exist_ok=True)
    with tempfile.TemporaryDirectory() as tmp:
        exe = build_dumper(tmp)
        listing = subprocess.run([exe, "list"], capture_output=True, text=True, check=True).stdout
        moods = [(int(i), n) for i, n in (line.split("\t") for line in listing.strip().splitlines())]
        # Only browsable expressions (static helper frames start at "Half").
        browsable = []
        for i, n in moods:
            if n == "Half":
                break
            browsable.append((i, n))

        stills = []
        for i, name in browsable:
            ms = DURATION_MS.get(slug(name), DEFAULT_MS)
            raw = subprocess.run([exe, str(i), str(ms), str(FPS)], capture_output=True, check=True).stdout
            frames = [raw[k:k + W * H] for k in range(0, len(raw), W * H)]
            images = [to_image(f) for f in frames]
            path = os.path.join(OUT, slug(name) + ".gif")
            images[0].save(path, save_all=True, append_images=images[1:], duration=1000 // FPS, loop=0, optimize=True)
            # Pick a representative still for the contact sheet (a third of the way in).
            stills.append((name, images[len(images) // 3]))
            print("wrote", os.path.relpath(path, ROOT))

        # The snap-on boot: dark, 1 bar, 1 eye, full signal, then a happy hello.
        index = {n: i for i, n in moods}
        boot = []
        for name, ms in [("Off", 500), ("Snap 1", 150), ("Snap 2", 200), ("Snap 3", 250), ("Happy", 1500)]:
            raw = subprocess.run([exe, str(index[name]), str(ms), str(FPS)], capture_output=True, check=True).stdout
            boot += [to_image(raw[k:k + W * H]) for k in range(0, len(raw), W * H)]
        boot_path = os.path.join(OUT, "boot.gif")
        boot[0].save(boot_path, save_all=True, append_images=boot[1:], duration=1000 // FPS, loop=0, optimize=True)
        print("wrote", os.path.relpath(boot_path, ROOT))

        # Contact sheet of everything, 4 per row.
        cols = 4
        cw, ch = W * SCALE, H * SCALE
        pad = 16
        label_h = 28
        rows = (len(stills) + cols - 1) // cols
        sheet = Image.new("RGB", (cols * (cw + pad) + pad, rows * (ch + label_h + pad) + pad), (11, 14, 19))
        d = ImageDraw.Draw(sheet)
        try:
            font = ImageFont.load_default(size=18)
        except TypeError:  # Pillow < 10.1
            font = ImageFont.load_default()
        for k, (name, img) in enumerate(stills):
            x = pad + (k % cols) * (cw + pad)
            y = pad + (k // cols) * (ch + label_h + pad)
            sheet.paste(img, (x, y))
            d.text((x + 4, y + ch + 4), name, fill=(232, 236, 241), font=font)
        sheet_path = os.path.join(OUT, "all.png")
        sheet.save(sheet_path)
        print("wrote", os.path.relpath(sheet_path, ROOT))


if __name__ == "__main__":
    main()
