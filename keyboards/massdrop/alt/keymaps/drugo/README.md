# Drugo

My keymap. 

## Default layer
Pretty standard, except for:
- [*] Swapped HOME with END
- [+] CTRL tap-mod on CAPS LOCK
- [+] Added Scroll Lock indicator LED on ESC for KDE keyboard layout indication (enable in Input Devices → Keyboard → Advanced → Use keyboad LED to show alternative layout)
- [+] Added Space Cadet Shift 
- [+] Added Tap Dance on ESC: ESC on single tap, GUI+L on double tap


## Fn layer
- [*] Moved media controls to arrow keys (prev/next on LEFT/RIGHT, vol+/vol- on UP/DOWN, play/pause on SPACE)
- [+] Added brightness control on PGUP/PGDOWN
- [+] Added Unicode UCIS input (LCTRL to start, LGUI for Windows mode, LALT for Linux mode)
- [+] Added RGB animation freeze on X
- [+] Added macro layer on RALT

## Macro layer
- [+] Added dynamic macro record/stop/play on LCTRL/LGUI/LALT
- [+] Added various work-related macros
- [+] RGB only shows configured keys (set manually for now)
- [+] Added macOS mode toggle on ESC (Changes CAPS tap-mod to GUI)

## Other features
Notifications using RGB and HID. Host script available [here](https://github.com/Drugantibus/qmk-hid-rgb).
```
Possible message format:
    0x01: Sets the LED state
        0x01: All LEDs on
        0x02: Keylight only
        0x03: Underglow only
        0x04: All LEDs off
        0x05: Go to next RGB animation
    0x02: Notifications
        0x01: Set the "bottom" (user-facing) part of the underglow to a specific color
            0xXX: Red value
            0xXX: Green value
            0xXX: Blue value
            (hereafter (colors))
        0x02: Set the whole keyboard to a color
            (colors)
        0x03: Set the whole underglow to a color
            (colors)
    0x03: Get the current LED state
        No parameters, returns (sends) a value as in the 0x01 section
    0x04: Control single or group of LEDs
        0x01: Change a single LED's color
            0xXX: LED number
            (colors)
        0x02: Change a group of LEDs
            0x01-0x05: nth row
                (colors)
            0x06: Bottom underglow
                (colors)
            0x07: Right underglow
                (colors)
            0x08: Top underglow
                (colors)
            0x09: Left underglow
                (colors)
```
Note that the caller is expected to reset the previous state after sending a notification command. 