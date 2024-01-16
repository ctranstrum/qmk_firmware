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
#include "rgb_matrix_ledmaps.h"

__attribute__((weak)) void rgb_matrix_indicators_keymap(void) { return; }
__attribute__((weak)) void rgb_matrix_indicators_advanced_keymap(uint8_t led_min, uint8_t led_max) {
    return;
}

#ifdef RGB_MATRIX_LEDMAPS_ENABLED

static bool enabled = true;

#endif  // RGB_MATRIX_LEDMAPS_ENABLED

bool rgb_matrix_indicators_user(void) { rgb_matrix_indicators_keymap(); return true; }
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
#ifdef RGB_MATRIX_LEDMAPS_ENABLED
    if (rgb_matrix_is_enabled() && enabled) {
        set_layer_rgb(led_min, led_max, get_highest_layer(layer_state | default_layer_state));
    }

#endif  // RGB_MATRIX_LEDMAPS_ENABLED
    rgb_matrix_indicators_advanced_keymap(led_min, led_max);
    return true;
}

#ifdef RGB_MATRIX_LEDMAPS_ENABLED

void set_layer_rgb(uint8_t led_min, uint8_t led_max, int layer) {
    const ledmap *l = &(ledmaps[layer]);

    uint8_t val = rgb_matrix_get_val();

    const uint8_t mods = get_mods() | get_oneshot_mods() | get_weak_mods();
    const led_t led_state = host_keyboard_led_state();

    for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
        HSV hsv = {
            .h = (*l)[i][0],
            .s = (*l)[i][1],
            .v = (*l)[i][2] * val / 255,
        };

        if (i == 1 && layer < 3) {
            if (mods & MOD_BIT(KC_LSFT)) {
                hsv.h = 21; hsv.s = 255; hsv.v = val;
            }
            if (is_caps_word_on()) {
                hsv.h = 180; hsv.s = 255; hsv.v = 191 * val / 255;
            }
            if (led_state.caps_lock) {
                hsv.h = 0; hsv.s = 255; hsv.v = 191 * val / 255;
            }
        }

        RGB rgb = hsv_to_rgb(hsv);
        RGB_MATRIX_INDICATOR_SET_COLOR(i, rgb.r, rgb.g, rgb.b);
    }
}

void rgb_matrix_layers_enable() {
    dprintf("ledmaps are enabled\n");
    enabled = true;
}

void rgb_matrix_layers_disable() {
    dprintf("ledmaps are disabled\n");
    enabled = false;
}

#endif  // RGB_MATRIX_LEDMAPS_ENABLED
