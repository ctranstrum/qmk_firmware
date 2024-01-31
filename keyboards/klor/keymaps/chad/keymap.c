/*

                                        █         █    █            ▄▄▄▀▀▀▀▀▀▄▄▄        █▀▀▀▀▀▀▀▀▀▀█
                                        █        █     █          ▄▀            ▀▄      █          █
                                        █       █      █        ▄▀                ▀▄    █          █
                                        █      █       █        █                  █    █          █
                                        █     █        █       █                    █   █          █
                                        █    █         █       █                    █   █▄▄▄▄▄▄▄▄▄▄█
                                        █   █ █        █       █                    █   █      █
                                        █  █   █       █        █                  █    █       █
                                        █ █     █      █        ▀▄                ▄▀    █        █
                                        ██       █     █          ▀▄            ▄▀      █         █
                                        █         █    █▄▄▄▄▄▄▄▄    ▀▀▀▄▄▄▄▄▄▀▀▀        █          █

                                        ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
*/
#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>
#include "klor.h"
#ifdef HAPTIC_ENABLE
#include "drivers/haptic/DRV2605L.h"
#endif //HAPTIC ENABLE
#ifdef RGB_MATRIX_LEDMAPS_ENABLED
#include "features/rgb_matrix_ledmaps.h"
#endif // RGB_MATRIX_LEDMAPS_ENABLED


// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ D E F I N I T I O N S                                                                                                                      │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

// ┌───────────────────────────────────────────────────────────┐
// │ d e f i n e   l a y e r s                                 │
// └───────────────────────────────────────────────────────────┘

enum klor_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _COLEMAK,
    _QWERTY,
    _DVORAK,
    _NAV,
    _NUM,
    _FUNC,
    _VIM,
    _SYM,
    _MOUSE,
    _KEYBD,
};

// ┌───────────────────────────────────────────────────────────┐
// │ d e f i n e   k e y c o d e s                             │
// └───────────────────────────────────────────────────────────┘

enum custom_keycodes {
    COLEMAK = SAFE_RANGE,
    QWERTY,
    DVORAK,
    OS_SWAP,
    ARROW_FN,
    JIGGLE,
    VIM_1,
    VIM_2,
    VIM_3,
    VIM_4,
    VIM_5,
    VIM_6,
    VIM_7,
    VIM_8,
    VIM_9,
    VIM_10,
    VIM_PREV,
    VIM_NEXT,
    VIM_SAVE,
    VIM_QUIT,
    VIM_BAIL,
};

// ┌───────────────────────────────────────────────────────────┐
// │ d e f i n e   t a p d a n c e   c o d e s                 │
// └───────────────────────────────────────────────────────────┘

typedef enum {
    TD_NONE,
    TD_HOLD,
    TD_TAP,
    TD_DOUBLE_TAP,
    TD_MORE
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum tap_dance_codes {
    TD_NAV_SHIFT_CAPSWORD,
    TD_KEYBD_ASTERISK_AT,
};

// references for all tap dance functions ... dunno, must be a C thing
td_state_t cur_dance(tap_dance_state_t *state);
void td_nsc_finished(tap_dance_state_t *state, void *user_data);
void td_nsc_reset(tap_dance_state_t *state, void *user_data);
void td_kaa_finished(tap_dance_state_t *state, void *user_data);
void td_kaa_reset(tap_dance_state_t *state, void *user_data);

#define TD_NSC TD(TD_NAV_SHIFT_CAPSWORD)
#define TD_KAA TD(TD_KEYBD_ASTERISK_AT)

// ┌───────────────────────────────────────────────────────────┐
// │ d e f i n e   m a c r o n a m e s                         │
// └───────────────────────────────────────────────────────────┘

// LEFT HAND HOME ROW MODS ├───────────────────────────────────┐

#define MT_SZ MT(MOD_LSFT, KC_Z)
#define MT_CX MT(MOD_LCTL, KC_X)
#define MT_AC MT(MOD_LALT, KC_C)
#define MT_GD MT(MOD_LGUI, KC_D)
#define MT_RV MT(MOD_RALT, KC_V)

// RIGHT HAND HOME ROW MODS ├──────────────────────────────────┐

#define MT_RK MT(MOD_RALT, KC_K)
#define MT_GH MT(MOD_LGUI, KC_H)
#define MT_ACOM MT(MOD_LALT, KC_COMMA)
#define MT_CDOT MT(MOD_LCTL, KC_DOT)
#define MT_SSLH MT(MOD_LSFT, KC_SLASH)

// QWERTY HOME ROW MODS ├──────────────────────────────────────┐

#define MT_GV MT(MOD_LGUI, KC_V)
#define MT_RB MT(MOD_RALT, KC_B)
#define MT_RN MT(MOD_RALT, KC_N)
#define MT_GM MT(MOD_LGUI, KC_M)

// DVORAK HOME ROW MODS ├──────────────────────────────────────┐

#define MT_CQ MT(MOD_LCTL, KC_Q)
#define MT_AJ MT(MOD_LALT, KC_J)
#define MT_GK MT(MOD_LGUI, KC_K)
#define MT_RX MT(MOD_RALT, KC_X)
#define MT_RB MT(MOD_RALT, KC_B)
#define MT_AW MT(MOD_LALT, KC_W)
#define MT_CV MT(MOD_LCTL, KC_V)

// LAYER HOLD TAPS ├───────────────────────────────────────────┐

#define LT_NAV LT(_NAV, KC_LSFT)
#define LT_NUM LT(_NUM, KC_BACKSPACE)
#define LT_FN LT(_FUNC, KC_TAB)
#define LT_SYM LT(_SYM, KC_SPACE)
#define LT_MOUSE LT(_MOUSE, KC_ENTER)
#define LT_VIM LT(_VIM, KC_ESC)
#define LT_KBD LT(_KEYBD, KC_ASTERISK)

// MAC KEYS ├──────────────────────────────────────────────────┐

#define LAUNCHPD HYPR(KC_4)
#define MICMUTE HYPR(KC_M)
#define SCRN_CP LCMD(LCTL(LSFT(KC_4)))
#define SCRN_FN LCMD(LSFT(KC_4))
#define SCRN_LK LCMD(LCTL(KC_Q))

// ┌───────────────────────────────────────────────────────────┐
// │ d e f i n e   c u s t o m   s h i f t   k e y s           │
// └───────────────────────────────────────────────────────────┘

#include "features/custom_shift_keys.h"

const custom_shift_key_t custom_shift_keys[] = {
    {KC_ASTERISK, KC_AT},
    {KC_COMMA, KC_SEMICOLON},
    {KC_DOT, KC_COLON},
    {KC_LPRN, KC_LT},
    {KC_PIPE, KC_AMPERSAND},
    {KC_RPRN, KC_GT},
    {KC_SLASH, KC_BACKSLASH},
    {KC_TILDE, KC_GRAVE},
    {LALT_T(KC_COMMA), KC_SEMICOLON},
    {LCTL_T(KC_DOT), KC_COLON},
    {LSFT_T(KC_SLASH), KC_BACKSLASH},
    {LT_KBD, KC_AT}
};

uint8_t NUM_CUSTOM_SHIFT_KEYS = sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

// ┌───────────────────────────────────────────────────────────┐
// │ d e f i n e   c o m b o s                                 │
// └───────────────────────────────────────────────────────────┘

// Looks like most of these work only in Colemak,
// which honestly is probably just fine with me
const uint16_t PROGMEM altdot_combo[] = {LCTL_T(KC_DOT), KC_I, COMBO_END};
const uint16_t PROGMEM cmdz_combo[] = {KC_A, KC_Q, COMBO_END};
const uint16_t PROGMEM cmdx_combo[] = {KC_R, KC_W, COMBO_END};
const uint16_t PROGMEM cmdc_combo[] = {KC_F, KC_S, COMBO_END};
const uint16_t PROGMEM cmdd_combo[] = {KC_P, KC_T, COMBO_END};
const uint16_t PROGMEM cmdv_combo[] = {KC_B, KC_G, COMBO_END};
const uint16_t PROGMEM ctrlz_combo[] = {KC_A, LSFT_T(KC_Z), COMBO_END};
const uint16_t PROGMEM ctrlx_combo[] = {KC_R, LCTL_T(KC_X), COMBO_END};
const uint16_t PROGMEM ctrlc_combo[] = {LALT_T(KC_C), KC_S, COMBO_END};
const uint16_t PROGMEM ctrld_combo[] = {LCMD_T(KC_D), KC_T, COMBO_END};
const uint16_t PROGMEM ctrlv_combo[] = {KC_G, RALT_T(KC_V), COMBO_END};
const uint16_t PROGMEM ctrlu_combo[] = {KC_E, KC_U, COMBO_END};

combo_t key_combos[] = {
    COMBO(altdot_combo, LALT(KC_DOT)),
    COMBO(cmdz_combo, LCMD(KC_Z)),
    COMBO(cmdx_combo, LCMD(KC_X)),
    COMBO(cmdc_combo, LCMD(KC_C)),
    COMBO(cmdd_combo, LCMD(KC_D)),
    COMBO(cmdv_combo, LCMD(KC_V)),
    COMBO(ctrlz_combo, LCTL(KC_Z)),
    COMBO(ctrlx_combo, LCTL(KC_X)),
    COMBO(ctrlc_combo, LCTL(KC_C)),
    COMBO(ctrld_combo, LCTL(KC_D)),
    COMBO(ctrlv_combo, LCTL(KC_V)),
    COMBO(ctrlu_combo, LCTL(KC_U)),
};

// ┌───────────────────────────────────────────────────────────┐
// │ d e f i n e   s o u n d s                                 │
// └───────────────────────────────────────────────────────────┘

#ifdef AUDIO_ENABLE
  #define WINXP_SOUND W__NOTE(_DS6), Q__NOTE(_DS5), H__NOTE(_AS5), H__NOTE(_GS5), H__NOTE(_DS5), H__NOTE(_DS6), H__NOTE(_AS5)
  #define MAC_SOUND S__NOTE(_CS5), B__NOTE(_C5)

  float winxp_song[][2] = SONG(WINXP_SOUND);
  float mac_song[][2] = SONG(MAC_SOUND);
#endif // AUDIO_ENABLE

// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ K E Y M A P S                                                                                                                              │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
   ┌───────────────────────────────────────────────────────────┐
   │ c o l e m a k   d h                                       │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │    Q    │    W    │    F    │    P    │    B    │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │    J    │    L    │    U    │    Y    │   ' "   │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │   [ {   │    A    │    R    │    S    │    T    │    G    ├─╯                ╰─┤    M    │    N    │    E    │    I    │    O    │   ] }   │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │   ( <   │    Z    │    X    │    C    │    D    │    V    ││  ▶ ||  ││  MUTE  ││    K    │    H    │   , :   │   . :   │   / \   │   ) >   │
   │         │  shift  │  ctrl   │   alt   │   cmd   │  r alt  ││        ││        ││  r alt  │   cmd   │   alt   │  ctrl   │  shift  │         │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┤╰────────╯╰────────╯├─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │   ESC   │ BACKSPC │  SHIFT  │   TAB   ││    -    │  SPACE  │  ENTER  │    *    │
                                 │   vim   │   num   │   nav   │    fn   ││         │   sym   │  mouse  │  keybd  │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_COLEMAK] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,                          KC_J,     KC_L,     KC_U,     KC_Y,     KC_QUOT,
    KC_LBRC,  KC_A,     KC_R,     KC_S,     KC_T,     KC_G,                          KC_M,     KC_N,     KC_E,     KC_I,     KC_O,     KC_RBRC,
    KC_LPRN,  MT_SZ,    MT_CX,    MT_AC,    MT_GD,    MT_RV,    KC_MPLY,   KC_MUTE,  MT_RK,    MT_GH,    MT_ACOM,  MT_CDOT,  MT_SSLH,  KC_RPRN,
                                  LT_VIM,   LT_NUM,   TD_NSC,   LT_FN,     KC_MINUS, LT_SYM,   LT_MOUSE, TD_KAA
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ q w e r t y                                               │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │    Q    │    W    │    E    │    R    │    T    │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │    Y    │    U    │    I    │    O    │    P    │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │   [ {   │    A    │    S    │    D    │    F    │    G    ├─╯                ╰─┤    H    │    J    │    K    │    L    │   ' "   │   ] }   │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │   ( <   │    Z    │    X    │    C    │    V    │    B    ││  ▶ ||  ││  MUTE  ││    N    │    M    │   , ;   │   . :   │   / \   │   ) >   │
   │         │  shift  │  ctrl   │   alt   │   cmd   │  r alt  ││        ││        ││  r alt  │   cmd   │   alt   │  ctrl   │  shift  │         │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │   ESC   │ BACKSPC │  SHIFT  │   TAB   ││    -    │  SPACE  │  ENTER  │    *    │
                                 │   vim   │   num   │   nav   │    fn   ││         │   sym   │  mouse  │  keybd  │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_QWERTY] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              _______,  _______,  KC_E,     KC_R,     KC_T,                          KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,
    _______,  _______,  KC_S,     KC_D,     KC_F,     _______,                       KC_H,     KC_J,     KC_K,     KC_L,     KC_QUOT,  _______,
    _______,  _______,  _______,  _______,  MT_GV,    MT_RB,    _______,   _______,  MT_RN,    MT_GM,    _______,  _______,  _______,  _______,
                                  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ d v o r a k                                               │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │   ' "   │   , ;   │   . :   │    P    │    Y    │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │    F    │    G    │    C    │    R    │    L    │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │   [ {   │    A    │    O    │    E    │    U    │    I    ├─╯                ╰─┤    D    │    H    │    T    │    N    │    S    │   ] }   │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │   ( <   │   / \   │    Q    │    J    │    K    │    X    ││  ▶ ||  ││  MUTE  ││    B    │    M    │    W    │    V    │    Z    │   ) >   │
   │         │  shift  │  ctrl   │   alt   │   cmd   │  r alt  ││        ││        ││  r alt  │   cmd   │   alt   │  ctrl   │  shift  │         │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │   ESC   │ BACKSPC │  SHIFT  │   TAB   ││    -    │  SPACE  │  ENTER  │    *    │
                                 │   vim   │   num   │   nav   │    fn   ││         │   sym   │  mouse  │  keybd  │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_DVORAK] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              KC_QUOT,  KC_COMM,  KC_DOT,   KC_P,     KC_Y,                          KC_F,     KC_G,     KC_C,     KC_R,     KC_L,
    _______,  _______,  KC_O,     KC_E,     KC_U,     KC_I,                          KC_D,     KC_H,     KC_T,     KC_N,     KC_S,     _______,
    _______,  MT_SSLH,  MT_CQ,    MT_AJ,    MT_GK,    MT_RX,    _______,   _______,  MT_RB,    MT_GM,    MT_AW,    MT_CV,    MT_SZ,    _______,
                                  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ n a v i g a t i o n                                       │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │         │         │         │         │         │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │  PG UP  │         │         │         │         │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │         │         │         │         │         │         ├─╯                ╰─┤  HOME   │    ←    │    ↑    │    →    │   END   │ BACKSPC │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │         │  SHIFT  │  CTRL   │   ALT   │   CMD   │  R ALT  ││        ││        ││  PG DN  │         │    ↓    │         │         │   DEL   │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │         │         │   ###   │         ││         │  SPACE  │         │         │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_NAV] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       KC_PGUP,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       KC_HOME,  KC_LEFT,  KC_UP,    KC_RIGHT, KC_END,   KC_BSPC,
    XXXXXXX,  KC_LSFT,  KC_LCTL,  KC_LALT,  KC_LCMD,  KC_RALT,  XXXXXXX,   XXXXXXX,  KC_PGDN,  XXXXXXX,  KC_DOWN,  XXXXXXX,  XXXXXXX,  KC_DEL,
                                  XXXXXXX,  XXXXXXX,  _______,  XXXXXXX,   XXXXXXX,  KC_SPC,   XXXXXXX,  XXXXXXX
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ n u m b e r                                               │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │         │         │         │         │         │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │    /    │    7    │    8    │    9    │    -    │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │         │         │         │         │         │         ├─╯                ╰─┤    *    │    4    │    5    │    6    │    +    │ BACKSPC │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │         │  SHIFT  │  CTRL   │   ALT   │   CMD   │  R ALT  ││        ││        ││    :    │    1    │    2    │    3    │    =    │    ;    │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │         │   ###   │         │         ││    ,    │  SPACE  │    0    │    .    │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_NUM] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       KC_SLSH,  KC_7,     KC_8,     KC_9,     KC_MINUS,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       KC_ASTR,  KC_4,     KC_5,     KC_6,     KC_PLUS,  KC_BSPC,
    XXXXXXX,  KC_LSFT,  KC_LCTL,  KC_LALT,  KC_LCMD,  KC_RALT,  XXXXXXX,   XXXXXXX,  KC_COLON, KC_1,     KC_2,     KC_3,     KC_EQUAL, KC_SCLN,
                                  XXXXXXX,  _______,  XXXXXXX,  XXXXXXX,   KC_COMMA, KC_SPACE, KC_0,     KC_DOT
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ f u n c t i o n                                           │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │         │         │         │         │         │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │         │   F7    │   F8    │   F9    │         │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │         │         │         │         │         │         ├─╯                ╰─┤         │   F4    │   F5    │   F6    │         │   F12   │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │         │  SHIFT  │  CTRL   │   ALT   │   CMD   │  R ALT  ││        ││        ││         │   F1    │   F2    │   F3    │         │   F11   │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │         │         │         │   ###   ││         │         │   F10   │         │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_FUNC] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       XXXXXXX,  KC_F7,    KC_F8,    KC_F9,    XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       XXXXXXX,  KC_F4,    KC_F5,    KC_F6,    XXXXXXX,  KC_F12,
    XXXXXXX,  KC_LSFT,  KC_LCTL,  KC_LALT,  KC_LCMD,  KC_RALT,  XXXXXXX,   XXXXXXX,  XXXXXXX,  KC_F1,    KC_F2,    KC_F3,    XXXXXXX,  KC_F11,
                                  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,   XXXXXXX,  XXXXXXX,  KC_F10,   XXXXXXX
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ v i m                                                     │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │         │         │         │         │         │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │         │  tab 7  │  tab 8  │  tab 9  │         │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │         │         │         │         │         │         ├─╯                ╰─┤         │  tab 4  │  tab 5  │  tab 6  │         │   bail  │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │         │         │         │         │         │         ││        ││        ││         │  tab 1  │  tab 2  │  tab 3  │         │   exit  │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │   ###   │         │         │         ││   save  │ prevtab │  tab 10 │ nexttab │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_VIM] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       XXXXXXX,  VIM_7,    VIM_8,    VIM_9,    XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       XXXXXXX,  VIM_4,    VIM_5,    VIM_6,    XXXXXXX,  VIM_BAIL,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX,  XXXXXXX,  VIM_1,    VIM_2,    VIM_3,    XXXXXXX,  VIM_QUIT,
                                  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,   VIM_SAVE, VIM_PREV, VIM_10,   VIM_NEXT
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ s y m b o l                                               │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │         │         │    +    │         │         │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │         │         │         │         │         │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │    `    │    ^    │    @    │    !    │    =    │    $    ├─╯                ╰─┤         │         │         │         │         │         │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │    ~    │    #    │    %    │    -    │    _    │    ?    ││        ││        ││  R ALT  │   CMD   │   ALT   │   CTRL  │         │         │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │    =>   │    |    │    &    │    _    ││         │   ###   │         │         │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_SYM] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              XXXXXXX,  XXXXXXX,  KC_PLUS,  XXXXXXX,  XXXXXXX,                       XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    KC_GRAVE, KC_CIRC,  KC_AT,    KC_EXLM,  KC_EQUAL, KC_DLR,                        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    KC_TILDE, KC_HASH,  KC_PERC,  KC_MINUS, KC_UNDS,  KC_QUES,  XXXXXXX,   XXXXXXX,  KC_RALT,  KC_LCMD,  KC_LALT,  KC_LCTL,  XXXXXXX,  XXXXXXX,
                                  ARROW_FN, KC_PIPE,  KC_AMPR,  KC_UNDS,   XXXXXXX,  _______,  XXXXXXX,  XXXXXXX
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ m o u s e   k e y s                                       │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │         │         │         │         │         │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │         │         │         │         │         │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │         │ r click │   left  │    up   │  right  │ l click ├─╯                ╰─┤ l click │ accel 1 │ accel 2 │ accel 3 │ r click │         │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │         │         │         │   down  │         │         ││        ││        ││         │   CMD   │   ALT   │   CTRL  │  SHIFT  │         │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │         │         │         │         ││         │         │   ###   │         │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_MOUSE] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                       XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  KC_BTN2,  KC_MS_L,  KC_MS_U,  KC_MS_R,  KC_BTN1,                       KC_BTN1,  KC_ACL0,  KC_ACL1,  KC_ACL2,  KC_BTN2,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MS_D,  XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX,  XXXXXXX,  KC_LCMD,  KC_LALT,  KC_LCTL,  KC_LSFT,  XXXXXXX,
                                  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX,  XXXXXXX,  _______,  XXXXXXX
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ k e y b o a r d                                           │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │  qwerty │         │ scrfile │  launch │ rgb tog │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │         │         │         │         │         │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │  brt +  │         │         │ scrcopy │         │  rgb +  ├─╯                ╰─┤         │         │         │         │         │         │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │  brt -  │  jiggle │ micmute │ colemak │  dvorak │  rgb -  ││        ││        ││         │         │         │         │         │         │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │   lock  │         │ caps lk │  flash  ││         │         │         │   ###   │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘ */

   [_KEYBD] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              QWERTY,   XXXXXXX,  SCRN_FN,  LAUNCHPD, RGB_TOG,                       XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    KC_BRMU,  XXXXXXX,  XXXXXXX,  SCRN_CP,  XXXXXXX,  RGB_VAI,                       XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    KC_BRMD,  JIGGLE,   MICMUTE,  COLEMAK,  DVORAK,   RGB_VAD,  XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
                                  SCRN_LK,  XXXXXXX,  KC_CAPS,  QK_BOOT,   XXXXXXX,  XXXXXXX,  XXXXXXX,  _______
 ),

 /*
   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸

   ┌───────────────────────────────────────────────────────────┐
   │ t e m p l a t e   p o l y d a c t y l                     │
   └───────────────────────────────────────────────────────────┘
             ┌─────────┬─────────┬─────────┬─────────┬─────────┐                    ┌─────────┬─────────┬─────────┬─────────┬─────────┐
             │         │         │         │         │         │ ╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮╭╮ │         │         │         │         │         │
   ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤ │╰╯╰╯╰╯╰╯╰╯╰╯╰╯╰╯│ ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐
   │         │         │         │         │         │         ├─╯                ╰─┤         │         │         │         │         │         │
   ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤╭────────╮╭────────╮├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
   │         │         │         │         │         │         ││        ││        ││         │         │         │         │         │         │
   └─────────┴─────────┴─────────┼─────────┼─────────┼─────────┼╰────────╯╰────────╯┼─────────┼─────────┼─────────┼─────────┴─────────┴─────────┘
                                 │         │         │         │         ││         │         │         │         │
                                 └─────────┴─────────┴─────────┴─────────┘└─────────┴─────────┴─────────┴─────────┘

   [_TEMPLATE] = LAYOUT_polydactyl(
 //╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷╷         ╷         ╷         ╷         ╷         ╷         ╷         ╷
              _______,  _______,  _______,  _______,  _______,                       _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,                       _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,  _______,
                                  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______
 )

   ╺━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╸
 */
};


// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ L E D M A P S                                                                                                                              │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

const ledmap PROGMEM ledmaps[] = {
    [_COLEMAK] = RGB_MATRIX_LAYOUT_LEDMAP(
                    WHT L3, WHT L3, WHT L3, WHT L3, WHT L3,                 WHT L3, WHT L3, WHT L3, WHT L3, WHT L1,
            VLT L1, WHT L3, WHT L3, WHT L3, PNK L3, WHT L3,                 WHT L3, PNK L3, WHT L3, WHT L3, WHT L3, VLT L1,
            BLU L1, WHT L3, WHT L3, WHT L3, WHT L3, WHT L3,                 WHT L3, WHT L3, WHT L1, WHT L1, WHT L1, BLU L1,
                                    VLT L1, GRN L1, SKY L1, YLW L2, WHT L2, SKY L1, ORG L3, YLW L3
            ),
    [_QWERTY] = RGB_MATRIX_LAYOUT_LEDMAP(
                    WHT L3, WHT L3, WHT L3, WHT L3, WHT L3,                 WHT L3, WHT L3, WHT L3, WHT L3, WHT L1,
            VLT L1, WHT L3, WHT L3, WHT L3, PNK L3, WHT L3,                 WHT L3, PNK L3, WHT L3, WHT L3, WHT L3, VLT L1,
            BLU L1, WHT L3, WHT L3, WHT L3, WHT L3, WHT L3,                 WHT L3, WHT L3, WHT L1, WHT L1, WHT L1, BLU L1,
                                    VLT L1, GRN L1, SKY L1, YLW L2, WHT L2, SKY L1, ORG L3, YLW L3
            ),
    [_DVORAK] = RGB_MATRIX_LAYOUT_LEDMAP(
                    WHT L1, WHT L1, WHT L1, WHT L3, WHT L3,                 WHT L3, WHT L3, WHT L3, WHT L3, WHT L3,
            VLT L1, WHT L3, WHT L3, WHT L3, PNK L3, WHT L3,                 WHT L3, PNK L3, WHT L3, WHT L3, WHT L3, VLT L1,
            BLU L1, WHT L1, WHT L3, WHT L3, WHT L3, WHT L3,                 WHT L3, WHT L3, WHT L3, WHT L3, WHT L3, BLU L1,
                                    VLT L1, GRN L1, SKY L1, YLW L2, WHT L2, SKY L1, ORG L3, YLW L3
            ),
    [_NAV] = RGB_MATRIX_LAYOUT_LEDMAP(
                    ______, ______, ______, ______, ______,                 GRN L3, ______, ______, ______, ______,
            ______, ______, ______, ______, ______, ______,                 BLU L1, SKY L3, SKY L3, SKY L3, BLU L3, RED L2,
            ______, WHT L1, WHT L1, WHT L1, WHT L1, WHT L1,                 GRN L1, ______, SKY L3, ______, ______, PNK L3,
                                    ______, ______, SKY L4, ______, ______, WHT L3, ______, ______
            ),
    [_NUM] = RGB_MATRIX_LAYOUT_LEDMAP(
                    ______, ______, ______, ______, ______,                 YLW L2, GRN L3, GRN L3, GRN L3, SKY L1,
            ______, ______, ______, ______, ______, ______,                 YLW L3, GRN L2, GRN L2, GRN L2, SKY L2, RED L2,
            ______, WHT L1, WHT L1, WHT L1, WHT L1, WHT L1,                 WHT L3, GRN L1, GRN L1, GRN L1, YLW L2, WHT L2,
                                    ______, GRN L4, ______, ______, WHT L1, WHT L3, GRN L1, WHT L2
            ),
    [_FUNC] = RGB_MATRIX_LAYOUT_LEDMAP(
                    ______, ______, ______, ______, ______,                 ______, ORG L3, ORG L3, ORG L3, ______,
            ______, ______, ______, ______, ______, ______,                 ______, ORG L2, ORG L2, ORG L2, ______, ORG L2,
            ______, WHT L1, WHT L1, WHT L1, WHT L1, WHT L1,                 ______, ORG L1, ORG L1, ORG L1, ______, ORG L1,
                                    ______, ______, ______, ORG L4, ______, ______, ORG L1, ______
            ),
    [_VIM] = RGB_MATRIX_LAYOUT_LEDMAP(
                    ______, ______, ______, ______, ______,                 ______, PUR L3, PUR L3, PUR L3, ______,
            ______, ______, ______, ______, ______, ______,                 ______, PUR L2, PUR L2, PUR L2, ______, VLT L3,
            ______, ______, ______, ______, ______, ______,                 ______, PUR L1, PUR L1, PUR L1, ______, RED L1,
                                    PUR L4, ______, ______, ______, GRN L1, BLU L2, PUR L1, BLU L3
            ),
    [_SYM] = RGB_MATRIX_LAYOUT_LEDMAP(
                    ______, ______, LIM L2, ______, ______,                 ______, ______, ______, ______, ______,
            WHT L1, ORG L3, SKY L3, WHT L2, YLW L3, GRN L3,                 ______, ______, ______, ______, ______, ______,
            WHT L1, BLU L2, YLW L3, LIM L1, WHT L1, YLW L1,                 WHT L1, WHT L1, WHT L1, WHT L1, ______, ______,
                                    PUR L3, WHT L3, BLU L3, WHT L1, ______, SKY L4, ______, ______
            ),
    [_MOUSE] = RGB_MATRIX_LAYOUT_LEDMAP(
                    ______, ______, ______, ______, ______,                 ______, ______, ______, ______, ______,
            ______, GRN L1, SKY L3, SKY L3, SKY L3, GRN L3,                 GRN L3, PNK L1, PNK L2, PNK L3, GRN L1, ______,
            ______, ______, ______, SKY L3, ______, ______,                 ______, WHT L1, WHT L1, WHT L1, WHT L1, ______,
                                    ______, ______, ______, ______, ______, ______, PNK L4, ______
            ),
    // Note: to test all the colors you can comment out the keyboard map and use the all-colors map instead
    [_KEYBD] = RGB_MATRIX_LAYOUT_LEDMAP(
                    PNK L3, ______, BLU L3, ORG L3, VLT L1,                 YLW L3, YLW L1, LIM L1, LIM L3, ______,
            YLW L3, ______, ______, GRN L3, ______, WHT L3,                 ORG L3, ORG L1, GRN L1, GRN L3, SKY L3, SKY L1,
            YLW L1, ORG L1, PUR L3, PNK L3, PNK L3, WHT L1,                 RED L3, RED L1, BLU L1, BLU L3, PUR L3, PUR L1,
                                    RED L3, ______, RED L1, VLT L5, PNK L1, PNK L3, VLT L1, VLT L3
            /*         RED L1, ORG L1, YLW L1, LIM L1, GRN L1,                 SKY L1, BLU L1, PUR L1, VLT L1, PNK L1, */
            /* ______, RED L3, ORG L3, YLW L3, LIM L3, GRN L3,                 SKY L3, BLU L3, PUR L3, VLT L3, PNK L3, ______, */
            /* WHT L1, RED L5, ORG L5, YLW L5, LIM L5, GRN L5,                 SKY L5, BLU L5, PUR L5, VLT L5, PNK L5, WHT L1, */
            /*                         WHT L1, WHT L2, WHT L4, WHT L5, VLT L5, WHT L5, WHT L3, WHT L1 */
            ),
};


// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ L I A T R I S   L E D                                                                                                                      │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

void keyboard_pre_init_user(void) {
    // Turn off the Liatris LED https://docs.splitkb.com/hc/en-us/articles/5799711553820-Power-LED
    setPinOutput(24);
    writePinHigh(24);
}


// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ H A P T I C   F E E D B A C K                                                                                                              │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

void keyboard_post_init_user(void) {
  // Call the post init code.
  #if HAPTIC_ENABLE
    haptic_disable(); // disables per key haptic feedback by default
  #endif //HAPTIC ENABLE
}


// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ O L E D                                                                                                                                    │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

#ifdef OLED_ENABLE

// ┌───────────────────────────────────────────────────────────┐
// │ d y n a m i c   m a c r o                                 │
// └───────────────────────────────────────────────────────────┘

char layer_state_str[24];
char o_text[24] = "";
int dmacro_num = 0;

#ifdef DYNAMIC_MACRO_ENABLE
    char dmacro_text[4][24] = { "", "RECORDING", "STOP RECORDING",  "PLAY RECORDING"};
    static uint16_t dmacro_timer;
    const char PROGMEM rec_ico[] = {0xD1, 0xE1, 0};
    const char PROGMEM stop_ico[] = {0xD3, 0xE1, 0};
    const char PROGMEM play_ico[] = {0xD2, 0xE1, 0};


    // DYNMACRO RECORD ├─────────────────────────────────────────────────────────────┐
    void dynamic_macro_record_start_user(void) {
          dmacro_num = 1;
        return;
    }

    // DYNMACRO STOP RECORDING ├─────────────────────────────────────────────────────┐
    void dynamic_macro_record_end_user(int8_t direction) {
          dmacro_num = 2;
          dmacro_timer = timer_read();
        return;
    }

    // DYNMACRO PLAY RECORDING ├─────────────────────────────────────────────────────┐
    void dynamic_macro_play_user(int8_t direction) {
          dmacro_num = 3;
          dmacro_timer = timer_read();
        return;
    }
#endif //DYNAMIC_MACRO_ENABLE


void matrix_scan_user(void) {
  #ifdef DYNAMIC_MACRO_ENABLE
    // DynMacroTimer
    if(dmacro_num > 0){
        if (timer_elapsed(dmacro_timer) < 3000) {
            strcpy ( o_text, dmacro_text[dmacro_num] );
          }
        else {
            if (dmacro_num == 1) {
                strcpy ( o_text, dmacro_text[1] );
              }
            else {
                strcpy ( o_text, layer_state_str );
                dmacro_num = 0;
              }
          }
      }
   #endif //DYNAMIC_MACRO_ENABLE
}


// ┌───────────────────────────────────────────────────────────┐
// │ o l e d   g r a p h i c s                                 │
// └───────────────────────────────────────────────────────────┘

void render_os_lock_status(void) {
    static const char PROGMEM sep_v[] = {0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0};
    static const char PROGMEM sep_h1[] = {0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0};
    static const char PROGMEM sep_h2[] = {0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0};
    static const char PROGMEM face_1[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0xE1, 0};
    static const char PROGMEM face_2[] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xE1, 0};
    static const char PROGMEM os_m_1[] = {0x95, 0x96, 0};
    static const char PROGMEM os_m_2[] = {0xB5, 0xB6, 0};
    static const char PROGMEM os_w_1[] = {0x97, 0x98, 0};
    static const char PROGMEM os_w_2[] = {0xB7, 0xB8, 0};
    static const char PROGMEM s_lock[] = {0x8F, 0x90, 0};
    static const char PROGMEM n_lock[] = {0x91, 0x92, 0};
    static const char PROGMEM c_lock[] = {0x93, 0x94, 0};
    static const char PROGMEM b_lock[] = {0xE1, 0xE1, 0};
    #ifdef AUDIO_ENABLE
      static const char PROGMEM aud_en[] = {0xAF, 0xB0, 0};
      static const char PROGMEM aud_di[] = {0xCF, 0xD0, 0};
    #endif
    #ifdef HAPTIC_ENABLE
      static const char PROGMEM hap_en[] = {0xB1, 0xB2, 0};
    #endif

// os mode status ────────────────────────────────────────┐

    oled_write_ln_P(sep_v, false);

    if (keymap_config.swap_lctl_lgui) {
        oled_write_P(os_w_1, false); // ──── WIN
    } else {
        oled_write_P(os_m_1, false); // ──── MAC
    }

    oled_write_P(sep_h1, false);
    oled_write_P(face_1, false);

    if (keymap_config.swap_lctl_lgui) {
        oled_write_P(os_w_2, false); // ──── WIN
    } else {
        oled_write_P(os_m_2, false); // ──── MAC
    }

    oled_write_P(sep_h1, false);
    oled_write_P(face_2, false);
    oled_write_ln_P(sep_v, false);


// lock key layer status ─────────────────────────────────┐

    led_t led_usb_state = host_keyboard_led_state();

    if (led_usb_state.num_lock) {
        oled_write_P(n_lock, false); // ──── NUMLOCK
    } else {
        oled_write_P(b_lock, false);
    }
    if (led_usb_state.caps_lock) {
        oled_write_P(c_lock, false); // ─── CAPSLOCK
    } else {
        oled_write_P(b_lock, false);
    }
    if (led_usb_state.scroll_lock) { // ─ SCROLLLOCK
        oled_write_P(s_lock, false);
    } else {
        oled_write_P(b_lock, false);
    }

// hardware feature status ──────────────────────────────┐

    oled_write_P(sep_h2, false);

    #ifndef AUDIO_ENABLE
        oled_write_P(b_lock, false);
    #endif
    #ifndef HAPTIC_ENABLE
        oled_write_P(b_lock, false);
    #endif

    #ifdef AUDIO_ENABLE // ────────────────── AUDIO
        if (is_audio_on()) {
          oled_write_P(aud_en, false);
        } else {
          oled_write_P(aud_di, false);
        }
    #endif // AUDIO ENABLE

     #ifdef HAPTIC_ENABLE // ─────────────── HAPTIC
        oled_write_P(hap_en, false);
     #endif // HAPTIC ENABLE
}


// layer status ──────────────────────────────────────────┐

int layerstate = 0;

layer_state_t layer_state_set_user(layer_state_t state) {
      switch (get_highest_layer(state)) {
            case _COLEMAK:
                strcpy ( layer_state_str, "COLEMAK");
                break;
            case _QWERTY:
                strcpy ( layer_state_str, "QWERTY");
                break;
            case _DVORAK:
                strcpy ( layer_state_str, "DVORAK");
                break;
            case _NAV:
                clear_oneshot_mods();
                strcpy ( layer_state_str, "NAVIGATION");
                break;
            case _NUM:
                clear_oneshot_mods();
                strcpy ( layer_state_str, "NUMBER");
                break;
            case _FUNC:
                clear_oneshot_mods();
                strcpy ( layer_state_str, "FUNCTION");
                break;
            case _VIM:
                strcpy ( layer_state_str, "VIM");
                break;
            case _SYM:
                clear_oneshot_mods();
                strcpy ( layer_state_str, "SYMBOL");
                break;
            case _MOUSE:
                strcpy ( layer_state_str, "MOUSE");
                break;
            case _KEYBD:
                strcpy ( layer_state_str, "KEYBOARD");
                break;
            default:
                strcpy ( layer_state_str, "XXXXXX");
        }
      if (dmacro_num < 1) {
          strcpy ( o_text, layer_state_str );
    }
    return state;
    // return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}


// ┌───────────────────────────────────────────────────────────┐
// │ w r i t e   t o   o l e d                                 │
// └───────────────────────────────────────────────────────────┘

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_master()) {  // ────────────────────────── PRIMARY SIDE

        // layer status ──────────────────────────────────────────────────┐
        #ifdef DYNAMIC_MACRO_ENABLE
            if(dmacro_num == 1){ oled_write_P(rec_ico, false); }
            if(dmacro_num == 2){ oled_write_P(stop_ico, false); }
            if(dmacro_num == 3){ oled_write_P(play_ico, false); }
        #endif //DYNAMIC_MACRO_ENABLE

        oled_write_ln(o_text, false);
        render_os_lock_status();

    } else {  // ─────────────────────────────────────────── SECONDARY SIDE

        // Chad sloppy ───────────────────────────────────────────────────┐

        static const char PROGMEM chad[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x60, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x30, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0C, 0x10, 0x60, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xBF, 0x40, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x09, 0x0A, 0x04, 0x18,
    0x68, 0x84, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
    0x04, 0x08, 0x10, 0x20, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x81, 0x42,
    0x5C, 0x20, 0xC0, 0x20, 0x20, 0x10, 0x10, 0x09, 0x0A, 0x04, 0x1C, 0x62, 0x82, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x60, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0E, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x10,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x23,
    0x24, 0x18, 0x10, 0x10, 0x08, 0x08, 0x08, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,
    0x08, 0x18, 0x00, 0x00, 0x04, 0x0C, 0x00, 0x00, 0x00, 0xC0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x02, 0x04, 0x04, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

        oled_write_raw_P(chad, sizeof(chad));
    }
    return false;
}
#endif // OLED_ENABLE


/*
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SHT_T:
            return TAPPING_TERM - 150;
        case SHT_N:
            return TAPPING_TERM - 150;
        default:
            return TAPPING_TERM;
    }
}
*/


// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ M A C R O S                                                                                                                                │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // first handle all custom shift keys
    if (!process_custom_shift_keys(keycode, record)) return false;

    static deferred_token jiggler = INVALID_DEFERRED_TOKEN;
    static report_mouse_t mouse = {0};

    uint32_t jiggler_callback(uint32_t trigger_time, void* cb_arg) {
        static const int8_t deltas[32] = {
            0, -1, -2, -2, -3, -3, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0,
            0,  1,  2,  2,  3,  3,  4,  4,  4,  4,  3,  3,  2,  2,  1, 0
        };
        static uint8_t phase = 0;
        mouse.x = deltas[phase];
        mouse.y = deltas[(phase + 8) & 31];
        phase = (phase + 1) & 31;
        host_mouse_send(&mouse);
        return rand() & 16383;
    }

    // Only handle keydown events
    if (!record->event.pressed) return true;

    if (jiggler) {
        // If jiggler is running, stop when any key is pressed
        cancel_deferred_exec(jiggler);
        jiggler = INVALID_DEFERRED_TOKEN;
        mouse = (report_mouse_t){};
        host_mouse_send(&mouse);
    }

    switch (keycode) {

        case OS_SWAP:
            if (!keymap_config.swap_lctl_lgui) {
                keymap_config.swap_lctl_lgui = true;  // ─── MAC
                #ifdef AUDIO_ENABLE
                    PLAY_SONG(mac_song);
                #endif // AUDIO_ENABLE
            }
            else {
                keymap_config.swap_lctl_lgui = false; // ─── WIN
                #ifdef AUDIO_ENABLE
                    PLAY_SONG(winxp_song);
                #endif // AUDIO_ENABLE
            }
            #ifdef HAPTIC_ENABLE
                DRV_pulse(pulsing_strong);
            #endif // HAPTIC_ENABLE
            eeconfig_update_keymap(keymap_config.raw);
            clear_keyboard();  // ──── clear to prevent stuck keys
            return false;


// ┌───────────────────────────────────────────────────────────┐
// │ l a y e r                                                 │
// └───────────────────────────────────────────────────────────┘

        case COLEMAK:
            layer_on(_COLEMAK);
            layer_off(_QWERTY);
            layer_off(_DVORAK);
            #ifdef HAPTIC_ENABLE
              DRV_pulse(transition_hum);
            #endif // HAPTIC_ENABLE
            return false;
        case QWERTY:
            layer_on(_COLEMAK);
            layer_on(_QWERTY);
            layer_off(_DVORAK);
            #ifdef HAPTIC_ENABLE
              DRV_pulse(transition_hum);
            #endif // HAPTIC_ENABLE
            return false;
        case DVORAK:
            layer_on(_COLEMAK);
            layer_off(_QWERTY);
            layer_on(_DVORAK);
            #ifdef HAPTIC_ENABLE
              DRV_pulse(transition_hum);
            #endif // HAPTIC_ENABLE
            return false;

// ┌───────────────────────────────────────────────────────────┐
// │ v i m   m a c r o s                                       │
// └───────────────────────────────────────────────────────────┘

        case ARROW_FN:
            SEND_STRING("=> ");
            return false;

        case VIM_1:
            tap_code(KC_ESC);
            SEND_STRING("1gt");
            return false;

        case VIM_2:
            tap_code(KC_ESC);
            SEND_STRING("2gt");
            return false;

        case VIM_3:
            tap_code(KC_ESC);
            SEND_STRING("3gt");
            return false;

        case VIM_4:
            tap_code(KC_ESC);
            SEND_STRING("4gt");
            return false;

        case VIM_5:
            tap_code(KC_ESC);
            SEND_STRING("5gt");
            return false;

        case VIM_6:
            tap_code(KC_ESC);
            SEND_STRING("6gt");
            return false;

        case VIM_7:
            tap_code(KC_ESC);
            SEND_STRING("7gt");
            return false;

        case VIM_8:
            tap_code(KC_ESC);
            SEND_STRING("8gt");
            return false;

        case VIM_9:
            tap_code(KC_ESC);
            SEND_STRING("9gt");
            return false;

        case VIM_10:
            tap_code(KC_ESC);
            SEND_STRING("10gt");
            return false;

        case VIM_PREV:
            tap_code(KC_ESC);
            SEND_STRING("gT");
            return false;

        case VIM_NEXT:
            tap_code(KC_ESC);
            SEND_STRING("gt");
            return false;

        case VIM_SAVE:
            tap_code(KC_ESC);
            SEND_STRING(" w");
            return false;

        case VIM_QUIT:
            tap_code(KC_ESC);
            SEND_STRING(" q");
            return false;

        case VIM_BAIL:
            tap_code(KC_ESC);
            SEND_STRING(":q!");
            tap_code(KC_ENTER);
            return false;

// ┌───────────────────────────────────────────────────────────┐
// │ p r o d u c t i v i t y                                   │
// └───────────────────────────────────────────────────────────┘

        case JIGGLE:
            jiggler = defer_exec(1, jiggler_callback, NULL);
            return false;

      case KC_MPLY:
          #ifdef HAPTIC_ENABLE
              DRV_pulse(sharp_click);
          #endif // HAPTIC_ENABL
          break;

#ifdef RGB_MATRIX_ENABLE
        case RGB_TOG:
            switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER);
                    rgb_matrix_set_color_all(0, 0, 0);
                } break;
                case LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER: {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                } break;
                case LED_FLAG_UNDERGLOW: {
                    // This line is for LED idle timer. It disables the toggle so you can turn
                    // off LED completely if you like
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable();
                } break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable();
                } break;
            }
            return false;
#endif  // RGB_MATRIX_ENABLE

    }

    return true;
}


// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ T A P   D A N C E S                                                                                                                        │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

// A very simple tap dance function for my very simple needs
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->pressed) return TD_HOLD;
    if (state->count == 1) return TD_TAP;
    if (state->count == 2) return TD_DOUBLE_TAP;
    return TD_MORE;
}

// NAV shift capsword
static td_tap_t td_nsc_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static uint32_t nsc_timer = 0;

// Caps word deactivates on key down, while a tap dance takes TAPPING TERM to resolve,
// so we don't want to activate shift if we pressed it while caps word was active
// and it deactivated right before we got here ... so we set a timer
void caps_word_set_user(bool active) {
    if (!active) {
        nsc_timer = timer_read32();
    }
}

void td_nsc_finished(tap_dance_state_t *state, void *user_data) {
    td_nsc_state.state = cur_dance(state);
    const uint8_t mods = get_mods() | get_oneshot_mods() | get_weak_mods();
    switch (td_nsc_state.state) {
        case TD_HOLD:
            clear_oneshot_mods();
            layer_on(_NAV);
            break;
        case TD_TAP:
            if (mods & MOD_BIT(KC_LSFT)) {
                caps_word_on();
            } else {
                if (timer_elapsed32(nsc_timer) > TAPPING_TERM + 25) {
                    set_oneshot_mods(MOD_BIT(KC_LSFT));
                }
            };
            break;
        case TD_DOUBLE_TAP: caps_word_on(); break;
        default: break;
    }
}

void td_nsc_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_nsc_state.state) {
        case TD_HOLD: layer_off(_NAV); break;
        default: break;
    }
}

// KEYBD asterisk at
static td_tap_t td_kaa_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void td_kaa_finished(tap_dance_state_t *state, void *user_data) {
    td_kaa_state.state = cur_dance(state);
    const uint8_t mods = get_mods() | get_oneshot_mods() | get_weak_mods();
    switch (td_kaa_state.state) {
        case TD_HOLD: layer_on(_KEYBD); break;
        case TD_TAP: SEND_STRING(mods & MOD_BIT(KC_LSFT) ? "@" : "*"); break;
        case TD_DOUBLE_TAP: SEND_STRING(mods & MOD_BIT(KC_LSFT) ? "@@" : "**"); break;
        case TD_MORE: SEND_STRING(mods & MOD_BIT(KC_LSFT) ? "@@@" : "***"); break;
        default: break;
    }
}

void td_kaa_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_kaa_state.state) {
        case TD_HOLD: layer_off(_KEYBD); break;
        default: break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_NAV_SHIFT_CAPSWORD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_nsc_finished, td_nsc_reset),
    [TD_KEYBD_ASTERISK_AT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_kaa_finished, td_kaa_reset),
};


// ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
// │ E N C O D E R                                                                                                                              │
// └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
// ▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘

#ifdef ENCODER_ENABLE

// ┌───────────────────────────────────────────────────────────┐
// │ l e f t   e n c o d e r                                   │
// └───────────────────────────────────────────────────────────┘

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_MS_WH_DOWN);
        } else {
            tap_code(KC_MS_WH_UP);
        }
        return false;

// ┌───────────────────────────────────────────────────────────┐
// │ r i g h t   e n c o d e r                                 │
// └───────────────────────────────────────────────────────────┘

    } else if (index == 1) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
        return false;
    }

    return true;
}

#endif // ENCODER_ENABLE




/*

                                                       ▐█    ▟▛ ▐█     ▄▆▀▀▀▀▀▀▆▄  ▐█▀▀▀▀▀█▌
                                                       ▐█   ▟▛  ▐█    ▟▛        ▜▙ ▐█     █▌
                                                       ▐█  ▟▛   ▐█   ▐█          █▋▐█     █▌
                                                       ▐█ ▟█▙   ▐█   ▐█          █▋▐█▀▀▜█▀▀▘
                                                       ▐█▟▛ ▜▙  ▐█    ▜▙        ▟▛ ▐█   ▜▙
                                                       ▐█▛   ▜▙ ▐█▄▄▄▄ ▀▜▆▄▄▄▄▆▛▀  ▐█    ▜▙

                                                                 ▄██████████████▄
                                                                 ████████████████
                                                            ▄██████▀  ▀████▀  ▀██████▄
                                                            ███████▄  ▄████▄  ▄███████
                                                            ███████████▀▀▀▀███████████
                                                            ▀█████████▀ ▄▄ ▀█████████▀
                                                                 ████▀ ▄██▄ ▀████
                                                                 ████▄▄████▄▄████

*/
