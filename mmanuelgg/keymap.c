/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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
#include <stdio.h>
#include "doom_logo.c"
#include "stormlight_logo.c"
#include "hollow_knight_logo.c"

void leader_end_user(void){
    if (leader_sequence_one_key(KC_Q)){
        tap_code16(LGUI(KC_1));
    }
}
char wpm_str[10];
static uint16_t key_timer; // timer para controlar el tiempo
static void refresh_rgb(void); // revisa si ha habido algún tipo de actividad
static void check_rgb_timeout(void); // revisa si no ha habido actividad en el tiempo fijado para suspender RGB
bool is_rgb_timeout = false; // almacena si el RBG está o no suspendido

void refresh_rgb(){
    key_timer = timer_read();
    if (is_rgb_timeout){
        is_rgb_timeout = false;
        rgblight_wakeup();
    }
}

void check_rgb_timeout(){
    if (!is_rgb_timeout && timer_elapsed(key_timer) > RGBLIGHT_TIMEOUT){
        rgblight_suspend();
        is_rgb_timeout = true;
    }
}

void housekeeping_task_user(void){
#ifdef RGBLIGHT_TIMEOUT
    check_rgb_timeout();
#endif
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record){
#ifdef RGBLIGHT_TIMEOUT
    if (record->event.pressed) refresh_rgb();
#endif
}

void post_encoder_update_user(uint8_t index, bool clockwise){
#ifdef RGBLIGHT_TIMEOUT
    refresh_rgb();
#endif
}

// Function TAP DANCE
void tdStart(tap_dance_state_t *state, void *user_data){
    if (state->count == 1){
        register_code16(KC_SCLN);
    }else{
        SEND_STRING(SS_DOWN(X_RALT) SS_TAP(X_N));
        clear_keyboard();
    }
}

void tdReset(tap_dance_state_t *state, void *user_data){
    if (state->count == 1){
        unregister_code16(KC_SCLN);
    }
}

enum {
    TD_ENE,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_ENE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tdStart, tdReset),
};

enum layers {
    _QWERTY = 0,
    _DVORAK,
    _MIRYOKU,
    _GAMING,
    _GAME,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
    _NAVR,
    _MEDIA,
    _SYMBOLS,
    _NUM,
    _MOUSE,
};


// Aliases for readability
#define QWERTY   DF(_QWERTY)
#define DVORAK   DF(_DVORAK)
#define GAMING   DF(_GAMING)
#define MIRYOKU  DF(_MIRYOKU)

#define SYM      MO(_SYM)
#define NAV      MO(_NAV)
#define FKEYS    MO(_FUNCTION)
#define ADJUST   MO(_ADJUST)
#define GAME     MO(_GAME)

#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT  MT(MOD_LALT, KC_ENT)
#define ALG_ENT  MT(MOD_RALT, KC_ENT)

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | [ {  |VolMut|  |F-keys|  ] } |   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr|  Nav | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
     KC_TAB  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,                                        KC_Y,   KC_U ,  KC_I ,   KC_O ,  KC_P , KC_BSPC,
     CTL_ESC , KC_A ,  KC_S   ,  KC_D  ,   KC_F ,   KC_G ,                                        KC_H,   KC_J ,  KC_K ,   KC_L , KC_SCLN, CTL_QUOT, //TD(TD_ENE), CTL_QUOT,
     KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , KC_LBRC,KC_MUTE,     FKEYS  , KC_RBRC, KC_N,   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                ADJUST , KC_LGUI,ALT_T(KC_SPC),KC_ENT,NAV ,     SYM    , KC_SPC , ALG_ENT , NAV , QK_LEAD
    ),

/*
 * Base Layer: Dvorak
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   | ' "  | , <  | . >  |   P  |   Y  |                              |   F  |   G  |   C  |   R  |   L  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   O  |   E  |   U  |   I  |                              |   D  |   H  |   T  |   N  |   S  |Ctrl/- _|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift | ; :  |   Q  |   J  |   K  |   X  | [ {  |VolMut|  |F-keys|  ] } |   B  |   M  |   W  |   V  |   Z  | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_DVORAK] = LAYOUT(
     KC_TAB  ,KC_QUOTE,KC_COMM,  KC_DOT,   KC_P ,   KC_Y ,                                        KC_F,   KC_G ,  KC_C ,   KC_R ,  KC_L , KC_BSPC,
     CTL_ESC , KC_A ,  KC_O   ,  KC_E  ,   KC_U ,   KC_I ,                                        KC_D,   KC_H ,  KC_T ,   KC_N ,  KC_S , CTL_MINS,
     KC_LSFT ,KC_SCLN, KC_Q   ,  KC_J  ,   KC_K ,   KC_X , KC_LBRC,KC_MUTE,     FKEYS  , KC_RBRC, KC_B,   KC_M ,  KC_W ,   KC_V ,  KC_Z , KC_RSFT,
                                 ADJUST, KC_LGUI,ALT_T(KC_SPC),KC_ENT, NAV,     SYM    , KC_SPC ,ALG_ENT, NAV  , QK_LEAD
    ),

    // Base Layer: MIRYOKU
    [_MIRYOKU] = LAYOUT(
     KC_TAB  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,                                        KC_Y,   KC_U ,  KC_I ,   KC_O ,  KC_P , KC_BSPC,
     CTL_ESC , LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F),   KC_G ,                   KC_H,   RSFT_T(KC_J) , RCTL_T(KC_K) , RALT_T(KC_L) , RGUI_T(KC_SCLN), CTL_QUOT,
     KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , KC_LBRC,KC_MUTE, FKEYS ,    KC_RBRC, KC_N,   KC_M ,KC_COMM, ALGR_T(KC_DOT) ,KC_SLSH, KC_RSFT,
            ADJUST , KC_LGUI,LT(_MEDIA, KC_ESC),LT(_NAVR, KC_SPC),LT(_MOUSE, KC_TAB),    LT(_SYM, KC_BSPC), LT(_NUM, KC_ENT) , LT(_FUNCTION, KC_DEL) , NAV , QK_LEAD
    ),

    [_NAVR] = LAYOUT(
      _______,  _______,  _______,   _______,   _______,   _______,                                       KC_COPY, KC_UNDO, KC_CUT,  KC_AGIN, KC_PSTE, _______,
      KC_CAPS,  KC_LGUI, KC_LALT , KC_LCTL  , KC_LSFT  ,   _______,                                       KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
      _______,  _______,   KC_ALGR, _______,   _______ ,   _______, _______, _______,  _______, _______,  KC_INS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,   _______,
                                _______, _______,   _______ , _______, _______,        KC_ENT,  KC_BSPC, KC_DEL,  _______,  _______
    ),

    [_MEDIA] = LAYOUT(
      _______,  _______,  _______,   _______,   _______,   _______,                                       KC_AGIN, KC_UNDO, KC_CUT,  KC_COPY, KC_PSTE, _______,
      _______,  KC_LGUI, KC_LALT , KC_LCTL  , KC_LSFT  ,   _______,                                       KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,_______,  _______,
      _______,  _______,   KC_ALGR, _______,   _______ ,   _______, _______, _______,  _______, _______,  _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,   _______,
                                _______, _______,   _______ , _______, _______,        KC_MSTP,  KC_MPLY, KC_MUTE,  _______,  _______
    ),
    [_SYMBOLS] = LAYOUT(
     KC_GRV ,   KC_1 ,   KC_2 ,   KC_3 ,   KC_4 ,   KC_5 ,                                      KC_6   , KC_7, KC_8 , KC_9,   KC_0 , KC_ASTR,
     KC_TILD , KC_EXLM,  KC_AT , KC_HASH,  KC_DLR, KC_PERC,                                     KC_CIRC, KC_4, KC_5 , KC_6, KC_EQL , KC_PLUS,
     KC_PIPE , KC_BSLS, KC_COLN, KC_SCLN, KC_MINS, KC_LBRC, KC_LCBR, _______, _______, KC_RCBR, KC_RBRC, KC_1, KC_2 , KC_3, KC_SLSH, KC_QUES,
                                 _______, _______, CW_TOGG, _______, _______, _______, _______, _______, _______, _______
    ),
    [_NUM] = LAYOUT(
     _______, KC_ASTR, KC_7, KC_8 , KC_9,   KC_0 ,                                     _______, KC_7, KC_8 , KC_9,   KC_0 , KC_ASTR,
     _______, KC_PLUS, KC_4, KC_5 , KC_6, KC_EQL ,                                     KC_CIRC, KC_4, KC_5 , KC_6, KC_EQL , KC_PLUS,
     _______, KC_MINS, KC_1, KC_2 , KC_3, KC_SLSH, _______, _______, _______, KC_RCBR, KC_RBRC, KC_1, KC_2 , KC_3, KC_SLSH, KC_QUES,
                        _______, _______, CW_TOGG, KC_0   , _______, _______, _______, _______, _______, _______
    ),
    [_MOUSE] = LAYOUT(
      _______,  _______,  _______,   _______,   _______,   _______,                                        KC_COPY, KC_UNDO, KC_CUT,  KC_AGIN, KC_PSTE, _______,
      _______,  KC_LGUI, KC_LALT , KC_LCTL  , KC_LSFT  ,   _______,                                        KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______,
      _______,  _______,   KC_ALGR, _______,   _______ ,   _______, _______, _______,  _______,  _______,  KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_D, _______, _______,
                                _______, _______,   _______ , _______, _______,        KC_BTN3,  KC_BTN1, KC_BTN2,  KC_BTN4,  KC_BTN5
    ),

/*
 * Base Layer: GAMING
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   | ESC  |   Q  |   W  |   E  |   R  |                              |   Y  |   U  |   ↑  |   O  |   P  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    G   |LShift|   A  |   S  |   D  |   F  |                              |   H  |  ←   |   ↓  |   →  | ;  : |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |    T   |LCtrl |   Z  |   X  |   C  |   V  |  B   |VolMut|  |F-keys|  ] } |   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust|   M  | LAlt | Space| Game |  | Sym  | Space| AltGr|  Nav | Menu |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_GAMING] = LAYOUT(
     KC_TAB  , KC_ESC  ,  KC_Q  ,  KC_W  ,  KC_E ,  KC_R ,                                        KC_Y,  KC_U , KC_UP ,  KC_O ,  KC_P , KC_BSPC,
     KC_G    , KC_LSFT ,  KC_A  ,  KC_S  ,  KC_D ,  KC_F ,                                        KC_H,KC_LEFT,KC_DOWN,KC_RGHT,KC_SCLN, CTL_QUOT,
     KC_T    , KC_LCTL ,  KC_Z  ,  KC_X  ,  KC_C ,  KC_V , KC_B , KC_MUTE,     FKEYS  , KC_RBRC, TD(TD_ENE),   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                  ADJUST ,  KC_M, KC_LALT ,KC_SPC , GAME ,     SYM    , KC_SPC ,KC_RALT, NAV , QK_LEAD
    ),

/*
 * Game Layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |   1  |   2  |   3  |   4  |   5  |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |   I  |   J  |   K  |   L  |      |                              |      |      |      |      |      | Enter  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
     [_GAME] = LAYOUT(
       _______,   KC_1 ,   KC_2 ,   KC_3 ,   KC_4 ,   KC_5 ,                                     _______, _______, _______, _______, _______, _______,
       _______,   KC_I ,   KC_J ,  KC_K ,   KC_L  , _______,                                     _______, _______, _______, _______, _______, KC_ENT ,
       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
     ),

/*
 * Nav Layer: Media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        | LPAD |      |      |      |      |                              | Home | End  | PgUp | PgDn | WHUP | Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  MCTL  |MSACL1| BTN2 |  M↑  | BTN3 | BTN5 |                              |  ←   |   ↓  |   ↑  |   →  | WHDN | Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | MSACL0 | BTN1 |  M←  |  M↓  |  M→  | BTN4 |      | GUI  |  |      | WHLT | WHRT | BTN1 | BTN2 | BTN3 | BTN4 | PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |M Prev|M Play|M Next| BTN5 |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      KC_LPAD, _______, _______, _______, _______, _______,                                     KC_HOME, KC_END , KC_PGUP, KC_PGDN, KC_WH_U, KC_DEL,
      KC_MCTL, KC_ACL1, KC_BTN2, KC_MS_U, KC_BTN3, KC_BTN5,                                     KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, KC_WH_D, KC_INS,
      KC_ACL0, KC_BTN1, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN4, _______, KC_LGUI, _______, KC_WH_L, KC_WH_R, KC_BTN1, KC_BTN2, KC_BTN3, KC_BTN4, KC_PSCR,
                                 _______, _______, _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_BTN5, _______
    ),

/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |    `   |  1   |  2   |  3   |  4   |  5   |                              |   6  |  7   |  8   |  9   |  0   |   =    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    ~   |  !   |  @   |  #   |  $   |  %   |                              |   ^  |  &   |  *   |  (   |  )   |   +    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |    |   |   \  |  :   |  ;   |  -   |  [   |  {   |      |  |      |   }  |   ]  |  _   |  ,   |  .   |  /   |   ?    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
      KC_GRV ,   KC_1 ,   KC_2 ,   KC_3 ,   KC_4 ,   KC_5 ,                                       KC_6 ,   KC_7 ,   KC_8 ,   KC_9 ,   KC_0 , KC_EQL ,
     KC_TILD , KC_EXLM,  KC_AT , KC_HASH,  KC_DLR, KC_PERC,                                     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
     KC_PIPE , KC_BSLS, KC_COLN, KC_SCLN, KC_MINS, KC_LBRC, KC_LCBR, _______, _______, KC_RCBR, KC_RBRC, KC_UNDS, KC_COMM,  KC_DOT, KC_SLSH, KC_QUES,
                                 _______, _______, CW_TOGG, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Function Layer: Function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F9  | F10  | F11  | F12  |      |                              |   *  |   7  |   8  |   9  |   /  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | CapsLk |  F5  |  F6  |  F7  |  F8  |      |                              |   +  |   4  |   5  |   6  |   0  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |   -  |   1  |   2  |   3  |   =  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FUNCTION] = LAYOUT(
      _______,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, _______,                                     KC_PAST,   KC_7 ,   KC_8 ,   KC_9 , KC_PSLS, _______,
      KC_CAPS,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , _______,                                     KC_PPLS,   KC_4 ,   KC_5 ,   KC_6 ,   KC_0 , _______,
      KC_APP,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , _______, _______, KC_MPLY, _______, _______, KC_PMNS,   KC_1 ,   KC_2 ,   KC_3 , KC_PEQL, _______,
                                 _______, _______, KC_APP, KC_SPC, KC_TAB, _______, _______, _______, _______, _______
    ),

/*
 * Adjust Layer: Default layer settings, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |QWERTY|GAMING|QWERTY|      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |Dvorak|      |      |                              | TOG  | SAI  | HUI  | VAI  | MOD  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |Colmak|      |      |      |      |  |      |      |      | SAD  | HUD  | VAD  | RMOD |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, QWERTY , GAMING , MIRYOKU , _______, _______,                                    _______, _______, _______, _______,  _______, _______,
      _______, _______, _______, DVORAK , _______, _______,                                    RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI,  RGB_MOD, _______,
      _______, _______, _______, _______, _______, _______,_______, _______, _______, _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, _______,
                                 _______, _______, _______,_______, _______, _______, _______, _______, _______, _______
    ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

// My oled is 128x64
#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

static void render_status(void){
    /*
       static const char PROGMEM qmk_logo[] = {
       0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
       0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
       0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
    */
    //oled_write_P(qmk_logo, false);
    oled_write_P(PSTR("mmanuelgg Journey\n"), false);
    oled_write_P(PSTR("          Before\n"), false);
    sprintf(wpm_str, "WPM: %03d", get_current_wpm());
    oled_write_P(wpm_str, false);
    oled_write_P(PSTR("  Destination\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state|default_layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("QWERTY\n"), false);
            break;
        case _MIRYOKU:
            oled_write_P(PSTR("MIRYOKU\n"), false);
            break;
        case _DVORAK:
            oled_write_P(PSTR("DVORAK\n"), false);
            break;
        case _GAMING:
            oled_write_P(PSTR("GAMING\n"), false);
            break;
        case _GAME:
            oled_write_P(PSTR("GAME\n"), false);
            break;

        case _NAV:
            oled_write_P(PSTR("NAV\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("SYM\n"), false);
            break;
        case _FUNCTION:
            oled_write_P(PSTR("FUNCTION\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("ADJUST\n"), false);
            break;
        case _SYMBOLS:
            oled_write_P(PSTR("SYMBOLS\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("NUM\n"), false);
            break;
        case _MEDIA:
            oled_write_P(PSTR("MEDIA\n"), false);
            break;
        case _NAVR:
            oled_write_P(PSTR("NAVR\n"), false);
            break;
        case _MOUSE:
            oled_write_P(PSTR("MOUSE\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    // Write host Keyboard LED Status to OLEDs
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(PSTR("\n"), false);
    oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
    //oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);

}

static void render_image(void){
    static const char PROGMEM kyria_logo[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
        0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };

    switch (get_highest_layer(layer_state|default_layer_state)) {
        case _QWERTY:
            oled_write_raw_P(hollow_knight_logo, sizeof(hollow_knight_logo));
            break;
        case _MIRYOKU:
            oled_write_raw_P(hollow_knight_logo, sizeof(hollow_knight_logo));
            break;
        case _DVORAK:
            oled_write_raw_P(stormlight_logo, sizeof(stormlight_logo));
            break;
        case _GAMING:
            oled_write_raw_P(doom_logo, sizeof(doom_logo));
            break;
        default:
            oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // clang-format off
        // clang-format on
        render_status();
    } else {
        // clang-format off
        // clang-format on
        render_image();
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {

    if (index == 0) {
        // Volume control
        if (clockwise) {
            switch (get_highest_layer(layer_state|default_layer_state)) {
                case _FUNCTION:
                    tap_code(KC_MNXT);
                    break;
                case _SYM:
                    tap_code(KC_RGHT);
                    break;
                case _NAV:
                    tap_code(KC_DOWN);
                    break;
                default:
                    tap_code(KC_VOLU);
            }
        } else {
            switch (get_highest_layer(layer_state|default_layer_state)) {
                case _FUNCTION:
                    tap_code(KC_MPRV);
                    break;
                case _SYM:
                    tap_code(KC_LEFT);
                    break;
                case _NAV:
                    tap_code(KC_UP);
                    break;
                default:
                    tap_code(KC_VOLD);
            }
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            switch (get_highest_layer(layer_state|default_layer_state)) {
                case _FUNCTION:
                    tap_code(KC_WH_R);
                    break;
                case _NAV:
                    tap_code(KC_RGHT);
                    break;
                default:
                    tap_code(KC_WH_U);
            }
        } else {
            switch (get_highest_layer(layer_state|default_layer_state)) {
                case _FUNCTION:
                    tap_code(KC_WH_L);
                    break;
                case _NAV:
                    tap_code(KC_LEFT);
                    break;
                default:
                    tap_code(KC_WH_D);
            }
        }
    }
    return false;
}
#endif

#ifdef RGBLIGHT_LAYERS
void keyboard_post_init_user(void){
    rgblight_enable_noeeprom();
    layer_state_set_user(layer_state);
}
layer_state_t layer_state_set_user(layer_state_t state){
    switch (get_highest_layer(default_layer_state)) {
        case _GAMING:
            rgblight_sethsv_noeeprom(HSV_CYAN);
            break;
        case _DVORAK:
            rgblight_sethsv_noeeprom(HSV_BLUE);
            break;
        default:
            rgblight_sethsv_noeeprom(HSV_PURPLE);
            break;
    }
    return state;
}
#endif
