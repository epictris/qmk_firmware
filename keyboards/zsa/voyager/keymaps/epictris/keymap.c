// Copyright 2023 ZSA Technology Labs, Inc <@zsa>
// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum custom_keycodes {
    MOD_TMUX = SAFE_RANGE,
    MOD_CTL_W,
    MOD_SHIFT,
    TILD_SLSH,
    SHEBANG,
    SLSH_GT,
    TM1,
    TM2,
    TM3,
    TM4,
    TM5,
    LAYOUT_MAC,
    LAYOUT_LINUX,
    COPY,
    CUT,
    PASTE,
    UNDO,
    ALL,
    FIND
};

// RGB_MATRIX_EFFECT(custom_rgb)
//
// #ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
//
// static bool custom_rgb() {
//     RGB_MATRIX_USE_LIMITS(led_min; led_max);
//     for (uint8_t i = led_min; i < led_nax; i++) {
//         rgb_matrix_set_color(i, i, i, i);
//     }
//     return rbg_matrix_check_finished_leds(led_max);
// }
//
// #endif

bool mac_layout = true;

void apply_tmux_prefix(uint16_t keycode) {
    tap_code16(C(KC_B));
    tap_code(keycode);
}


void execute_macro(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TM1:
            return apply_tmux_prefix(KC_1);
        case TM2:
            return apply_tmux_prefix(KC_2);
        case TM3:
            return apply_tmux_prefix(KC_3);
        case TM4:
            return apply_tmux_prefix(KC_4);
        case TM5:
            return apply_tmux_prefix(KC_5);
        case TILD_SLSH:
            return SEND_STRING("~/");
        case SHEBANG:
            return SEND_STRING("#!/");
        case SLSH_GT:
            return SEND_STRING("/>");
    }
}
uint16_t mod_ctl_w_release_key = KC_NO;
bool mod_ctl_w_active = false;

void apply_mod_ctl_w(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case OSL(1):
        case MO(2):
        case MOD_SHIFT:
        case OSM(KC_LSFT):
            break;
        case MOD_CTL_W:
            if (record->event.pressed) {
                mod_ctl_w_active = true;
                mod_ctl_w_release_key = C(KC_W);
            }
            else {
                mod_ctl_w_active = false;
                tap_code16(mod_ctl_w_release_key);
            }
            break;
        default:
            if (record->event.pressed) {
                if (mod_ctl_w_active) {
                    mod_ctl_w_release_key = KC_NO;
                    tap_code16(C(KC_W));
                }
            }
    }
}

uint16_t mod_tmux_release_key = KC_NO;
bool mod_tmux_active = false;

void apply_mod_tmux(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case OSL(1):
        case MO(2):
        case MOD_SHIFT:
        case OSM(KC_LSFT):
        case KC_LALT:
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
    } else {
    }
    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
    return true;
}

const uint16_t PROGMEM combo_gui_a[] = {KC_P, KC_B,  COMBO_END};
const uint16_t PROGMEM combo_gui_f[] = {KC_T, KC_G,  COMBO_END};
const uint16_t PROGMEM combo_gui_z[] = {KC_D, KC_V,  COMBO_END};
const uint16_t PROGMEM combo_gui_x[] = {KC_Z, KC_X,  COMBO_END};
const uint16_t PROGMEM combo_gui_c[] = {KC_X, KC_F,  COMBO_END};
const uint16_t PROGMEM combo_gui_v[] = {KC_F, KC_D,  COMBO_END};

const uint16_t PROGMEM combo_gui_n[] = {KC_M, KC_N,  COMBO_END};
const uint16_t PROGMEM combo_gui_k[] = {KC_K, KC_H,  COMBO_END};

const uint16_t PROGMEM combo_ctl1[] = {KC_S, KC_T,  COMBO_END};
const uint16_t PROGMEM combo_ctl2[] = {KC_N, KC_E,  COMBO_END};

const uint16_t PROGMEM combo_sft1[] = {KC_R, KC_S,  COMBO_END};
const uint16_t PROGMEM combo_sft2[] = {KC_E, KC_I,  COMBO_END};

const uint16_t PROGMEM combo_ctl_sft1[] = {KC_R, KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM combo_ctl_sft2[] = {KC_N, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM combo_gui1[] = {KC_C, KC_P,  COMBO_END};
const uint16_t PROGMEM combo_gui2[] = {KC_L, KC_U,  COMBO_END};
const uint16_t PROGMEM combo_gui_sft1[] = {KC_W, KC_C, KC_P, COMBO_END};
const uint16_t PROGMEM combo_gui_sft2[] = {KC_L, KC_U, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_ctl_sft_c[] = {KC_X, KC_F, KC_D, COMBO_END};
const uint16_t PROGMEM combo_ctl_sft_v[] = {KC_F, KC_D, KC_V, COMBO_END};

const uint16_t PROGMEM combo_mod_tmux1[] = {KC_W, KC_C,  COMBO_END};
const uint16_t PROGMEM combo_mod_tmux2[] = {KC_U, KC_Y,  COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_gui1, KC_LGUI),
    COMBO(combo_gui2, KC_LGUI),
    COMBO(combo_gui_sft1, G(KC_LSFT)),
    COMBO(combo_gui_sft2, G(KC_LSFT)),
    COMBO(combo_ctl1, KC_LCTL),
    COMBO(combo_ctl2, KC_LCTL),
    COMBO(combo_sft1, KC_LSFT),
    COMBO(combo_sft2, KC_LSFT),
    COMBO(combo_ctl_sft1, C(KC_LSFT)),
    COMBO(combo_ctl_sft2, C(KC_LSFT)),
    COMBO(combo_mod_tmux1, MOD_TMUX),
    COMBO(combo_mod_tmux2, MOD_TMUX),
    COMBO(combo_gui_a, G(KC_A)),
    COMBO(combo_gui_f, G(KC_F)),
    COMBO(combo_gui_z, G(KC_Z)),
    COMBO(combo_gui_x, G(KC_X)),
    COMBO(combo_gui_c, G(KC_C)),
    COMBO(combo_gui_v, G(KC_V)),
    COMBO(combo_gui_k, G(KC_K)),
    COMBO(combo_gui_n, G(KC_N)),
    COMBO(combo_ctl_sft_c, C(S(KC_C))),
    COMBO(combo_ctl_sft_v, C(S(KC_V)))
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
    if (is_caps_word_on()) {
        set_mods(MOD_BIT(KC_LSFT));
    }
    add_mods(held_key_mods);
    tap_code16(held_key);
    clear_mods();
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

void use_os_binding(uint16_t keycode, keyrecord_t *record) {
    if (record ->event.pressed) {
        if (mac_layout) {
            switch (keycode) {
                case COPY:
                    return tap_code16(G(KC_C));
                case CUT:
                    return tap_code16(G(KC_X));
                case PASTE:
                    return tap_code16(G(KC_V));
                case UNDO:
                    return tap_code16(G(KC_Z));
                case ALL:
                    return tap_code16(G(KC_A));
                case FIND:
                    return tap_code16(G(KC_F));
            }
        } else {
            switch (keycode) {
                case COPY:
                    return tap_code16(C(KC_C));
                case CUT:
                    return tap_code16(C(KC_X));
                case PASTE:
                    return tap_code16(C(KC_V));
                case UNDO:
                    return tap_code16(C(KC_Z));
                case ALL:
                    return tap_code16(C(KC_A));
                case FIND:
                    return tap_code16(C(KC_F));
            }
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    apply_mod_tmux(keycode, record);
    apply_mod_ctl_w(keycode, record);
    apply_mod_bsls(keycode, record);
    break_caps_word(keycode, record);
    use_os_binding(keycode, record);

    switch (keycode) {
        case LAYOUT_MAC:
            mac_layout = true;
            break;
        case LAYOUT_LINUX:
            mac_layout = false;
            break;
        case KC_1 ... KC_0:
            set_held_key(keycode, record);
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
            set_held_key(keycode, record);
    }

    switch (keycode) {
        case TM1:
        case TM2:
        case TM3:
        case TM4:
        case TM5:
        case TILD_SLSH:
        case SHEBANG:
        case SLSH_GT:
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
                switch (held_key) {
                    case KC_NO:
                        register_code(KC_LSFT);
                        // set_mods(MOD_BIT(KC_LSFT));
                        mod_interrupt = false;
                        break;
                    default:
                        return repeat_held_key(keycode, record);
                }
            } else {
                unregister_code(KC_LSFT);
                clear_mods();
                if (!mod_interrupt) {
                    set_oneshot_mods(MOD_BIT(KC_LSFT));
                }
            }
            break;
        case MO(2):
            if (record->event.pressed) {;;
                if (held_key != KC_NO) {
                    switch (held_key) {
                        case KC_1 ... KC_0:
                            clear_mods();
                            tap_code(KC_DOT);
                            return false;
                        default:
                            return repeat_held_key(keycode, record);
                    }
                } else {
                    if (get_mods() == MOD_BIT(KC_LSFT)) {
                        caps_word_on();
                        break;
                    }
                    caps_word_off();
                }
                clear_mods();
            } else {
                unregister_code(KC_LSFT);
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
        // case MO(2):
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
        KC_COMM,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                             KC_M,    KC_N,    KC_E,     KC_I,   KC_O,     KC_QUES,
        KC_KB_VOLUME_DOWN,  KC_Z,    KC_X,    KC_F,    KC_D,    KC_V,                             KC_K,    KC_H,    KC_UNDS,  KC_DOT,  KC_SLSH,  KC_KB_VOLUME_UP,
                                   KC_SPC, MO(2),                             MOD_SHIFT, OSL(1)
    ),
    [1] = LAYOUT(
        MO(4), KC_7,     KC_8,    KC_9,    KC_0,    KC_F11,                    KC_F12,  KC_1,    KC_2,    KC_3,    KC_4,    MO(4),
        KC_6, KC_GRV, KC_LBRC, KC_TAB, KC_LT, KC_RBRC,                         KC_PLUS,  KC_GT,    KC_MINS,    KC_EXLM,    KC_AT,    KC_5,
        KC_CIRC, KC_SCLN, KC_LPRN,  KC_DQT, KC_EQL, KC_RPRN,                  KC_BSPC,  KC_ENT, KC_ESC, KC_COLN, KC_BSLS, KC_DLR,
        KC_BRID, KC_TILD,   KC_LCBR, KC_HASH, KC_PERC, KC_RCBR,                KC_AMPR, KC_PIPE, KC_ASTR, _______, TILD_SLSH, KC_BRIU,
                                            KC_SPC, MO(2),                  _______,  _______
    ),
    [2] = LAYOUT(
        MO(4),   KC_7,    KC_8,    KC_9,    KC_0,     KC_F11,                  KC_F12,  KC_1,    KC_2,    KC_3,    KC_4,          MO(4),
        KC_6,    _______, KC_HOME, KC_UP,   KC_END,   FIND,                    _______, C(KC_L), C(KC_U), _______, _______, TM5,
        S(KC_I), KC_LALT, KC_LEFT, KC_DOWN, KC_RIGHT, KC_DEL,                  _______, C(KC_N), C(KC_D), C(KC_I), C(KC_O),       S(KC_A),
        RGB_VAD, FIND,    CUT,     COPY,    PASTE,    UNDO,                    _______, _______, _______, _______, SHEBANG,       RGB_VAI,
                                            _______, MO(2),                    KC_MEH, MO(3)
    ),
    [3] = LAYOUT(
        MO(4), KC_7,     KC_8,    KC_9,    KC_0,    KC_F11,                    KC_F12,  KC_1,    KC_2,    KC_3,    KC_4,    MO(4),
        KC_6, KC_GRV, KC_LBRC, KC_TAB, KC_LT, KC_RBRC,                         KC_PLUS,  KC_GT,    KC_MINS,    KC_EXLM,    KC_AT,    KC_5,
        KC_CIRC, KC_SCLN, KC_LPRN,  KC_DQT, KC_EQL, KC_RPRN,                  KC_BSPC,  KC_ENT, KC_ESC, KC_COLN, KC_BSLS, KC_DLR,
        KC_BRID, KC_TILD,   KC_LCBR, KC_HASH, KC_PERC, KC_RCBR,                KC_AMPR, KC_PIPE, KC_ASTR, _______, _______, KC_BRIU,
                                            KC_SPC, MO(2),                  _______,  _______
    ),
    [4] = LAYOUT(
        _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,                  KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______,
        KC_F6,   _______, _______, _______, _______, _______,                 _______, _______, _______, _______, _______, KC_F5,
        _______, _______, _______, _______, _______, _______,                 _______, _______, _______, _______, _______, _______,
        LAYOUT_MAC, _______, _______, _______, _______, _______,                 _______, _______, _______, _______, _______, LAYOUT_LINUX,
                                                     _______, _______,        _______, _______
    ),
};
