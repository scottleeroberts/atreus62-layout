# Scott's Atreus62 Layout

Custom QMK firmware for the Atreus62 with home-row modifiers and tmux integration.

Companion to the [ergodox](https://github.com/scottleeroberts/ergodox) repo — both
boards share the same layer structure, the same home-row mods, the same tmux macros,
and build against the same pinned QMK tree. The bottom row is the one place they
deliberately differ; see [Differences](#differences-from-the-ergodox-repo).

## Features

- **Home-row modifiers**: Alt, Cmd, Ctrl, Shift on home row (ASDF / JKL;)
- **4 layers**: Base (QWERTY), Symbols, Tmux shortcuts, Numbers
- **Doubled thumb keys**: Enter and Space each appear twice on the bottom row
- **Tmux integration**: Custom macros for window management (Ctrl+Space prefix)
- **Chordal Hold + Flow Tap**: Anti-misfire for home-row mods
- **Modern QMK**: Updated for latest QMK firmware (Feb 2026)

## Building and Flashing

**Complete instructions: [MANUAL_BUILD_FLASH.md](MANUAL_BUILD_FLASH.md)**

Built against QMK firmware [`9e8199c`](https://github.com/qmk/qmk_firmware/commit/9e8199c41189a2eb6243600bf3f96f136650820b) — the same SHA the ergodox repo pins.

**Quick reference** (if you've already done the setup):
```bash
# Build (from build/qmk_firmware directory)
docker run --rm -v $(pwd):/qmk_firmware -w /qmk_firmware ghcr.io/qmk/qmk_cli \
  make atreus62:scottleeroberts

# Flash — start this first, THEN press RESET (or hold Numbers + ESC)
until [ -e /dev/ttyACM0 ]; do sleep 0.1; done && \
avrdude -v -p atmega32u4 -c avr109 -P /dev/ttyACM0 \
  -U flash:w:atreus62_scottleeroberts.hex:i
```

Current firmware size: **21826/28672 bytes (76%, 6846 free)**.

## Layer Guide

### BASE (Layer 0) - Default QWERTY

```
,-----------------------------------.              ,-----------------------------------.
| ESC |  1  |  2  |  3  |  4  |  5  |              |  6  |  7  |  8  |  9  |  0  | BkSp|
|-----+-----+-----+-----+-----+-----|              |-----+-----+-----+-----+-----+-----|
|  `  |  Q  |  W  |  E  |  R  |  T  |              |  Y  |  U  |  I  |  O  |  P  |  -  |
|-----+-----+-----+-----+-----+-----|              |-----+-----+-----+-----+-----+-----|
| Ctrl|  A  |  S  |  D  |  F  |  G  |              |  H  |  J  |  K  |  L  |  ;  |  '  |
|     | Alt | Cmd | Tmx | Ctl | Sft |              | Sft | Ctl |     | Cmd | Alt |     |
|-----+-----+-----+-----+-----+-----|              |-----+-----+-----+-----+-----+-----|
| Sft |  Z  |  X  |  C  |  V  |  B  |              |  N  |  M  |  ,  |  .  |  /  |  =  |
|-----+-----+-----+-----+-----+-----|  ,-----.     |-----+-----+-----+-----+-----+-----|
|     |     |     | Cmd | Num |Entr |  |Entr |Spc| | Spc | Sym | Tab |     |     |     |
`-----------------------------------'  `-----'     `-----------------------------------'
```

The two center keys are the big 1.5u thumb keys: **Enter** (left) and **Space**
(right). Each is doubled by the ordinary key immediately beside it, so Enter and
Space are both reachable either with the thumb or with the adjacent finger.

### SYMB (Layer 1) - Symbols & Navigation

- **Top row:** `~ ! @ # $ % ^ & * ( ) |`
- **Home row left:** `[ ] { }`
- **Home row right:** Arrow keys + `? \`
- **Bottom row:** `( )` on C and V

### TMUX (Layer 2) - Terminal Multiplexer

Activated by holding the D key. All macros send the Ctrl+Space prefix first:

| Key | Action | Sends |
|-----|--------|-------|
| U | Copy mode | Ctrl+Space [ |
| I | Split horizontal | Ctrl+Space " |
| O | Split vertical | Ctrl+Space % |
| J | Window 1 | Ctrl+Space 1 |
| K | Window 2 | Ctrl+Space 2 |
| L | Window 3 | Ctrl+Space 3 |
| ; | Window 4 | Ctrl+Space 4 |
| ' | Window 5 | Ctrl+Space 5 |
| N | New window | Ctrl+Space c |

All tmux keys sit on the right hand, so holding D (left hand) and striking them
never trips Chordal Hold.

### NUMBERS (Layer 3) - Numpad

**Right side - Number pad and media:**
```
    7  8  9    Mute
    4  5  6    Vol+
    1  2  3    Vol-
```

**Bottom row:** Tab and `0`, sitting under the `7`/`4`/`1` and `8`/`5`/`2` columns.

**Top left:** Reset/Bootloader (`QK_BOOT`)

The Atreus62 has no RGB or backlight hardware, so the left half of this layer is
otherwise unused, where the ergodox holds RGB controls.

## Configuration Details

### Home-Row Modifiers

- **Tapping term:** 170ms
- **Chordal Hold:** Same-hand tap-hold + next key resolves as tap (prevents misfires)
- **Flow Tap:** Keys pressed within 100ms of each other always tap (fast typing protection)
- **Permissive Hold:** Mod activates immediately when another key is tapped during hold

Handedness for Chordal Hold is derived automatically from the Atreus62's geometry in
`keyboard.json` — the left six columns resolve as `L`, the right six as `R`, and the
two thumb keys as `L`/`R` respectively. No manual `chordal_hold_layout` is needed.

## Files

| File | Purpose |
|------|---------|
| `keymap.c` | Main keymap and macro definitions |
| `config.h` | Keyboard configuration settings |
| `rules.mk` | Build rules and feature enables |
| `MANUAL_BUILD_FLASH.md` | Complete build and flash instructions |
| `atreus62_scottleeroberts.hex` | Prebuilt firmware, ready to flash |

## Differences from the ergodox repo

The alphas, home-row mods and layer keys match. The differences are hardware, plus
one deliberate layout change to the bottom row:

| | Atreus62 | Ergodox EZ |
|---|---|---|
| MCU / bootloader | Pro Micro, caterina | Teensy 2.0, halfkay |
| Flash tool | `avrdude` (`/dev/ttyACM0`) | `teensy_loader_cli` |
| Flash budget | 28672 bytes | 32256 bytes |
| RGB | none | underglow on Shine |
| Combos | none | J+H, K+J, D+F |
| Bottom row | Cmd/Num and Sym/Tab shifted outward; Enter and Space doubled | single Enter, single Space |

## License

GPL v2 (inherited from QMK Firmware)
