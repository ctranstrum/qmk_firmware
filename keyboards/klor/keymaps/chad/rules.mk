# add custom functions
SRC += features/custom_shift_keys.c

# microprocessor
CONVERT_TO = liatris

# hardware onboard
OLED_ENABLE = yes
OLED_DRIVER = ssd1306
ENCODER_ENABLE = yes
# ENCODER_MAP_ENABLE = yes
EXTRAKEY_ENABLE = yes
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = ws2812
RGB_MATRIX_SUPPORTED = yes

# optional hardware
AUDIO_ENABLE = no
MUSIC_ENABLE = no
HAPTIC_ENABLE = no

# software options
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = yes
DYNAMIC_MACRO_ENABLE = no
KEY_OVERRIDE_ENABLE = no
MOUSEKEY_ENABLE = yes
TAP_DANCE_ENABLE = yes

# Vial config
VIAL_ENABLE = yes
VIAL_ENCODERS_ENABLE = yes
VIALRGB_ENABLE = yes
VIAL_INSECURE = yes
