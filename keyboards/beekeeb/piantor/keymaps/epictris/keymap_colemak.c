// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum custom_keycodes {
    LCTL_HOLD_LGUI_TAP = SAFE_RANGE,
    MOD_BSLS,
    MOD_TMUX,
    TILD_SLSH,
    DOT_DOT_SLSH,
    Z_Z,
    CTL_R_EQL,
    VIM_PGUP,
    VIM_PGDN,
    REP_RIGHT,
    REP_LEFT,
};

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
    switch (keycode) {
        case MOD_TMUX:
            return false;  // Ignore ALTREP keys.
        case REP_RIGHT:
            return false;
        case REP_LEFT:
            return false;
    }

    return true;  // Other keys can be repeated.
}

void execute_macro(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DOT_DOT_SLSH:
            return SEND_STRING("../");
        case TILD_SLSH:
            return SEND_STRING("~/");
        case Z_Z:
            return SEND_STRING("zz");
        case CTL_R_EQL:
            tap_code16(C(KC_R));
            return tap_code16(KC_EQL);
        case VIM_PGUP:
            tap_code16(KC_ESC);
            tap_code16(C(KC_U));
            return SEND_STRING("zz");
        case VIM_PGDN:
            tap_code16(KC_ESC);
            tap_code16(C(KC_D));
            return SEND_STRING("zz");
    }
}
bool LGUI_tap_interrupted = false;
void custom_lctrl_tap_lgui(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LCTL_HOLD_LGUI_TAP:
            if (record->event.pressed) {
                register_mods(MOD_MASK_CTRL);
                LGUI_tap_interrupted = false;
            } else {
                unregister_mods(MOD_MASK_CTRL);
                if (!LGUI_tap_interrupted) {
                    add_oneshot_mods(MOD_BIT(KC_LGUI));
                }
            }
            return;
    }
    LGUI_tap_interrupted = true;
}

uint16_t mod_tmux_release_key = KC_NO;
bool mod_tmux_active = false;

void apply_mod_tmux(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case OSL(1):
        case OSL(2):
        case OSM(KC_LSFT):
        case LCTL_HOLD_LGUI_TAP:
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

bool send_unshifted_key(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (get_oneshot_mods() == MOD_BIT(KC_LSFT)) {
            clear_oneshot_mods();
            tap_code(keycode);
        } else if (get_mods() == MOD_BIT(KC_LSFT)) {
            clear_mods();
            tap_code(keycode);
            add_mods(MOD_BIT(KC_LSFT));
        } else {
            tap_code(keycode);
        }
    }
    return false;
}

int rep_count = 0;
static void process_rep_right(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case KC_C:
            SEND_STRING("d ~/"); break;
        case KC_S:
            SEND_STRING("ed 1000q "); break;
        case KC_LBRC:
            SEND_STRING("0]"); break;
        case KC_LT:
            SEND_STRING("< "); break;
        case KC_Z:
            SEND_STRING("shrc"); break;
        case KC_EXLM:
            SEND_STRING("= 0"); break;
        case KC_D:
            SEND_STRING("ef "); break;
        case KC_P:
            SEND_STRING("ython "); break;
        case KC_T:
            SEND_STRING("mux "); break;
        case KC_TILD:
            SEND_STRING("= "); break;
    }
}

static void process_rep_left(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case KC_DOT:
            if (rep_count == 0) {
                SEND_STRING("./");
            } else {
                SEND_STRING("../");
            } break;
        case KC_GT:
            SEND_STRING("> "); break;
        case KC_RPRN:
            SEND_STRING(": -> "); break;
        case KC_EQL:
            SEND_STRING("= 0"); break;
        case KC_PLUS:
            SEND_STRING("= 1"); break;
        case KC_N:
            SEND_STRING("vim "); break;
        case KC_E:
            SEND_STRING("xit"); break;
    }
}

static void process_repeat_keys(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case REP_RIGHT:
            process_rep_right(get_last_keycode(), get_last_mods());
            rep_count += 1;
            break;
        case REP_LEFT:
            process_rep_left(get_last_keycode(), get_last_mods());
            rep_count += 1;
            break;
        default:
            rep_count = 0;
    }
}

uint16_t mod_backslash_release = KC_NO;
bool mod_backslash = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    custom_lctrl_tap_lgui(keycode, record);
    apply_mod_tmux(keycode, record);

    if (mod_backslash) {
        if (keycode != KC_LSFT && keycode != KC_SPC && keycode !=OSM(1) && keycode != OSM(2)) {
            if (record->event.pressed) {
                tap_code(KC_BSLS);
                mod_backslash_release = KC_NO;
            }
        }
    }
    if (record->event.pressed) {
        process_repeat_keys(keycode, record);
    }

    switch (keycode) {
        case DOT_DOT_SLSH:
        case TILD_SLSH:
        case Z_Z:
        case VIM_PGUP:
        case VIM_PGDN:
        case CTL_R_EQL:
            if (record->event.pressed) {
                execute_macro(keycode, record);
            }
            break;
        case MOD_BSLS:
            if (record->event.pressed) {
                mod_backslash_release = KC_BSLS;
                mod_backslash = true;
            }
            else {
                tap_code(mod_backslash_release);
                mod_backslash = false;
            }
            break;
        case KC_SLSH:
        case KC_COMM:
        case KC_DOT:
        case KC_MINS:
        case KC_QUOT:
            return send_unshifted_key(keycode, record);
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
    Base (0)
         ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
         │ Q │ W │ F │ P │ B │       │ J │ L │ U │ Y │ ' │
     ┌───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┐
     │Tab│ A │ R │ S │ T │ G │       │ M │ N │ E │ I │ O │Ent│
     └───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┘
      ARP│ Z │ X │ C │ D │ V │       │ K │ H │ , │ . │ / │ARP
         └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
                   ┌───┐                   ┌───┐
                   │Ctl├───┐           ┌───┤CAP│
                   └───┤Spc├───┐   ┌───┤OS1├───┘
                       └───┤M02│   │Sft├───┘
                           └───┘   └───┘
    Ctl - is it reannt    tap to add LGUI to next keystroke, hold for LCTL
    Sft - tap to shift next keystroke, hold for LSFT
    Bsp - holding while
*/
        [0] = LAYOUT_split_3x6_3(
        _______,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                             KC_J,    KC_L,    KC_U,     KC_Y,    KC_QUOT,  _______,
        KC_TAB,   KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                             KC_M,    KC_N,    KC_E,     KC_I,    KC_O,     KC_ENT,
        REP_LEFT,  KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                             KC_K,    KC_H,    KC_COMM,  KC_DOT,  KC_SLSH,  REP_RIGHT,
                         LCTL_HOLD_LGUI_TAP, KC_SPC, MO(2),                             OSM(MOD_LSFT), OSL(1), CW_TOGG
    ),
/*
    Symbols/numbers (1)
     ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
     │ 6 │ 7 │ 8 │ 9 │ 0 │ ! │       │ & │ 1 │ 2 │ 3 │ 4 │ 5 │
     ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
     │ # │ [ │ " │ _ │ ( │ ; │       │Bsp│ ) │ - │ : │Esc│ ! │
     └───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┘
         │ < │ { │ ? │ | │ % │       │ * │ = │ + │ } │ > │
         └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
                   ┌───┐                   ┌───┐
                   │ \ ├───┐           ┌───┤   │
                   └───┤Spc├───┐   ┌───┤ x ├───┘
                       └───┤C-b│   │   ├───┘
                           └───┘   └───┘
    \   - tap for \, hold to prefix characters with \
    CAP - Caps Word cd
    C-b - default tmux prefix
*/
/*
    Symbols/numbers (1)
     ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
     │ 6 │ 7 │ 8 │ 9 │ 0 │ % │       │ * │ 1 │ 2 │ 3 │ 4 │ 5 │
     ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
     │ ? │ [ │ " │ _ │ ; │ ( │       │ ) │ = │ - │ : │ ] │ ! │
     └───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┘
         │ { │ < │ & │ | │ + │       │ # │Bsp│Esc│ > │ } │
         └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘

                   ┌───┐                   ┌───┐
                   │ \ ├───┐           ┌───┤   │
                   └───┤Spc├───┐   ┌───┤ x ├───┘
                       └───┤C-b│   │   ├───┘
                           └───┘   └───┘
    \   - tap for \, hold to prefix characters with \
    CAP - Caps Word
    C-b - default tmux prefix
*/
    [1] = LAYOUT_split_3x6_3(
        KC_6,     KC_7,    KC_8,    KC_9,    KC_0,    KC_PERC,                KC_ASTR,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
        KC_QUES, KC_LBRC, KC_DQT,  KC_UNDS, KC_SCLN, KC_LPRN,                KC_RPRN,  KC_EQL,  KC_MINS, KC_COLN, KC_RBRC, KC_EXLM,
        _______, KC_LCBR,   KC_LT, KC_AMPR, KC_PIPE, KC_PLUS,                KC_HASH,  KC_BSPC,   KC_ESC, KC_GT, KC_RCBR,    _______,
                                  MOD_BSLS, KC_SPC, MOD_TMUX,                _______,  _______,  _______
    ),


/*
    Navigation (2)
     ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
     │ 6 │ 7 │ 8 │ 9 │ 0 │PDN│       │PGU│ 1 │ 2 │ 3 │ 4 │ 5 │
     ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
     │   │Alt│Sft│VUP│VDN│ ~ │       │Del│ ↑ │ ↓ │ @ │AR │ ` │
     └───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┘
         │zz │CRE│ ^ │ $ │HOM│       │END│ ← │ → │   │~/ │
         └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
                   ┌───┐                   ┌───┐
                   │   ├───┐           ┌───┤Esc│
                   └───┤   ├───┐   ┌───┤REP├───┘
                       └───┤ x │   │   ├───┘
                           └───┘   └───┘
    REP - repeat last keystroke/combination
    ARP - repeat last keystroke/combination
    CRE - Ctl-r + = (execute code in insert mode nvim)
    VUP - Esc Ctl-u + zz
    VDN - Esc Ctl-d + zz
*/
    [2] = LAYOUT_split_3x6_3(
        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_PGDN,                  KC_PGUP,   KC_1,   KC_2,    KC_3,         KC_4,      KC_5,
        _______, KC_LALT, KC_LSFT, VIM_PGUP, VIM_PGDN, KC_TILD,                   KC_DEL, KC_UP,  KC_DOWN, KC_AT,       KC_GRV,  _______,
        _______, Z_Z,     CTL_R_EQL, KC_CIRC, KC_DLR,  KC_HOME,                 KC_END, KC_LEFT, KC_RIGHT, DOT_DOT_SLSH, TILD_SLSH, _______,
                                        _______, OSL(3), _______,                  _______, G(KC_LSFT), KC_ESC
    ),
/*
    Function/System Keys (3)
     ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
     │F6 │F7 │F8 │F9 │F10│F11│       │F12│F1 │F2 │F3 │F4 │F5 │
     ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
     │   │   │   │   │   │   │       │   │   │   │   │   │   │
     └───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┘
         │SB-│SB+│VL-│VL+│VL0│       │   │LCK│   │   │   │
         └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
                   ┌───┐                   ┌───┐
                   │   ├───┐           ┌───┤Esc│
                   └───┤ x ├───┐   ┌───┤   ├───┘
                       └───┤   │   │TO4├───┘
                           └───┘   └───┘
    VL- - Reduce volume
    VL+ - Increase volume
    VL0 - Mute
    SB+ - Increase screen brightness
    SB- - Reduce screen brightness
    LCK - Lock screen
*/
    [3] = LAYOUT_split_3x6_3(
        KC_F6,      KC_F7,             KC_F8,           KC_F9,   KC_F10,  KC_F11,                  KC_F12,  KC_F1,           KC_F2,   KC_F3,   KC_F4,   KC_F5,
        _______,    _______,           _______,         _______, _______, _______,                 _______, _______, _______, _______, _______, _______,
        _______, KC_BRID, KC_BRIU, KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP, KC_KB_MUTE,                 _______, KC_SYSTEM_SLEEP,         _______, _______, _______, _______,
                                                        _______,  _______,   _______,          TO(4), _______,  KC_ESC
    ),

/*
    Left-hand-only mode (4)
     ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
     │VL0│VL-│VL+│SB-│SB+│   │       │   │   │   │   │   │   │
     ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
     │TAB│PUP│PDN│ ↑ │ ↓ │STB│       │   │   │   │   │   │   │
     └───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┘
         │HOM│END│ ← │ → │   │       │   │   │   │   │   │
         └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
                   ┌───┐                   ┌───┐
                   │Ctl├───┐           ┌───┤TO0│
                   └───┤MO5├───┐   ┌───┤   ├───┘
                       └───┤Sft│   │   ├───┘
                           └───┘   └───┘
    STB - LSFT + TAB
*/
    [4] = LAYOUT_split_3x6_3(
        KC_KB_MUTE, KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP, KC_BRID, KC_BRIU,      _______,                    _______, _______, _______, _______, _______, _______,
            KC_TAB,           KC_PGUP,        KC_PGDN,   KC_UP, KC_DOWN, LSFT(KC_TAB),                    _______, _______, _______, _______, _______, _______,
           _______,           KC_HOME,         KC_END,  KC_LEFT, KC_RIGHT,      _______,                    _______, _______, _______, _______, _______, _______,
                                                                         OSM(KC_LCTL), MO(5), OSM(KC_LSFT),    _______, _______, TO(0)
    ),
/*
    Left-hand shortcuts (5)
     ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
     │   │   │   │   │   │   │       │   │   │   │   │   │   │
     ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
     │GTB│⌘-Z│⌘-X│⌘-C│⌘-V│   │       │   │   │   │   │   │   │
     └───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┘
         │SB-│SB+│VL-│VL+│VL0│       │   │   │   │   │   │
         └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
                   ┌───┐                   ┌───┐
                   │   ├───┐           ┌───┤   │
                   └───┤ x ├───┐   ┌───┤   ├───┘
                       └───┤   │   │   ├───┘
                           └───┘   └───┘
    GTB - LGUI + TAB
*/
    [5] = LAYOUT_split_3x6_3(
        _______,   _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
        G(KC_TAB), G(KC_Z), G(KC_X), G(KC_C), G(KC_V), _______,                  _______, _______, _______, _______, _______, _______,
        _______, KC_BRID, KC_BRIU, KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP, KC_KB_MUTE,                 _______, _______,         _______, _______, _______, _______,
                                     _______, _______, _______,                  _______, _______, _______
    ),
};
