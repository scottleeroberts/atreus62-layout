enum {
 BASE,
 SYMB,
 TMUX,
 NUMBERS
};

enum {
    M_TMUX_1 = SAFE_RANGE,
    M_TMUX_2,
    M_TMUX_3,
    M_TMUX_4,
    M_TMUX_5,
    M_TMUX_COPY_MODE,
    M_TMUX_SP,
    M_TMUX_VS,
    M_TMUX_NEW
};

#define MY_A LOPT_T(KC_A)
#define MY_SCLN LOPT_T(KC_SCLN)
#define MY_G LSFT_T(KC_G)
#define MY_S LGUI_T(KC_S)
#define MY_D LT(TMUX, KC_D)
#define MY_F CTL_T(KC_F)
#define MY_J CTL_T(KC_J)
#define MY_L LGUI_T(KC_L)
#define MY_H LSFT_T(KC_H)


void do_tmux_key(keyrecord_t *record, uint8_t code, uint8_t modifier) {
  if (record->event.pressed) {
    register_code(KC_LCTL);
    register_code(KC_SPC);
    unregister_code(KC_SPC);
    unregister_code(KC_LCTL);

    register_code(modifier);
    register_code(code);
    unregister_code(code);
    unregister_code(modifier);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
    case M_TMUX_1: do_tmux_key(record, KC_1, KC_NO); break;
    case M_TMUX_2: do_tmux_key(record, KC_2, KC_NO); break;
    case M_TMUX_3: do_tmux_key(record, KC_3, KC_NO); break;
    case M_TMUX_4: do_tmux_key(record, KC_4, KC_NO); break;
    case M_TMUX_5: do_tmux_key(record, KC_5, KC_NO); break;
    case M_TMUX_COPY_MODE: do_tmux_key(record, KC_LBRC, KC_NO); break;
    case M_TMUX_SP: do_tmux_key(record, KC_QUOT, KC_LSFT); break;
    case M_TMUX_VS: do_tmux_key(record, KC_5, KC_LSFT); break;
    case M_TMUX_NEW: do_tmux_key(record, KC_C, KC_NO); break;
  }
  return true;
};
