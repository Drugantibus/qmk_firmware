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
    MCR_SIG,               //Signature macro
    MCR_WTF,               //What the fuck did you say to me you little bitch?
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
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, MCR_SIG, _______, _______, MCR_WTF, _______, _______, _______, _______, _______, _______,          _______, _______, \
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
        case MCR_SIG:
            if (record->event.pressed) {
                SEND_STRING("Best,\nAlessandro Fort");
            }
            return false;
        case MCR_WTF:
            if (record->event.pressed) {
                SEND_STRING("What the fuck did you just fucking say about me, you little bitch? I'll have you know I graduated top of my class in the Navy Seals, and I've been involved in numerous secret raids on Al-Quaeda, and I have over 300 confirmed kills. I am trained in gorilla warfare and I'm the top sniper in the entire US armed forces. You are nothing to me but just another target. I will wipe you the fuck out with precision the likes of which has never been seen before on this Earth, mark my fucking words. You think you can get away with saying that shit to me over the Internet? Think again, fucker. As we speak I am contacting my secret network of spies across the USA and your IP is being traced right now so you better prepare for the storm, maggot. The storm that wipes out the pathetic little thing you call your life. You're fucking dead, kid. I can be anywhere, anytime, and I can kill you in over seven hundred ways, and that's just with my bare hands. Not only am I extensively trained in unarmed combat, but I have access to the entire arsenal of the United States Marine Corps and I will use it to its full extent to wipe your miserable ass off the face of the continent, you little shit. If only you could have known what unholy retribution your little \"clever\" comment was about to bring down upon you, maybe you would have held your fucking tongue. But you couldn't, you didn't, and now you're paying the price, you goddamn idiot. I will shit fury all over you and you will drown in it. You're fucking dead, kiddo.");
            }
            return false;
        case MO(2):
            if (record->event.pressed) {
                layer_previous_state = rgb_matrix_get_flags();
                rgb_matrix_set_flags(LED_FLAG_NONE);
                rgb_matrix_set_color_all(0, 0, 0);
                rgb_matrix_set_color(46, 0, 255, 0); // X
                rgb_matrix_set_color(49, 255, 0, 0); // B
                rgb_matrix_set_color(58, 255, 50, 0); // LCTRL
                rgb_matrix_set_color(59, 255, 50, 0); // LGUI
                rgb_matrix_set_color(60, 255, 50, 0); // LALT
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
        //         data[1] = 'F';
        //         data[2] = 'o';
        //         data[3] = 'r';
        //         data[4] = 't';
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
        // Not working
        // case KC_LALT:
        //     if (record->event.pressed) {
        //         for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        //             for (uint8_t j = 0; j < MATRIX_COLS; j++){
        //                 // RGB_MATRIX_TEST_LED_FLAGS();
        //                 if (keymaps[1][i][j] != KC_TRNS) {
        //                     rgb_matrix_set_color(i+j, 0, 255, 0);
        //                 } else {
        //                     rgb_matrix_set_color(i+j, 255, 50, 0);
        //                 }
        //             }
        //         }
        //         return true;
        //     }
        default:
            return true; //Process all other keycodes normally
    }
}

#ifdef RAW_ENABLE
void raw_hid_receive(uint8_t *data, uint8_t length) {
    switch(data[0]){
        // Set RGB state
        case 1:
            switch (data[1]) {
                case 2: {
                    rgb_matrix_enable_noeeprom();
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    rgb_matrix_set_color_all(0, 0, 0);
                }
                break;
                case 3: {
                    rgb_matrix_enable_noeeprom();
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                }
                break;
                case 4: {
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
        break;
        // Notifications
        // Note: caller is expected to reset RGB to previous state
        // TODO: set flags on single LEDs so RGB animation isn't interrupted on underglow
        case 2:
            
            switch (data[1]) {
                // Solid bottom underglow
                case 1: {
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
                }
                break;
                // Full color change
                case 2: {
                    uint8_t r = data[2];
                    uint8_t g = data[3];
                    uint8_t b = data[4];
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_set_color_all(r, g, b);
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
    }
}
#endif

#ifdef UCIS_ENABLE
    const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE(
    UCIS_SYM("poop", 0x1F4A9),                // 💩
    UCIS_SYM("rofl", 0x1F923),                // 🤣
    UCIS_SYM("bang", 0x203D),                 // ‽
    UCIS_SYM("sarc", 0x2E2E),                 // ⸮
    UCIS_SYM("f",    0x2131),                 // ℱ
    UCIS_SYM("look", 0x0CA0, 0x005F, 0x0CA0)  // ಠ_ಠ
);
#endif