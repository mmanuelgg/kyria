/* Copyright 2022 Thomas Baart <thomas@splitkb.com>
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

#pragma once

//#ifdef WPM_ENABLE
//#    define SPLIT_WPM_ENABLE
//#endif

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_OLED_ENABLE
#define EE_HANDS

// #ifdef RGBLIGHT_ENABLE
// // Default
// #    define RGBLIGHT_EFFECT_BREATHING
// #    define RGBLIGHT_HUE_STEP 8
// #    define RGBLIGHT_SAT_STEP 8
// #    define RGBLIGHT_VAL_STEP 8
// #    ifndef RGBLIGHT_LIMIT_VAL
// #    define RGBLIGHT_LIMIT_VAL 150
// #    endif
// // New
// #    define RGBLIGHT_SLEEP
// #    define RGBLIGHT_TIMEOUT 60000
// #    define RGBLIGHT_SPLIT
// //#    define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_BREATHING
// #    define RGBLIGHT_DEFAULT_HUE 250
// //#    define RGBLIGHT_LAYERS
// #endif
#ifdef RGBLIGHT_ENABLE
    // #define RGBLIGHT_LIMIT_VAL 150  // max brightness cap (adjust to your liking)
    #define RGBLIGHT_SLEEP          // turn off underglow when suspended
    #define RGBLIGHT_TIMEOUT 60000  // timeout to turn off underglow
    #define RGBLIGHT_SPLIT          // split keyboard underglow sync
    // Don't enable animations
    // #define RGBLIGHT_EFFECT_BREATHING  <-- remove animations
#endif

// Tiempo entre pulsaciones para tap dance
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD

// Mouse config
// Constant mode
//#define MK_3_SPEED
//#define MK_MOMENTARY_ACCEL
//#define MK_C_OFFSET_UNMOD 5
//#define MK_C_OFFSET_0 4
//#define MK_C_OFFSET_1 16
// Accel mode
#define MOUSEKEY_MOVE_DELTA 5
#define MOUSEKEY_MAX_SPEED 7
#define MK_COMBINED
