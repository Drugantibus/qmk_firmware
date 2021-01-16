# Drugo

My keymap. 

## Default layer
Pretty standard, except for:
- [*] Swapped HOME with END
- [+] CTRL tap-mod on CAPS LOCK

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

## Other features
Notifications using RGB and HID. Host script not published yet.
```
Possible message format:
    0x01: Sets the LED state
        0x01: All LEDs on
        0x02: Keylight only
        0x03: Underglow only
        0x04: All LEDs off
    0x02: Notifications
        0x01: Set the "bottom" (user-facing) part of the underglow to a specific color
            0xXX: Red value
            0xXX: Green value
            0xXX: Blue value
        0x02: Set the whole keyboard to a color
            0xXX: Red value
            0xXX: Green value
            0xXX: Blue value
    0x03: Get the current LED state
        No parameters, returns (sends) a value as in the 0x01 section
```
Note that the caller is expected to reset the previous state after sending a notification command. 