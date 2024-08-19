/* Copyright 2024 NikkiSatmaka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#include "flow.h"

enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _COLEMAKDH,
    _QWERTY,
    _EXT,
    _FUNC,
    _SYM,
    _NUM,
    _MOUSE,
    _ADJUST
};

enum custom_keycodes {
    KC_COLEMAKDH = SAFE_RANGE,
    KC_QWERTY,
    KC_EXT,
    KC_SYM,
    KC_ADJUST,
    KC_PRVWD,
    KC_NXTWD,
    KC_LSTRT,
    KC_LEND,
    KC_DLINE,
    OS_FUNC
};

// Shortcut to make keymap more readable

#define L_EXT         MO(_EXT)
#define L_SYM         MO(_SYM)
#define L_FUNC        MO(_FUNC)
#define L_NUM         MO(_NUM)
#define L_MOUSE       MO(_MOUSE)
#define T_QWERTY      TG(_QWERTY)
#define D_MOUSE       DF(_MOUSE)
#define D_COLEMAKDH   DF(_COLEMAKDH)

// flow_config should correspond to following format:
// * layer keycode
// * modifier keycode
const uint16_t flow_config[FLOW_COUNT][2] = {
    {L_EXT, KC_LALT},
    {L_EXT, KC_LGUI},
    {L_EXT, KC_LSFT},
    {L_EXT, KC_LCTL},
    {L_EXT, KC_RALT},
    {L_SYM, KC_LALT},
    {L_SYM, KC_LGUI},
    {L_SYM, KC_LSFT},
    {L_SYM, KC_LCTL}
};

const uint16_t flow_layers_config[FLOW_LAYERS_COUNT][2] = {
    {OS_FUNC, _FUNC}
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * COLEMAK-DH
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  Del |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   '  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  ;   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |Enter |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR | EXT  | /LShift /       \Space \  | SYM  | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_COLEMAKDH] = LAYOUT(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,  KC_8,     KC_9,    KC_0,     KC_DEL,
  KC_TAB,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,                        KC_J,    KC_L,  KC_U,     KC_Y,    KC_QUOT,  KC_BSPC,
  KC_ESC,   KC_A,   KC_R,    KC_S,    KC_T,    KC_G,                        KC_M,    KC_N,  KC_E,     KC_I,    KC_O,     KC_SCLN,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_D,    KC_V, KC_MUTE,      XXXXXXX, KC_K,    KC_H,  KC_COMM,  KC_DOT,  KC_SLSH,  KC_ENT,
                  KC_LGUI, KC_LALT, KC_LCTL, L_EXT, KC_LSFT,       KC_SPC, L_SYM, KC_RCTL, KC_RALT, KC_RGUI
),

/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  Del |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |Enter |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR | EXT  | /LShift /       \Space \  | SYM  | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_QWERTY] = LAYOUT(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,  KC_8,     KC_9,    KC_0,     KC_DEL,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                       KC_Y,    KC_U,  KC_I,     KC_O,    KC_P,     KC_BSPC,
  KC_ESC,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                       KC_H,    KC_J,  KC_K,     KC_L,    KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,     XXXXXXX, KC_N,    KC_M,  KC_COMM,  KC_DOT,  KC_SLSH,  KC_ENT,
                  KC_LGUI, KC_LALT, KC_LCTL, L_EXT, KC_LSFT,      KC_SPC, L_SYM, KC_RCTL, KC_RALT, KC_RGUI
),

/* EXT
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | ESC  |BackWd| Find |ForWd | Ins  |                    | PUp  | Home |  Up  | End  | Caps | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |O_LAlt|O_LGUI|O_LSft|O_Lctl|O_RAlt|-------.    ,-------| PDown| Left | Down | Right|  Del |  `   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      | Undo | Cut  | Copy | LGUI |Paste |-------|    |-------|Enter | Bspc |  Tab | Menu | Pscr |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR | EXT  | /LShift /       \Enter \  | FUNC | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_EXT] = LAYOUT(
  _______, KC_F1,    KC_F2,       KC_F3,      KC_F4,       KC_F5,                             KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
  _______, KC_ESC,   A(KC_LEFT),  C(KC_F),    A(KC_RGHT),  KC_INS,                            KC_PGUP,  KC_HOME,  KC_UP,    KC_END,   KC_CAPS,  KC_F12,
  _______, KC_LALT,  KC_LGUI,     KC_LSFT,    KC_LCTL,     KC_RALT,                           KC_PGDN,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_DEL,   KC_GRV,
  _______, C(KC_Z),  C(KC_X),     C(KC_INS),  KC_LGUI,     S(KC_INS), _______,       _______, KC_ENT,   KC_BSPC,  KC_TAB,   KC_APP,   KC_PSCR,  _______,
                                  _______, _______, _______, _______, _______,       KC_ENT, L_FUNC, _______, _______, _______
),

/* FUNC
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | STOP | PREV | PLAY | NEXT | BRIUP|                    |  F12 |  F7  |  F8  |  F9  |ZoomIn|      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |O_LAlt|O_LGUI|O_LSft|O_Lctl| BRIDN|-------.    ,-------|  F11 |  F4 |   F5  |  F6  |ZoomOut|     |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      | MUTE | VOLDO| Copy | VOLUP|Paste |-------|    |-------|  F10 |  F1  |  F2  |  F3  |ZoomRst|     |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR | EXT  | /LShift /       \Space \  | FUNC | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_FUNC] = LAYOUT(
  _______, _______,  _______,  _______,    _______,  _______,                           _______,  _______,  _______,  _______,  _______,     _______,
  _______, KC_MSTP,  KC_MPRV,  KC_MPLY,    KC_MNXT,  KC_BRIU,                           KC_F12,   KC_F7,    KC_F8,    KC_F9,    C(KC_PPLS),  _______,
  _______, KC_LALT,  KC_LGUI,  KC_LSFT,    KC_LCTL,  KC_BRID,                           KC_F11,   KC_F4,    KC_F5,    KC_F6,    C(KC_PMNS),  _______,
  _______, KC_MUTE,  KC_VOLD,  RCS(KC_C),  KC_VOLU,  RCS(KC_V), _______,       _______, KC_F10,   KC_F1,    KC_F2,    KC_F3,    C(KC_P0),    _______,
                            _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),

/* SYM
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   !  |   @  |   #  |   $  |   %  |                    |   =  |   `  |   :  |   ;  |   +  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |O_LAlt|O_LGUI|O_LSft|O_Lctl|   ^  |-------.    ,-------|   *  |   (  |   {  |   [  |   -  |      |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      |   \  |   |  |   \  |   |  |   &  |-------|    |-------|   ~  |   )  |   }  |   ]  |   _  |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |MOUSE | /  NUM  /       \Space \  | SYM  | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_SYM] = LAYOUT(
  _______, KC_F1,    KC_F2,       KC_F3,    KC_F4,       KC_F5,                           KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
  _______, KC_EXLM,  KC_AT,       KC_HASH,  KC_DLR,      KC_PERC,                         KC_EQL,   KC_GRV,   KC_COLN,  KC_SCLN,  KC_PLUS,  KC_F12,
  _______, KC_LALT,  KC_LGUI,     KC_LSFT,  KC_LCTL,     KC_CIRC,                         KC_ASTR,  KC_LPRN,  KC_LCBR,  KC_LBRC,  KC_MINS,  _______,
  _______, KC_NUBS,  S(KC_NUBS),  KC_NUHS,  S(KC_NUHS),  KC_AMPR, _______,       _______, KC_TILD,  KC_RPRN,  KC_RCBR,  KC_RBRC,  KC_UNDS,  _______,
                              _______, _______, _______, L_MOUSE, L_NUM,         _______, _______, _______, _______, _______
),

/* NUM
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   !  |   @  |   #  |   :  |   ;  |                    |   =  |   7  |   8  |   9  |   +  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |O_LAlt|O_LGUI|O_LSft|O_Lctl|O_RAlt|-------.    ,-------|   *  |   4  |   5  |   6  |   -  |      |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      |      | Menu |  Tab | Bspc |Enter |-------|    |-------|   0  |   1  |   2  |   3  |   /  |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |MOUSE | /  NUM  /       \Space \  | SYM  | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_NUM] = LAYOUT(
  _______, _______,  _______,  _______,  _______,  _______,                         _______,  _______,  _______,  _______,  _______,  _______,
  _______, _______,  _______,  _______,  _______,  KC_NUM,                          KC_EQL,   KC_7,     KC_8,     KC_9,     KC_PLUS,  _______,
  _______, KC_LALT,  KC_LGUI,  KC_LSFT,  KC_LCTL,  KC_RALT,                         KC_ASTR,  KC_4,     KC_5,     KC_6,     KC_MINS,  _______,
  _______, _______,  KC_APP,   KC_TAB,   KC_BSPC,  KC_ENT,  _______,       _______, KC_0,     KC_1,     KC_2,     KC_3,     KC_SLSH,  _______,
                        _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),

/* MOUSE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |TG_QWE|      |MOUS1 | M UP |MOUS2 |M Wl U|                    |M Acc2|   &  |   `  |   ~  |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |DF_SYM|      |M LFT |M DWN |M RGHT|M Wl D|-------.    ,-------|M Acc1|   $  |   %  |   ^  |   _  |      |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |DF_CLM|M Wl L|MOUS3 |MOUS4 |MOUS5 |M Wl R|-------|    |-------|M Acc0|   !  |   @  |   #  |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |MOUSE | /  NUM  /       \Space \  | SYM  | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_MOUSE] = LAYOUT(
  _______,      _______, _______, _______, _______, _______,                             _______,  _______,  _______,  _______,  _______, _______,
  T_QWERTY,     _______,  KC_BTN1,  KC_MS_U,  KC_BTN2,  KC_WH_U,                         KC_ACL2,  KC_AMPR,  KC_GRV,   KC_TILD,  _______, _______,
  D_MOUSE,      _______,  KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_WH_D,                         KC_ACL1,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_UNDS, _______,
  D_COLEMAKDH,  KC_WH_L,  KC_BTN3,  KC_BTN4,  KC_BTN5,  KC_WH_R, _______,       _______, KC_ACL0,  KC_EXLM,  KC_AT,    KC_HASH,  _______, _______,
                             _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
)

};

#ifdef OLED_ENABLE

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
    };

    oled_write_P(qmk_logo, false);
}

static void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n\n"), false);
    oled_write_ln_P(PSTR("MODE"), false);
    oled_write_ln_P(PSTR(""), false);
    if (keymap_config.swap_lctl_lgui) {
        oled_write_ln_P(PSTR("MAC"), false);
    } else {
        oled_write_ln_P(PSTR("WIN"), false);
    }

    switch (get_highest_layer(default_layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("Qwrt"), false);
            break;
        case _COLEMAKDH:
            oled_write_ln_P(PSTR("Clmk"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n\n"), false);
    // Print current layer
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
        case _COLEMAKDH:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("Sym"), false);
            break;
        case _EXT:
            oled_write_P(PSTR("Ext"), false);
            break;
        case _FUNC:
            oled_write_P(PSTR("Func"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("Num\n"), false);
            break;
        case _MOUSE:
            oled_write_P(PSTR("Mouse\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR("CPSLK"), led_usb_state.caps_lock);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
    }
    return false;
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!update_flow(keycode, record->event.pressed, record->event.key)) return false;

    switch (keycode) {
        case KC_COLEMAKDH:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAKDH);
            }
            return false;
        case KC_QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case KC_EXT:
            if (record->event.pressed) {
                layer_on(_EXT);
                update_tri_layer(_EXT, _SYM, _ADJUST);
            } else {
                layer_off(_EXT);
                update_tri_layer(_EXT, _SYM, _ADJUST);
            }
            return false;
        case KC_SYM:
            if (record->event.pressed) {
                layer_on(_SYM);
                update_tri_layer(_EXT, _SYM, _ADJUST);
            } else {
                layer_off(_SYM);
                update_tri_layer(_EXT, _SYM, _ADJUST);
            }
            return false;
        case KC_ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
        case KC_PRVWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_NXTWD:
             if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_LSTRT:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                     //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
            }
            break;
        case KC_LEND:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
            }
            break;
        case KC_DLINE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_BSPC);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_BSPC);
            }
            break;
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
            }
            return false;
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
            }
            return false;
    }
    return true;
}

void matrix_scan_user(void) {
    flow_matrix_scan();
}

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return true;
}

#endif
