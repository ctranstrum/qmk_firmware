/* Copyright 2021 Mikael Manukyan <arm.localhost@gmail.com>
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

#include "quantum.h"

#ifdef RGB_MATRIX_LEDMAPS_ENABLED
#define RGB_MATRIX_LAYOUT_LEDMAP(                                           \
           lt5, lt4, lt3, lt2, lt1,           rt1, rt2, rt3, rt4, rt5,      \
      lm6, lm5, lm4, lm3, lm2, lm1,           rm1, rm2, rm3, rm4, rm5, rm6, \
      lb6, lb5, lb4, lb3, lb2, lb1,           rb1, rb2, rb3, rb4, rb5, rb6, \
                     lh4, lh3, lh2, lh1, rh1, rh2, rh3, rh4                 \
  ) {                                                                       \
      lh1, lh2, lh3, lb1, lm1, lt1, lt2,                                    \
      lm2, lb2, lh4, lb3, lm3, lt3, lt4,                                    \
      lm4, lb4, lb5, lm5, lt5, lm6, lb6,                                    \
      rh1, rh2, rh3, rb1, rm1, rt1, rt2,                                    \
      rm2, rb2, rh4, rb3, rm3, rt3, rt4,                                    \
      rm4, rb4, rb5, rm5, rt5, rm6, rb6                                     \
  }

typedef uint8_t ledmap[DRIVER_LED_TOTAL][3];
extern const ledmap ledmaps[];

void set_layer_rgb(uint8_t led_min, uint8_t led_max, int layer);

void rgb_matrix_layers_enable(void);
void rgb_matrix_layers_disable(void);

#define RED { 0, 255,
#define PNK { 0, 210,
#define ORG { 21, 255,
#define YLW { 43, 255,
#define LIM { 64, 225,
#define GRN { 85, 255,
#define SKY { 127, 255,
#define BLU { 169, 210,
#define PUR { 191, 225,
#define VLT { 211, 255,
#define WHT { 0, 0,

#define L1 115 }
#define L2 150 }
#define L3 185 }
#define L4 220 }
#define L5 255 }

#define ______ { 0, 0, 0 }

#endif  // RGB_MATRIX_LEDMAPS_ENABLED

void rgb_matrix_indicators_keymap(void);
void rgb_matrix_indicators_advanced_keymap(uint8_t led_min, uint8_t led_max);
