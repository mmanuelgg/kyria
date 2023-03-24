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

#ifdef RGBLIGHT_ENABLE
// Default
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_HUE_STEP 8
#    define RGBLIGHT_SAT_STEP 8
#    define RGBLIGHT_VAL_STEP 8
#    ifndef RGBLIGHT_LIMIT_VAL
#    	define RGBLIGHT_LIMIT_VAL 150
#    endif
// New
#    define RGBLIGHT_SLEEP
#    define RGBLIGHT_TIMEOUT 60000
#    define RGBLIGHT_SPLIT
#    define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_BREATHING
#    define RGBLIGHT_DEFAULT_HUE 250
#    define RGBLIGHT_LAYERS
#endif

// Lets you roll mod-tap keys
#define IGNORE_MOD_TAP_INTERRUPT

// Tiempo entre pulsaciones para tap dance
#define TAPPING_TERM 170
