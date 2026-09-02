#include QMK_KEYBOARD_H

// Layer definitions
enum layers {
    BASE,
    SYMB,
    TMUX,
    NUMBERS
};

// Custom keycodes for tmux macros
enum custom_keycodes {
    TMUX_WIN_1 = SAFE_RANGE,
    TMUX_WIN_2,
    TMUX_WIN_3,
    TMUX_WIN_4,
    TMUX_WIN_5,
    TMUX_COPY_MODE,
    TMUX_SPLIT_H,
    TMUX_SPLIT_V,
    TMUX_NEW_WIN
};

// Home-row modifiers
#define MY_A LALT_T(KC_A)
#define MY_SCLN LALT_T(KC_SCLN)
#define MY_S LGUI_T(KC_S)
#define MY_D LT(TMUX, KC_D)
#define MY_F LCTL_T(KC_F)
#define MY_G LSFT_T(KC_G)
#define MY_H LSFT_T(KC_H)
#define MY_J LCTL_T(KC_J)
#define MY_L LGUI_T(KC_L)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT(
        KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_GRAVE, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
        KC_LCTL,  MY_A,    MY_S,    MY_D,    MY_F,    MY_G,                             MY_H,    MY_J,    KC_K,    MY_L,    MY_SCLN, KC_QUOT,
        KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_EQUAL,
        _______,  _______, _______, _______, KC_LCMD, MO(NUMBERS), KC_ENT,     KC_SPC,  MO(SYMB),KC_TAB,  _______, _______, _______, _______
    ),

    [SYMB] = LAYOUT(
        _______,  _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
        KC_TILD,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
        _______,  KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR, _______,                          KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,KC_QUES, KC_BSLS,
        _______,  _______, _______, KC_LPRN, KC_RPRN, _______,                          _______, _______, _______, _______, _______, _______,
        _______,  _______, _______, _______, _______, _______,     _______,    _______, _______, _______, _______, _______, _______, _______
    ),

    [TMUX] = LAYOUT(
        _______,  _______, _______, _______, _______, _______,                          _______, _______,        _______,      _______,      _______,      _______,
        _______,  _______, _______, _______, _______, _______,                          _______, TMUX_COPY_MODE, TMUX_SPLIT_H, TMUX_SPLIT_V, _______,      _______,
        _______,  _______, _______, _______, _______, _______,                          _______, TMUX_WIN_1,     TMUX_WIN_2,   TMUX_WIN_3,   TMUX_WIN_4,   TMUX_WIN_5,
        _______,  _______, _______, _______, _______, _______,                          TMUX_NEW_WIN, _______,   _______,      _______,      _______,      _______,
        _______,  _______, _______, _______, _______, _______,     _______,    _______, _______, _______,        _______,      _______,      _______,      _______
    ),

    [NUMBERS] = LAYOUT(
        QK_BOOT,  _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
        _______,  _______, _______, _______, _______, _______,                          _______, KC_7,    KC_8,    KC_9,    _______, KC_MUTE,
        _______,  _______, _______, _______, _______, _______,                          _______, KC_4,    KC_5,    KC_6,    _______, KC_VOLU,
        _______,  _______, _______, _______, _______, _______,                          _______, KC_1,    KC_2,    KC_3,    _______, KC_VOLD,
        _______,  _______, _______, _______, _______, _______,     _______,    _______, KC_TAB,  KC_0,    _______, _______, _______, _______
    )
};

// Helper function: sends Ctrl+Space (tmux prefix) followed by a key
void send_tmux_key(const char* key) {
    SEND_STRING(SS_LCTL(" "));
    send_string(key);
}

// Custom keycode processing
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {
        case TMUX_WIN_1:      send_tmux_key("1"); return false;
        case TMUX_WIN_2:      send_tmux_key("2"); return false;
        case TMUX_WIN_3:      send_tmux_key("3"); return false;
        case TMUX_WIN_4:      send_tmux_key("4"); return false;
        case TMUX_WIN_5:      send_tmux_key("5"); return false;
        case TMUX_COPY_MODE:  send_tmux_key("["); return false;
        case TMUX_SPLIT_H:    send_tmux_key(SS_LSFT("'")); return false;
        case TMUX_SPLIT_V:    send_tmux_key(SS_LSFT("5")); return false;
        case TMUX_NEW_WIN:    send_tmux_key("c"); return false;
    }
    return true;
}
