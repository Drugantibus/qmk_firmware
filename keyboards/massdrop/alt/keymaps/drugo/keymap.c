#include QMK_KEYBOARD_H
#include "raw_hid.h"
// #include "print.h"

enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
    RGB_FRZ,               //Freezes current RGB effect
    HID_SND,               //Send test HID
    MCR_BST,               //Signature macro (Best)
    MCR_THX,               //Signature macro (Thanks)
    MCR_APPR,              //Approval macro
    MCR_PSW,               //Password macro
    MCR_ISS,               //Issues macro
    MCR_UCIS,              //Print configured UCIS entries
    UNI_ON,                //Startd UCIS (unicode input)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // DEFAULT
    [0] = LAYOUT_65_ansi_blocker(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_END, \
 CTL_T(KC_CAPS), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP, \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN, \
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    // FUNCTION
    [1] = LAYOUT_65_ansi_blocker(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_MUTE, \
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______,U_T_AUTO,U_T_AGCR, _______, KC_PSCR, KC_SLCK, KC_PAUS, _______, KC_HOME, \
        _______,RGB_RMOD, RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, HID_SND, _______, _______, _______, _______, _______,          _______, KC_BRIU, \
        _______, RGB_TOG, RGB_FRZ, _______, _______, MD_BOOT, NK_TOGG, DBG_TOG, DBG_KBD,DBG_MTRX, _______, _______,          KC_VOLU, KC_BRID, \
        UNI_ON , UC_M_WC, UC_M_LN ,                            KC_MPLY,                            MO(2)  , _______, KC_MPRV, KC_VOLD, KC_MNXT  \
    ),
    // MACRO
    [2] = LAYOUT_65_ansi_blocker(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, MCR_THX, _______,MCR_UCIS, MCR_ISS, _______, MCR_PSW, _______, _______, _______, _______, \
        _______,MCR_APPR, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______, _______, _______, MCR_BST, _______, _______, _______, _______, _______, _______,          _______, _______, \
        DM_REC1, DM_RSTP, DM_PLY1,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    
};


#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))

uint8_t layer_previous_state = LED_FLAG_ALL;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case UNI_ON:
            if (record->event.pressed) {
                qk_ucis_start();
            }
            return false;        
        case MCR_BST:
            if (record->event.pressed) {
                SEND_STRING("\n\nBest,\nAlessandro Fort");
            }
            return false;
        case MCR_THX:
            if (record->event.pressed) {
                SEND_STRING("\n\nThanks,\nAlessandro Fort");
            }
            return false;
        case MCR_APPR:
            if (record->event.pressed) {
                SEND_STRING("\n\nI've sent your request for approval, I'll let you know as soon as I'm able to proceed.\n@ , please see this request for ");
            }
            return false;
        case MCR_PSW:
            if (record->event.pressed) {
                SEND_STRING("\n\nPassword was reset as requested. Please let us know in case of any issues.");
            }
            return false;
        case MCR_ISS:
            if (record->event.pressed) {
                SEND_STRING("Please let us know in case there are any issues.");
            }
            return false;
        case MCR_UCIS:
            if (record->event.pressed) {
                SEND_STRING("bang sarc f look lenny sus shrug yay tflip tback lost cool");
            }
            return false;
        case MO(2):
            if (record->event.pressed) {
                layer_previous_state = rgb_matrix_get_flags();
                rgb_matrix_set_flags(LED_FLAG_NONE);
                rgb_matrix_set_color_all(0, 0, 0);
                rgb_matrix_set_color(20, 255, 50, 0); // T
                rgb_matrix_set_color(22, 100, 0, 200);// U
                rgb_matrix_set_color(25, 255, 50, 0); // P
                rgb_matrix_set_color(23, 255, 50, 0); // I
                rgb_matrix_set_color(31, 255, 50, 0); // A
                rgb_matrix_set_color(49, 255, 50, 0); // B
                rgb_matrix_set_color(58, 100, 0, 200);// LCTRL
                rgb_matrix_set_color(59, 100, 0, 200);// LGUI
                rgb_matrix_set_color(60, 100, 0, 200);// LALT
                for (int i = 67; i < 105; i++){
                    rgb_matrix_set_color(i, 0, 200, 50); // Underglow
                }
            }else {
                rgb_matrix_set_flags(layer_previous_state);
                rgb_matrix_set_color_all(0, 0, 0);
            }
            return true;
        // case HID_SND:
        //     if (record->event.pressed) {
        //         uint8_t data[RAW_EPSIZE];
        //         data[0] = 1;
        //         data[1] = 'Q';
        //         data[2] = 'M';
        //         data[3] = 'K';
        //         data[4] = '!';
        //         for (int i = 5; i < RAW_EPSIZE; i++){
        //             data[i] = 0;
        //         }
        //         raw_hid_send(data, RAW_EPSIZE);
        //     }
        //     return false;
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR): {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
              }
            }
            return false;
        case RGB_FRZ:
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_NONE: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                    }
                    break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                    }
                    break;
                }
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}

void set_led_range(int start, int stop, uint8_t r, uint8_t g, uint8_t b){
    for(int i = start; i <= stop; i++){
        rgb_matrix_set_color(i, r, g, b);
    }
}

#ifdef RAW_ENABLE
void raw_hid_receive(uint8_t *data, uint8_t length) {
    switch(data[0]){
        // Set RGB state
        case 1:
            switch (data[1]) {
                case 2: 
                    rgb_matrix_enable_noeeprom();
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    rgb_matrix_set_color_all(0, 0, 0);
                    break;
                case 3: 
                    rgb_matrix_enable_noeeprom();
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                    break;
                case 4: 
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                    break;
                case 5: 
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_step();
                    break;
                default: 
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                    break;
            }
            break;
        // Notifications
        // Note: caller is expected to reset RGB to previous state
        // TODO: set flags on single LEDs so RGB animation isn't interrupted on underglow
        case 2:
            switch (data[1]) {
                // Bottom underglow
                case 1:{
                    uint8_t r = data[2];
                    uint8_t g = data[3];
                    uint8_t b = data[4];
                    switch(rgb_matrix_get_flags()){
                        case LED_FLAG_ALL:
                            rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                            break;
                        case LED_FLAG_UNDERGLOW:
                            rgb_matrix_set_flags(LED_FLAG_NONE);
                            break;
                        case LED_FLAG_NONE:
                            rgb_matrix_enable_noeeprom();
                            break;
                    }
                    for (int i = 67; i <= 81; i++) {
                        rgb_matrix_set_color(i, r, g, b);
                    }
                    break;
                }
                // Full color change
                case 2:{
                    uint8_t r = data[2];
                    uint8_t g = data[3];
                    uint8_t b = data[4];
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_set_color_all(r, g, b);
                    break;
                }
                // Full underglow
                case 3:{ 
                    uint8_t r = data[2];
                    uint8_t g = data[3];
                    uint8_t b = data[4];
                    switch(rgb_matrix_get_flags()){
                        case LED_FLAG_ALL:
                            rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                        break;
                        case LED_FLAG_UNDERGLOW:
                            rgb_matrix_set_flags(LED_FLAG_NONE);
                        break;
                        case LED_FLAG_NONE:
                            rgb_matrix_enable_noeeprom();
                        break;
                    }
                    for (int i = 67; i <= 105; i++) {
                        rgb_matrix_set_color(i, r, g, b);
                    }
                    break;
                }
            }
            break;
        // Get RGB state
        case 3:
            switch(rgb_matrix_get_flags()){
                case LED_FLAG_ALL: data[0] = 1; break;
                case LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR: data[0] = 2; break;
                case LED_FLAG_UNDERGLOW: data[0] = 3; break;
                case LED_FLAG_NONE: data[0] = 4; break;
                default: data[0] = 0; break;
            }
            raw_hid_send(data, RAW_EPSIZE);
            break;
        // Single LED control
        case 4:{
            // TODO: set only the specific LED(s) flag
            rgb_matrix_set_flags(LED_FLAG_NONE);
            uint8_t index = data[2];
            uint8_t r = data[3];
            uint8_t g = data[4];
            uint8_t b = data[5];
            switch(data[1]){
                // Set one led to color
                case 1:
                    rgb_matrix_set_color(index, r, g, b);
                    break;
                // Set one row to color
                case 2:
                    switch(index){
                        case 1: // First row
                            set_led_range(0, 14, r, g, b);
                            break;
                        case 2: // Second row
                            set_led_range(15, 29, r, g, b);
                            break;
                        case 3: // Third row
                            set_led_range(30, 43, r, g, b);
                            break;
                        case 4: //Fourth row
                            set_led_range(44, 57, r, g, b);
                            break;
                        case 5: // Fifth row
                            set_led_range(58, 66, r, g, b);
                            break;
                        case 6: // Bottom underglow
                            set_led_range(67, 81, r, g, b);
                            break;
                        case 7: // Right underglow
                            set_led_range(82, 86, r, g, b);
                            break;
                        case 8: // Top underglow
                            set_led_range(87, 99, r, g, b);
                            break;
                        case 9: // Left underglow
                            set_led_range(100, 104, r, g, b);
                            break;
                    }
                break;
            }
        }
    }
}
#endif

#ifdef UCIS_ENABLE
    const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE(
    UCIS_SYM("bang", 0x203D),                 // ‽
    UCIS_SYM("sarc", 0x2E2E),                 // ⸮
    UCIS_SYM("f",    0x2131),                 // ℱ
    UCIS_SYM("look", 0x0CA0, 0x005F, 0x0CA0),  // ಠ_ಠ
    UCIS_SYM("lenny",0x0028, 0x0020, 0x0361, 0x00B0, 0x0020, 0x035C, 0x0296, 0x0020, 0x0361, 0x00B0, 0x0029),  // ( ͡° ͜ʖ ͡°)
    UCIS_SYM("sus",  0x0028, 0x0020, 0x0360, 0x00b0, 0x0020, 0x035f, 0x0296, 0x0020, 0x0361, 0x00b0, 0x0029), // ( ͠° ͟ʖ ͡°)
    UCIS_SYM("shrug",0x00AF, 0x005C, 0x005F, 0x0028, 0x30C4, 0x0029, 0x005F, 0x002F, 0x00AF), // ¯\_(ツ)_/¯
    UCIS_SYM("yay",  0x1555, 0x0028, 0x0020, 0x141B, 0x0020, 0x0029, 0x1557), // ᕕ( ᐛ )ᕗ
    UCIS_SYM("tflip",0x0028, 0x256f, 0x00b0, 0x25a1, 0x00b0, 0xff09, 0x256f, 0xfe35, 0x0020, 0x253b, 0x2501, 0x253b), // (╯°□°）╯︵ ┻━┻
    UCIS_SYM("tback",0x252c, 0x2500, 0x252c, 0x30ce, 0x0028, 0x0020, 0x00ba, 0x0020, 0x005f, 0x0020, 0x00ba, 0x30ce, 0x0029), // ┬─┬ノ( º _ ºノ)
    UCIS_SYM("lost", 0x0028, 0x0020, 0xff61, 0x005f, 0xff61, 0x0029), // ( ｡_｡)
    UCIS_SYM("cool", 0x0028, 0x2310, 0x25a0, 0x005f, 0x25a0, 0x0029) // (⌐■_■)
);
#endif
