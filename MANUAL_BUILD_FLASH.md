# Manual Build & Flash Instructions for Linux

**No scripts, full control, no surprises.**

Compatible with Ubuntu, Debian, Arch, Manjaro, and other Linux distributions.

## Overview

This guide uses:
- **Docker** - to build the firmware (uses QMK's official pre-built image, no Dockerfile needed)
- **avrdude** - to flash the firmware to your keyboard

You don't need to install Python, QMK CLI, or the full AVR toolchain. Docker handles the build environment, and avrdude is a single package install.

> **Note:** The Atreus62 uses a Pro Micro with the **caterina** bootloader, so it flashes with `avrdude` over a virtual serial port (`/dev/ttyACM0`). This is different from the Ergodox EZ, which uses a Teensy and `teensy_loader_cli`.

---

## Prerequisites

### Ubuntu / Debian

```bash
# Install Docker (if not already installed)
sudo apt update
sudo apt install docker.io
sudo systemctl start docker
sudo systemctl enable docker

# Add yourself to docker group (to run without sudo)
sudo usermod -aG docker $USER
# Log out and back in for group to take effect

# Install flashing tool
sudo apt install avrdude
```

### Arch / Manjaro

```bash
# Install Docker (if not already installed)
sudo pacman -S docker
sudo systemctl start docker
sudo systemctl enable docker

# Add yourself to docker group (to run without sudo)
sudo usermod -aG docker $USER
# Log out and back in for group to take effect

# Install flashing tool
sudo pacman -S avrdude
```

---

## Part 1: Build Firmware

### Step 1: Check you're in the right directory

```bash
cd ~/scottleeroberts/atreus62-layout
ls -la keymap.c config.h rules.mk
# All three files should exist
```

### Step 2: Create build directory

```bash
mkdir -p build
cd build
```

### Step 3: Clone QMK firmware (first time only)

```bash
# Pin to the exact QMK version this firmware was tested against
QMK_SHA=9e8199c41189a2eb6243600bf3f96f136650820b

# If you haven't cloned QMK yet:
git clone https://github.com/qmk/qmk_firmware.git
cd qmk_firmware
git checkout $QMK_SHA
cd ..

# If you already have it and want to re-pin:
cd qmk_firmware
git fetch origin
git checkout $QMK_SHA
cd ..
```

**This takes 2-5 minutes depending on your internet.**

### Step 4: Initialize submodules (first time only)

```bash
cd qmk_firmware

# The Atreus62 is an AVR board, so only LUFA is required
git submodule update --init --recursive lib/lufa

# If you'd rather just grab everything (takes 5-10 minutes, ~2GB):
# git submodule update --init --recursive
```

### Step 5: Copy your keymap files

```bash
# Create keymap directory
mkdir -p keyboards/atreus62/keymaps/scottleeroberts

# Copy your files
cp ../../keymap.c keyboards/atreus62/keymaps/scottleeroberts/
cp ../../config.h keyboards/atreus62/keymaps/scottleeroberts/
cp ../../rules.mk keyboards/atreus62/keymaps/scottleeroberts/

# Verify they're there
ls -la keyboards/atreus62/keymaps/scottleeroberts/
```

### Step 6: Build with Docker

```bash
# Make sure you're in build/qmk_firmware directory
pwd
# Should show: .../atreus62-layout/build/qmk_firmware

# Run the build
docker run --rm \
  -v $(pwd):/qmk_firmware \
  -w /qmk_firmware \
  ghcr.io/qmk/qmk_cli \
  make atreus62:scottleeroberts
```

**This takes 2-3 minutes.** You'll see compilation messages scrolling by.

Success looks like:
```
Linking: .build/atreus62_scottleeroberts.elf
Creating load file for flashing: .build/atreus62_scottleeroberts.hex
 * The firmware size is fine - XXXXX/28672 (XX%, XXXX bytes free)
```

> The Atreus62's caterina bootloader reserves 4KB, so the usable flash budget is
> **28672 bytes**, not the full 32KB.

### Step 7: Copy firmware out

```bash
# Copy the .hex file to parent directory
cp atreus62_scottleeroberts.hex ../..

# Go back to main directory
cd ../..

# Verify it's there
ls -lh atreus62_scottleeroberts.hex
```

**Build complete!** You now have: `atreus62_scottleeroberts.hex`

---

## Part 2: Flash Firmware

The Atreus62 is a single-PCB keyboard with one Pro Micro. **One flash updates the whole board.**

### Step 1: Connect the keyboard

Plug in the Atreus62 via USB.

### Step 2: Know your bootloader window

The caterina bootloader only stays active for about **8 seconds** after reset, then
hands back to the firmware. So the order matters: **start avrdude first, then reset.**
The `-D` flag below is not used; avrdude will wait for the port to appear.

### Step 3: Put keyboard in bootloader mode

**Option A: Physical RESET button**
Press the reset button on the PCB (double-tap it if a single press doesn't take).

**Option B: Keyboard shortcut (if this firmware is already flashed)**
1. Hold the Numbers layer key (left side of the bottom row, `MO(NUMBERS)`)
2. Press the top-left key (ESC position), which is mapped to `QK_BOOT`

### Step 4: Confirm the bootloader port

```bash
ls /dev/ttyACM*

# You should see /dev/ttyACM0 appear right after reset.
# It disappears again after ~8 seconds.
```

### Step 5: Flash

```bash
avrdude -v -p atmega32u4 -c avr109 -P /dev/ttyACM0 \
  -U flash:w:atreus62_scottleeroberts.hex:i
```

**Timing tip:** if the port keeps vanishing before avrdude connects, press RESET and
run the command immediately, or use a wait loop:

```bash
until [ -e /dev/ttyACM0 ]; do sleep 0.1; done && \
avrdude -v -p atmega32u4 -c avr109 -P /dev/ttyACM0 \
  -U flash:w:atreus62_scottleeroberts.hex:i
```

Start that command, *then* press RESET.

### Step 6: Test

1. Keyboard should automatically reconnect after flash
2. Type some keys - they should work
3. If not working, unplug and replug USB

---

## Alternative: let QMK do the flashing

From `build/qmk_firmware`, this builds and flashes in one step, and handles the
bootloader wait for you:

```bash
make atreus62:scottleeroberts:avrdude
```

Press RESET when it prompts "Detecting USB port...".

Note this needs a local AVR toolchain (it can't run inside the Docker one-liner,
since the container has no access to the reset event on your USB bus).

---

## Done!

The keyboard is now flashed and ready to use.

---

## Troubleshooting

### avrdude: "can't open device /dev/ttyACM0"

**Cause:** The bootloader window closed before avrdude connected, or the port has a
different number.

**Solution:**
1. Run `ls /dev/ttyACM*` right after pressing RESET to confirm the port name
2. Use the `until` wait-loop above, starting it *before* you press RESET
3. Double-tap RESET rather than single-pressing

### Permission denied on /dev/ttyACM0

```bash
# Option 1: Run with sudo
sudo avrdude -v -p atmega32u4 -c avr109 -P /dev/ttyACM0 \
  -U flash:w:atreus62_scottleeroberts.hex:i

# Option 2: Add yourself to the dialout/uucp group (permanent fix)
sudo usermod -aG uucp $USER      # Arch / Manjaro
sudo usermod -aG dialout $USER   # Ubuntu / Debian
# Log out and back in

# Option 3: Add a udev rule
sudo tee /etc/udev/rules.d/50-atreus62.rules > /dev/null <<'RULE'
# Pro Micro caterina bootloader
ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0036", MODE:="0666"
ATTRS{idVendor}=="1b4f", ATTRS{idProduct}=="9205", MODE:="0666"
ATTRS{idVendor}=="1b4f", ATTRS{idProduct}=="9203", MODE:="0666"
RULE

sudo udevadm control --reload-rules
sudo udevadm trigger
```

### ModemManager grabs the port

On some distros ModemManager probes new serial devices and steals the bootloader
window.

```bash
sudo systemctl stop ModemManager
# flash, then optionally: sudo systemctl start ModemManager
```

### Docker permission denied

```bash
# Add yourself to docker group
sudo usermod -aG docker $USER

# Log out and back in
```

Or run Docker with sudo:
```bash
sudo docker run --rm \
  -v $(pwd):/qmk_firmware \
  -w /qmk_firmware \
  ghcr.io/qmk/qmk_cli \
  make atreus62:scottleeroberts
```

### Build fails with "submodule not found"

```bash
cd build/qmk_firmware
git submodule update --init --recursive --force
cd ../..
```

### Firmware too large

The caterina bootloader leaves 28672 bytes. If you go over, turn features off in
`rules.mk`:

```make
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
UNICODE_ENABLE = no
```

### Keyboard is bricked / won't enumerate

Short the `RST` and `GND` pins on the Pro Micro twice in quick succession. That
forces the bootloader for 8 seconds regardless of firmware state.

---

## Quick Reference Card

**Build:**
```bash
cd ~/scottleeroberts/atreus62-layout
mkdir -p build && cd build
QMK_SHA=9e8199c41189a2eb6243600bf3f96f136650820b
git clone https://github.com/qmk/qmk_firmware.git  # first time
cd qmk_firmware
git checkout $QMK_SHA
git submodule update --init --recursive lib/lufa  # first time
mkdir -p keyboards/atreus62/keymaps/scottleeroberts
cp ../../{keymap.c,config.h,rules.mk} keyboards/atreus62/keymaps/scottleeroberts/
docker run --rm -v $(pwd):/qmk_firmware -w /qmk_firmware ghcr.io/qmk/qmk_cli make atreus62:scottleeroberts
cp atreus62_scottleeroberts.hex ../..
cd ../..
```

**Flash:**
```bash
# Start this first, THEN press RESET (or hold Numbers + ESC)
until [ -e /dev/ttyACM0 ]; do sleep 0.1; done && \
avrdude -v -p atmega32u4 -c avr109 -P /dev/ttyACM0 \
  -U flash:w:atreus62_scottleeroberts.hex:i
```

---

## Notes

- **Single flash** - the Atreus62 is one PCB with one Pro Micro
- **Each build is clean** - no cached state to corrupt
- **You see every command** - no hidden operations
- **Easy to debug** - if something fails, you know exactly where
- **No scripts to trust** - you control everything
- **QMK SHA pinned** - reproducible builds with `9e8199c41189a2eb6243600bf3f96f136650820b`
- **Same pin as the [ergodox](https://github.com/scottleeroberts/ergodox) repo** - both boards build against the identical QMK tree
