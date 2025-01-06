/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//#define USE_MATRIX_I2C

/* Select hand configuration */

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

//#define TAPPING_FORCE_HOLD

// Home row mods
#define TAPPING_TERM 175
#define QUICK_TAP_TERM 90
#define PERMISSIVE_HOLD

#define _HOME_T LSFT_T(KC_T)
#define _HOME_S LCTL_T(KC_S)
#define _HOME_R LALT_T(KC_R)
#define _HOME_A LGUI_T(KC_A)

#define _HOME_N RSFT_T(KC_N)
#define _HOME_E RCTL_T(KC_E)
#define _HOME_I LALT_T(KC_I)
#define _HOME_O LGUI_T(KC_O)

#define _HOME_LT LGUI_T(KC_LEFT)
#define _HOME_DW LALT_T(KC_DOWN)
#define _HOME_RT LCTL_T(KC_RIGHT)

// Layers
// Base layers
#define _COLEMAK 0
#define _QWERTY 1

// Numeric layer
#define _LAYER_NUMERIC 2
#define _LN_SPC LT(_LAYER_NUMERIC, KC_SPC)

// Symbol layer
#define _LAYER_SYMBOLS 3
#define _LS_ENT LT(_LAYER_SYMBOLS, KC_ENT)
#define _LS_MIN LT(_LAYER_SYMBOLS, KC_MINS)

// Split keyboard features
#define SPLIT_MODS_ENABLE
#define SPLIT_LAYER_STATE_ENABLE

#ifdef RGBLIGHT_ENABLE
    #define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_RAINBOW_MOOD
    #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
    #define RGBLIGHT_EFFECT_SNAKE
    #define RGBLIGHT_EFFECT_KNIGHT
    #define RGBLIGHT_EFFECT_CHRISTMAS
    #define RGBLIGHT_EFFECT_STATIC_GRADIENT
    #define RGBLIGHT_EFFECT_RGB_TEST
    #define RGBLIGHT_EFFECT_ALTERNATING
    #define RGBLIGHT_EFFECT_TWINKLE
    #define RGBLIGHT_LIMIT_VAL 120
    #define RGBLIGHT_HUE_STEP 10
    #define RGBLIGHT_SAT_STEP 17
    #define RGBLIGHT_VAL_STEP 17
#endif
