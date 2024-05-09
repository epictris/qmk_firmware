// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum custom_keycodes {
    MOD_TMUX = SAFE_RANGE,
    MOD_SHIFT,
    TILD_SLSH,
    Z_Z,
    TM0,
    TM1,
    TM2,
    TM3,
};

void apply_tmux_prefix(uint16_t keycode) {
    tap_code16(C(KC_B));
    tap_code(keycode);
}


void execute_macro(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
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

const uint16_t PROGMEM combo_alt_bspc[] = {KC_M, KC_N,  COMBO_END};
const uint16_t PROGMEM combo_gui1[] = {KC_C, KC_D,  COMBO_END};
const uint16_t PROGMEM combo_gui2[] = {KC_H, KC_COMM,  COMBO_END};
const uint16_t PROGMEM combo_ctl1[] = {KC_S, KC_T,  COMBO_END};
const uint16_t PROGMEM combo_ctl_sft1[] = {KC_R, KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM combo_ctl_sft2[] = {KC_N, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM combo_ctl2[] = {KC_N, KC_E,  COMBO_END};
const uint16_t PROGMEM combo_sft1[] = {KC_X, KC_C,  COMBO_END};
const uint16_t PROGMEM combo_sft2[] = {KC_COMM, KC_DOT,  COMBO_END};
const uint16_t PROGMEM combo_alt1[] = {KC_A, KC_R,  COMBO_END};
const uint16_t PROGMEM combo_alt2[] = {KC_I, KC_O,  COMBO_END};
const uint16_t PROGMEM combo_ctl_b1[] = {KC_R, KC_S,  COMBO_END};
const uint16_t PROGMEM combo_ctl_b2[] = {KC_E, KC_I,  COMBO_END};
const uint16_t PROGMEM combo_tmux0[] = {KC_W, KC_F, COMBO_END};
const uint16_t PROGMEM combo_tmux1[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_tmux2[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM combo_tmux3[] = {KC_U, KC_Y, COMBO_END};


combo_t key_combos[] = {
    COMBO(combo_alt_bspc, A(KC_BSPC)),
    COMBO(combo_gui1, KC_LGUI),
    COMBO(combo_gui2, KC_LGUI),
    COMBO(combo_ctl_sft1, C(KC_LSFT)),
    COMBO(combo_ctl_sft2, C(KC_LSFT)),
    COMBO(combo_ctl1, KC_LCTL),
    COMBO(combo_ctl2, KC_LCTL),
    COMBO(combo_sft1, KC_LSFT),
    COMBO(combo_sft2, KC_LSFT),
    COMBO(combo_alt1, KC_LALT),
    COMBO(combo_alt2, KC_LALT),
    COMBO(combo_ctl_b1, MOD_TMUX),
    COMBO(combo_ctl_b2, MOD_TMUX),
    COMBO(combo_tmux0, TM0),
    COMBO(combo_tmux1, TM1),
    COMBO(combo_tmux2, TM2),
    COMBO(combo_tmux3, TM3),
};

uint16_t held_key = KC_NO;
uint8_t held_key_mods = 0;

void set_held_key(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        held_key = keycode;
        held_key_mods = get_oneshot_mods() | get_mods();
    } else {
        if (held_key == keycode) {
            held_key = KC_NO;
            held_key_mods = 0;
        }
    }
}

bool repeat_held_key(uint16_t keycode, keyrecord_t *record ) {
    add_mods(held_key_mods);
    tap_code16(held_key);
    clear_mods();
    return false;
}

bool mod_interrupt = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    apply_mod_tmux(keycode, record);
    apply_mod_bsls(keycode, record);


    switch (keycode) {
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
        case KC_QUES:
        case KC_EXLM:
        case KC_QUOT:
        case KC_COMM:
        case KC_DOT:
        case KC_SLSH:
        case KC_GRV:
        case KC_LBRC:
        case KC_DQT:
        case KC_RBRC:
        case KC_LT:
        case KC_GT:
        case KC_PLUS:
        case KC_EQL:
        case KC_MINS:
        case KC_AT:
        case KC_CIRC:
        case KC_BSLS:
        case KC_LPRN:
        case KC_UNDS:
        case KC_RPRN:
        case KC_TAB:
        case KC_BSPC:
        case KC_ENT:
        // case KC_ESC:
        case KC_COLN:
        case KC_SCLN:
        case KC_DLR:
        case KC_TILD:
        case KC_LCBR:
        case KC_ASTR:
        case KC_RCBR:
        case KC_PIPE:
        case KC_HASH:
        case KC_AMPR:
        case KC_PERC:
            set_held_key(keycode, record);
    }

    switch (keycode) {
        case TM0:
        case TM1:
        case TM2:
        case TM3:
        case Z_Z:
        case TILD_SLSH:
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
        case MOD_SHIFT:
            if (record->event.pressed) {
                if (held_key != KC_NO) {
                    return repeat_held_key(keycode, record);
                }
                set_mods(MOD_BIT(KC_LSFT));
                mod_interrupt = false;
            } else {
                clear_mods();
                if (!mod_interrupt) {
                    set_oneshot_mods(MOD_BIT(KC_LSFT));
                }
            }
            break;
        case MO(2):
            if (record->event.pressed) {
                if (held_key != KC_NO) {
                    return repeat_held_key(keycode, record);
                } else {
                    if (get_mods() == MOD_BIT(KC_LSFT)) {
                        caps_word_on();
                        break;
                    }
                    caps_word_off();
                }
            } else {
                clear_mods();
                clear_oneshot_mods();
            }
        case KC_SPC:
            if (record->event.pressed) {
                if (get_mods() == MOD_BIT(KC_LSFT)) {
                    tap_code(KC_SPC);
                    caps_word_on();
                    return false;
                }
            }
    }


    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case MOD_SHIFT:
        case MO(2):
        case OSL(1):
            break;
        default:
            if (record->event.pressed) {
                set_oneshot_mods(0);
                mod_interrupt = true;
            }
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
        _______,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                             KC_J,    KC_L,    KC_U,     KC_Y,   KC_QUOT,  _______,
        KC_QUES,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                             KC_M,    KC_N,    KC_E,     KC_I,   KC_O,     KC_EXLM,
        _______,  KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                             KC_K,    KC_H,    KC_COMM,  KC_DOT,  KC_SLSH,  _______,
                         _______, KC_SPC, MO(2),                             MOD_SHIFT, OSL(1), KC_LSFT
    ),
    [1] = LAYOUT_split_3x6_3(
        _______, KC_GRV, KC_LBRC, KC_DQT, KC_RBRC, KC_LT,                KC_GT,  KC_PLUS,    KC_EQL,    KC_MINS,    KC_AT,    _______,
        KC_CIRC, KC_BSLS, KC_LPRN,  KC_UNDS, KC_RPRN, KC_TAB,                KC_BSPC,  KC_ENT, KC_ESC, KC_COLN, KC_SCLN, KC_DLR,
        _______, KC_TILD,   KC_LCBR, KC_HASH, KC_RCBR, _______,                KC_AMPR, KC_PIPE, KC_PERC, KC_ASTR, TILD_SLSH,    _______,
                                  _______, KC_SPC, MO(2),                _______,  _______,  _______
    ),
    [2] = LAYOUT_split_3x6_3(
        KC_6, KC_7,     KC_8,    KC_0,    KC_9,    _______,                    _______,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
        KC_CIRC, LSFT(KC_I), C(KC_U), KC_LEFT, KC_UP, KC_TAB,                   KC_DEL, KC_DOWN,  KC_RIGHT, C(KC_D), LSFT(KC_A),  KC_DLR,
        _______, Z_Z,     KC_X, G(KC_C), KC_D,  G(KC_V),                 _______, _______, _______, KC_DOT, _______, _______,
                                    _______, _______, MO(2),                 KC_MEH, MO(3), MO(4)
    ),
    [3] = LAYOUT_split_3x6_3(
        _______, KC_GRV, KC_LBRC, KC_DQT, KC_RBRC, KC_LT,                KC_GT,  KC_PLUS,    KC_EQL,    KC_MINS,    KC_AT,    _______,
        KC_CIRC, KC_BSLS, KC_LPRN,  KC_UNDS, KC_RPRN, KC_TAB,                KC_BSPC,  KC_ENT, KC_ESC, KC_COLN, KC_SCLN, KC_DLR,
        _______, KC_TILD,   KC_LCBR, KC_HASH, KC_RCBR, _______,                KC_AMPR, KC_PIPE, KC_PERC, KC_ASTR, TILD_SLSH,    _______,
                                  _______, KC_SPC, _______,                _______,  _______,  _______
    ),
    [4] = LAYOUT_split_3x6_3(
        KC_F6,      KC_F6,             KC_F8,           KC_F9,   KC_F10,  KC_F11,                  KC_F12,  KC_F1,           KC_F2,   KC_F3,   KC_F4,   KC_F5,
        _______,    _______,           _______,         _______, _______, _______,                 KC_KB_MUTE, KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP, KC_BRID, KC_BRIU, KC_SYSTEM_SLEEP,
        _______, _______, _______, _______, _______, _______,                 _______, _______,         _______, _______, _______, _______,
                                                        _______,  _______,   _______,          _______, _______, _______
    )
};
