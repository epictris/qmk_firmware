// Copyright 2023 ZSA Technology Labs, Inc <@zsa>
// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum custom_keycodes {
    MOD_TMUX = SAFE_RANGE,
    MOD_SHIFT,
    TILD_SLSH,
    SHEBANG,
    SLSH_GT,
    TM1,
    TM2,
    TM3,
    TM4,
    TM5,
};

#define TMUX_PREFIX C(KC_B)


void apply_prefix(uint16_t prefix, uint16_t keycode) {
    tap_code16(prefix);
    tap_code16(keycode);
}


uint16_t prefix_modifier = KC_NO;
uint16_t last_keypress = KC_NO;


bool execute_macro(uint16_t keycode) {
    switch (keycode) {
        case TM1:
            apply_prefix(TMUX_PREFIX, KC_1);
            return true;
        case TM2:
            apply_prefix(TMUX_PREFIX, KC_2);
            return true;
        case TM3:
            apply_prefix(TMUX_PREFIX, KC_3);
            return true;
        case TM4:
            apply_prefix(TMUX_PREFIX, KC_4);
            return true;
        case TM5:
            apply_prefix(TMUX_PREFIX, KC_5);
            return true;
        case TILD_SLSH:
            SEND_STRING("~/");
            return true;
        case SHEBANG:
            SEND_STRING("#!/");
            return true;
        case SLSH_GT:
            SEND_STRING("/>");
            return true;
    }
    return false;
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
    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
    return true;
}

const uint16_t PROGMEM combo_gui_a[] = {KC_QUES, KC_A,  COMBO_END};
const uint16_t PROGMEM combo_gui_f[] = {KC_T, KC_G,  COMBO_END};
const uint16_t PROGMEM combo_gui_z[] = {KC_D, KC_V,  COMBO_END};
const uint16_t PROGMEM combo_gui_x[] = {KC_Z, KC_X,  COMBO_END};
const uint16_t PROGMEM combo_gui_c[] = {KC_X, KC_F,  COMBO_END};
const uint16_t PROGMEM combo_gui_v[] = {KC_F, KC_D,  COMBO_END};

const uint16_t PROGMEM combo_gui_n[] = {KC_M, KC_N,  COMBO_END};
const uint16_t PROGMEM combo_gui_k[] = {KC_K, KC_H,  COMBO_END};

const uint16_t PROGMEM combo_ctl1[] = {KC_S, KC_T,  COMBO_END};
const uint16_t PROGMEM combo_ctl2[] = {KC_N, KC_E,  COMBO_END};
const uint16_t PROGMEM combo_ctl_sft1[] = {KC_R, KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM combo_ctl_sft2[] = {KC_N, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM combo_gui1[] = {KC_C, KC_P,  COMBO_END};
const uint16_t PROGMEM combo_gui2[] = {KC_L, KC_U,  COMBO_END};
const uint16_t PROGMEM combo_gui_sft1[] = {KC_W, KC_C, KC_P, COMBO_END};
const uint16_t PROGMEM combo_gui_sft2[] = {KC_L, KC_U, KC_Y, COMBO_END};

const uint16_t PROGMEM combo_mod_tmux1[] = {KC_W, KC_C,  COMBO_END};
const uint16_t PROGMEM combo_mod_tmux2[] = {KC_U, KC_Y,  COMBO_END};


combo_t key_combos[] = {
    COMBO(combo_gui1, KC_LGUI),
    COMBO(combo_gui2, KC_LGUI),
    COMBO(combo_gui_sft1, G(KC_LSFT)),
    COMBO(combo_gui_sft2, G(KC_LSFT)),
    COMBO(combo_ctl1, KC_LCTL),
    COMBO(combo_ctl2, KC_LCTL),
    COMBO(combo_ctl_sft1, C(KC_LSFT)),
    COMBO(combo_ctl_sft2, C(KC_LSFT)),
    COMBO(combo_mod_tmux1, TMUX_PREFIX),
    COMBO(combo_mod_tmux2, TMUX_PREFIX),
    COMBO(combo_gui_a, G(KC_A)),
    COMBO(combo_gui_f, G(KC_F)),
    COMBO(combo_gui_z, G(KC_Z)),
    COMBO(combo_gui_x, G(KC_X)),
    COMBO(combo_gui_c, G(KC_C)),
    COMBO(combo_gui_v, G(KC_V)),
    COMBO(combo_gui_k, G(KC_K)),
    COMBO(combo_gui_n, G(KC_N)),
};

uint16_t held_key = KC_NO;
uint8_t held_key_mods = 0;

void update_held_key(uint16_t keycode, keyrecord_t *record) {
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

bool repeat_held_key(keyrecord_t *record) {
    if (record->event.pressed) {
        if (is_caps_word_on()) {
            set_mods(MOD_BIT(KC_LSFT));
        }
        add_mods(held_key_mods);
        tap_code16(held_key);
        clear_mods();
    }
    return false;
}

bool mod_interrupt = false;

void keyboard_post_init_user(void) {
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_custom_rgb);
    rgb_matrix_set_speed(50);
}

void break_caps_word(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_DOT:
            caps_word_off();
    }
}

bool update_prefix_modifier(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        prefix_modifier = keycode;
    } else {
        prefix_modifier = KC_NO;
    }
    return true;
}

void apply_prefix_modifier(void) {
    switch (last_keypress) {
        case KC_BSLS:
        case TMUX_PREFIX:
            return;
    }
    tap_code16(prefix_modifier);
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (execute_macro(keycode)) {
            return false;
        }
    }

    switch (keycode) {
        case KC_BSLS:
        case TMUX_PREFIX:
            return update_prefix_modifier(keycode, record);
        case MOD_SHIFT:
            if (held_key != KC_NO) {
                return repeat_held_key(record);
            }
            if (record->event.pressed) {
                caps_word_off();
                register_code(KC_LSFT);
            } else {
                unregister_code(KC_LSFT);
            }
            return false;
        case MO(2):
            if (held_key != KC_NO) {
                return repeat_held_key(record);
            }
            if (record->event.pressed) {
                if (get_mods() == MOD_BIT(KC_LSFT)) {
                    caps_word_on();
                    return false;
                }
                unregister_code(KC_LSFT);
                caps_word_off();
                clear_mods();
            }
            return true;
        case OSL(1):
            return true;
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
    }

    if (record->event.pressed) {
        apply_prefix_modifier();
    }

    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        last_keypress = keycode;
    }

    switch(keycode) {
        case KC_1 ... KC_0:
        case KC_A ... KC_Z:
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
            update_held_key(keycode, record);
    }

    switch(keycode) {
        case MOD_SHIFT:
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
    [0] = LAYOUT(
        MO(4), KC_7,     KC_8,    KC_9,    KC_0,    KC_F11,                    KC_F12,  KC_1,    KC_2,    KC_3,    KC_4,    MO(4),
        KC_6,  KC_Q,    KC_W,    KC_C,    KC_P,    KC_B,                             KC_J,    KC_L,    KC_U,     KC_Y,   KC_QUOT,  KC_5,
        KC_QUES,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                             KC_M,    KC_N,    KC_E,     KC_I,   KC_O,     KC_COMM,
        KC_KB_VOLUME_DOWN,  KC_Z,    KC_X,    KC_F,    KC_D,    KC_V,                             KC_K,    KC_H,    KC_UNDS,  KC_DOT,  KC_SLSH,  KC_KB_VOLUME_UP,
                                   KC_SPC, MO(2),                             MOD_SHIFT, OSL(1)
    ),
    [1] = LAYOUT(
        MO(4), KC_7,     KC_8,    KC_9,    KC_0,    KC_F11,                    KC_F12,  KC_1,    KC_2,    KC_3,    KC_4,    MO(4),
        KC_6, KC_GRV, KC_LBRC, KC_TAB, KC_LT, KC_RBRC,                         KC_PLUS,  KC_GT,    KC_MINS,    KC_EXLM,    KC_AT,    KC_5,
        KC_CIRC, KC_BSLS, KC_LPRN,  KC_DQT, KC_EQL, KC_RPRN,                  KC_BSPC,  KC_ENT, KC_ESC, KC_COLN, KC_SCLN, KC_DLR,
        KC_BRID, KC_TILD,   KC_LCBR, KC_HASH, KC_PERC, KC_RCBR,                KC_AMPR, KC_PIPE, KC_ASTR, _______, TILD_SLSH, KC_BRIU,
                                            KC_SPC, MO(2),                  _______,  _______
    ),
    [2] = LAYOUT(
        MO(4), KC_7,     KC_8,    KC_9,    KC_0,    KC_F11,                    KC_F12,  TM1,    TM2,    TM3,    TM4,    MO(4),
        KC_6, C(S(KC_TAB)), KC_HOME, KC_UP, KC_END, C(KC_TAB),              SLSH_GT,  KC_LALT,    C(KC_U),    LSFT(KC_LALT),  KC_LALT,   TM5,
        KC_CIRC, LSFT(KC_I), KC_LEFT, KC_DOWN, KC_RIGHT, KC_DEL,               _______, KC_ENT,  C(KC_D), _______, LSFT(KC_A),  KC_DLR,
        RGB_VAD, _______,  _______, _______, _______,  _______,                 _______, _______, _______, _______, SHEBANG, RGB_VAI,
                                            _______, MO(2),                    KC_MEH, MO(3)
    ),
    [3] = LAYOUT(
        MO(4), KC_7,     KC_8,    KC_9,    KC_0,    KC_F11,                    KC_F12,  KC_1,    KC_2,    KC_3,    KC_4,    MO(4),
        KC_6, KC_GRV, KC_LBRC, KC_TAB, KC_LT, KC_RBRC,                         KC_PLUS,  KC_GT,    KC_MINS,    KC_EXLM,    KC_AT,    KC_5,
        KC_CIRC, KC_BSLS, KC_LPRN,  KC_DQT, KC_EQL, KC_RPRN,                  KC_BSPC,  KC_ENT, KC_ESC, KC_COLN, KC_SCLN, KC_DLR,
        KC_BRID, KC_TILD,   KC_LCBR, KC_HASH, KC_PERC, KC_RCBR,                KC_AMPR, KC_PIPE, KC_ASTR, _______, _______, KC_BRIU,
                                            KC_SPC, MO(2),                  _______,  _______
    ),
    [4] = LAYOUT(
        _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,                  KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______,
        KC_F6,   _______, _______, _______, _______, _______,                 _______, _______, _______, _______, _______, KC_F5,
        _______, _______, _______, _______, _______, _______,                 _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                 _______, _______, _______, _______, _______, _______,
                                                     _______, _______,        _______, _______
    ),
};
