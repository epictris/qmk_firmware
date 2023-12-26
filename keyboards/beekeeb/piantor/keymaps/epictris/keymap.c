// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum custom_keycodes {
    MOD_TMUX = SAFE_RANGE,
    M_5GJ,
    M_5GK,
    TILD_SLSH,
    DOT_DOT_SLSH,
    Z_Z,
    TM0,
    TM1,
    TM2,
    TM3,
    GT_GT,
    LT_LT,
    TM_LEFT,
    TM_UP,
    TM_DOWN,
    TM_RIGHT,
    CAPS_WORD,
};

void apply_tmux_prefix(uint16_t keycode) {
    tap_code16(C(KC_B));
    tap_code(keycode);
}


void execute_macro(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case M_5GJ:
            return SEND_STRING("5gj");
        case M_5GK:
            return SEND_STRING("5gk");
        case TM_UP:
            return apply_tmux_prefix(KC_UP);
        case TM_DOWN:
            return apply_tmux_prefix(KC_DOWN);
        case TM_LEFT:
            return apply_tmux_prefix(KC_LEFT);
        case TM_RIGHT:
            return apply_tmux_prefix(KC_RIGHT);
        case GT_GT:
            return SEND_STRING(">>");
        case LT_LT:
            return SEND_STRING("<<");
        case TM0:
            tap_code16(C(KC_B));
            return tap_code(KC_0);
        case TM1:
            tap_code16(C(KC_B));
            return tap_code(KC_1);
        case TM2:
            tap_code16(C(KC_B));
            return tap_code(KC_2);
        case TM3:
            tap_code16(C(KC_B));
            return tap_code(KC_3);
        case DOT_DOT_SLSH:
            return SEND_STRING("../");
        case TILD_SLSH:
            return SEND_STRING("~/");
        case Z_Z:
            return SEND_STRING("zz");
    }
}

uint16_t mod_tmux_release_key = KC_NO;
bool mod_tmux_active = false;

void apply_mod_tmux(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case OSL(1):
        case MO(2):
        case OSM(KC_LSFT):
            break;
        case MOD_TMUX:
            if (record->event.pressed) {
                mod_tmux_active = true;
                mod_tmux_release_key = C(KC_B);
            }
            else {
                mod_tmux_active = false;
                tap_code16(mod_tmux_release_key);
            }
            break;
        default:
            if (record->event.pressed) {
                if (mod_tmux_active) {
                    mod_tmux_release_key = KC_NO;
                    tap_code16(C(KC_B));
                }
            }
    }
}
bool mod_bsls_active = false;
bool has_activated = false;

void apply_mod_bsls(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case OSL(1):
        case MO(2):
        case OSM(KC_LSFT):
            break;
        case KC_BSLS:
            if (record->event.pressed) {
                mod_bsls_active = true;
                has_activated = false;
            }
            else {
                mod_bsls_active = false;
                has_activated = false;
            }
            break;
        default:
            if (record->event.pressed) {
                if (mod_bsls_active) {
                    if (has_activated) {
                        tap_code(KC_BSLS);
                    }
                    has_activated = true;
                }
            }
    }
}

bool set_shift_action (uint16_t keycode, void (*action)(void), keyrecord_t *record) {
    if (record->event.pressed) {
        if (get_oneshot_mods() == MOD_BIT(KC_LSFT)) {
            clear_oneshot_mods();
            action();
            return false;
        } else if (get_mods() == MOD_BIT(KC_LSFT)) {
            clear_mods();
            action();
            add_mods(MOD_BIT(KC_LSFT));
            return false;
        }
    }
    return true;
}
bool set_shifted_key(uint16_t keycode, uint16_t shifted_keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (get_oneshot_mods() == MOD_BIT(KC_LSFT)) {
            clear_oneshot_mods();
            tap_code16(shifted_keycode);
        } else if (get_mods() == MOD_BIT(KC_LSFT)) {
            clear_mods();
            tap_code16(shifted_keycode);
            add_mods(MOD_BIT(KC_LSFT));
        } else {
            tap_code16(keycode);
        }
    }
    return true;
}

const uint16_t PROGMEM combo_bsls[] = {KC_S, KC_R,  COMBO_END};
const uint16_t PROGMEM combo_cmd1[] = {KC_C, KC_D,  COMBO_END};
const uint16_t PROGMEM combo_cmd2[] = {KC_H, KC_UNDS,  COMBO_END};
const uint16_t PROGMEM combo_ctl1[] = {KC_S, KC_T,  COMBO_END};
const uint16_t PROGMEM combo_ctl2[] = {KC_N, KC_E,  COMBO_END};
const uint16_t PROGMEM combo_sft1[] = {KC_UNDS, KC_DOT,  COMBO_END};
const uint16_t PROGMEM combo_sft2[] = {KC_X, KC_C,  COMBO_END};
const uint16_t PROGMEM combo_sft_gui1[] = {KC_UNDS, KC_DOT, KC_H, COMBO_END};
const uint16_t PROGMEM combo_sft_gui2[] = {KC_X, KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM combo_ctl_sft1[] = {KC_T, KC_S, KC_R, COMBO_END};
const uint16_t PROGMEM combo_ctl_sft2[] = {KC_N, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM combo_ctl_b[] = {KC_E, KC_I,  COMBO_END};
const uint16_t PROGMEM combo_tmux0[] = {KC_W, KC_F, COMBO_END};
const uint16_t PROGMEM combo_tmux1[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_tmux2[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM combo_tmux3[] = {KC_U, KC_Y, COMBO_END};


combo_t key_combos[] = {
    COMBO(combo_bsls, KC_BSLS),
    COMBO(combo_cmd1, KC_LGUI),
    COMBO(combo_cmd2, KC_LGUI),
    COMBO(combo_ctl1, KC_LCTL),
    COMBO(combo_ctl2, KC_LCTL),
    COMBO(combo_sft1, KC_LSFT),
    COMBO(combo_sft2, KC_LSFT),
    COMBO(combo_ctl_b, MOD_TMUX),
    COMBO(combo_tmux0, TM0),
    COMBO(combo_tmux1, TM1),
    COMBO(combo_tmux2, TM2),
    COMBO(combo_tmux3, TM3),
    COMBO(combo_sft_gui1, G(KC_LSFT)),
    COMBO(combo_sft_gui2, G(KC_LSFT)),
    COMBO(combo_ctl_sft1, C(KC_LSFT)),
    COMBO(combo_ctl_sft2, C(KC_LSFT))
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    apply_mod_tmux(keycode, record);
    apply_mod_bsls(keycode, record);

    switch (keycode) {
        case M_5GJ:
        case M_5GK:
        case TM_LEFT:
        case TM_RIGHT:
        case TM_UP:
        case TM_DOWN:
        case GT_GT:
        case LT_LT:
        case TM0:
        case TM1:
        case TM2:
        case TM3:
        case DOT_DOT_SLSH:
        case TILD_SLSH:
        case Z_Z:
            if (record->event.pressed) {
                execute_macro(keycode, record);
            }
            break;
        case KC_MINS:
        case KC_COMM:
        case KC_DOT:
        case KC_UNDS:
        case KC_SLSH:
        case KC_LBRC:
        case KC_RBRC:
        case KC_SCLN:
        case KC_QUOT:
        case KC_EQL:
        case KC_GRV:
            set_shifted_key(keycode, keycode, record);
            return false;
        case CAPS_WORD:
            caps_word_on();
            break;
        case MO(2):
            if (record->event.pressed) {
                caps_word_off();
                clear_mods();
                clear_oneshot_mods();
            }
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
        _______,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                             KC_J,    KC_L,    KC_U,     KC_Y,    KC_QUOT,  _______,
        KC_COMM,   KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                             KC_M,    KC_N,    KC_E,     KC_I,    KC_O,     KC_MINS,
        _______,  KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                             KC_K,    KC_H,    KC_UNDS,  KC_DOT,  KC_SLSH,  _______,
                         KC_LGUI, KC_SPC, MO(2),                             OSM(MOD_LSFT), OSL(1), CAPS_WORD
    ),
    [1] = LAYOUT_split_3x6_3(
        _______, KC_GRV, KC_LBRC, KC_EXLM, KC_RBRC, KC_LT,                KC_GT,  KC_PERC,    KC_EQL,    KC_PLUS,    KC_AT,    _______,
        KC_CIRC, KC_QUES, KC_LPRN,  KC_DQT, KC_RPRN, KC_TAB,                KC_BSPC,  KC_ENT, KC_ESC, KC_COLN, KC_SCLN, KC_DLR,
        _______, _______,   KC_LCBR, KC_AMPR, KC_RCBR, _______,                KC_DEL, KC_PIPE, KC_HASH, KC_ASTR, KC_TILD,    _______,
                                  _______, KC_SPC, MO(4),                _______,  _______,  _______
    ),
    [2] = LAYOUT_split_3x6_3(
        KC_6,     KC_7,    KC_8,    KC_9,    KC_0,    LT_LT,                GT_GT,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
        KC_CIRC, LSFT(KC_I), C(KC_U), KC_LEFT, KC_UP, M_5GK,                   M_5GJ, KC_DOWN,  KC_RIGHT, C(KC_D), LSFT(KC_A),  KC_DLR,
        _______, Z_Z,     _______, _______, _______,  _______,                 _______, _______, _______, DOT_DOT_SLSH, TILD_SLSH, _______,
                                    _______, _______, _______,                  KC_LALT, MO(3), MO(5)
    ),
    [3] = LAYOUT_split_3x6_3(
        _______, KC_GRV, KC_LBRC, KC_EXLM, KC_RBRC, KC_LT,                KC_GT,  KC_PERC,    KC_EQL,    KC_PLUS,    KC_AT,    _______,
        KC_CIRC, KC_QUES, KC_LPRN,  KC_DQT, KC_RPRN, KC_TAB,                KC_BSPC,  KC_ENT, KC_ESC, KC_COLN, KC_SCLN, KC_DLR,
        _______, _______,   KC_LCBR, KC_AMPR, KC_RCBR, _______,                KC_DEL, KC_PIPE, KC_HASH, KC_ASTR, KC_TILD,    _______,
                                  _______, _______, _______,                _______,  _______,  _______
    ),
    [4] = LAYOUT_split_3x6_3(
        KC_6,     KC_7,    KC_8,    KC_9,    KC_0,    LT_LT,                GT_GT,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
        KC_CIRC, LSFT(KC_I), C(KC_U), KC_LEFT, KC_UP, M_5GK,                   M_5GJ, KC_DOWN,  KC_RIGHT, C(KC_D), LSFT(KC_A),  KC_DLR,
        _______, Z_Z,     _______, _______, _______,  _______,                 _______, _______, _______, DOT_DOT_SLSH, TILD_SLSH, _______,
                                    _______, _______, _______,                  _______, _______, _______
    ),
    [5] = LAYOUT_split_3x6_3(
        KC_F6,      KC_F6,             KC_F8,           KC_F9,   KC_F10,  KC_F11,                  KC_F12,  KC_F1,           KC_F2,   KC_F3,   KC_F4,   KC_F5,
        _______,    _______,           _______,         _______, _______, _______,                 KC_KB_MUTE, KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP, KC_BRID, KC_BRIU, KC_SYSTEM_SLEEP,
        _______, _______, _______, _______, _______, _______,                 _______, _______,         _______, _______, _______, _______,
                                                        _______,  _______,   _______,          _______, _______, _______
    ),
    [6] = LAYOUT_split_3x6_3(
            _______,           _______,        _______,   _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
           _______,           _______,         _______,  KC_H, KC_K,      _______,                    _______, KC_J, KC_L, _______, _______, _______,
        _______, _______, _______, _______, _______,      _______,                    _______, _______, _______, _______, _______, _______,
                                                                         _______, _______, _______,    _______, _______, _______
    ),
    [7] = LAYOUT_split_3x6_3(
            _______,           _______,        _______,   _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
           _______,           _______,         _______,  TM_LEFT, TM_UP,      _______,                    _______, TM_DOWN, TM_RIGHT, _______, _______, _______,
        _______, _______, _______, _______, _______,      _______,                    _______, _______, _______, _______, _______, _______,
                                                                         _______, _______, _______,    _______, KC_LALT, _______
    ),
};
