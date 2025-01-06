/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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

#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include <stdbool.h>

enum keycodes {
    _KC_NXT = QK_USER,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEMAK] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _KC_NXT,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                         KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, KC_UNDS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, _HOME_A, _HOME_R, _HOME_S, _HOME_T,    KC_D,                         KC_H, _HOME_N, _HOME_E, _HOME_I, _HOME_O, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,  KC_TAB, _LN_SPC,    _LS_ENT, _LS_MIN, KC_BSPC
                                      //`--------------------------'  `--------------------------'
  ),

  [_QWERTY] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _KC_NXT,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_UNDS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,  KC_TAB, _LN_SPC,    _LS_ENT, _LS_MIN, KC_BSPC
                                      //`--------------------------'  `--------------------------'
  ),

  [_LAYER_NUMERIC] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, XXXXXXX,   KC_UP, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,    KC_7,    KC_8,    KC_9, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,_HOME_LT,_HOME_DW,_HOME_RT, KC_LSFT, XXXXXXX,                      KC_MINS,    KC_4,    KC_5,    KC_6, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS,    KC_1,    KC_2,    KC_3, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______,    KC_0
                                      //`--------------------------'  `--------------------------'
  ),

  [_LAYER_SYMBOLS] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_LPRN, KC_RPRN, KC_RALT, KC_UNDS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, KC_SLSH, KC_ASTR, KC_MINS, KC_PLUS,  KC_EQL,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_ASTR, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______,  KC_DEL
                                      //`--------------------------'  `--------------------------'
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case _KC_NXT:
      if (!record->event.pressed) {
        return false;
      }

      uint8_t current_layer = get_highest_layer(default_layer_state);
      switch (current_layer) {
        case _COLEMAK:
            set_single_default_layer(_QWERTY);
            break;
        case _QWERTY:
        default:
            set_single_default_layer(_COLEMAK);
            break;
      }
      return false;
    default:
      return true;
  }
}

void keyboard_pre_init_user(void) {
    // turn off power LED - it's way too bright
    gpio_set_pin_output(24);
    gpio_write_pin_high(24);
}

#ifdef OLED_ENABLE
static unsigned char PROGMEM current_time_buf[] = "00:00";

uint8_t current_battery_perc = 0;
uint8_t current_cpu_perc = 0;
uint8_t current_ram_perc = 0;


static unsigned char PROGMEM current_battery_perc_buf[] = "100";
static unsigned char PROGMEM current_cpu_perc_buf[] = "100";
static unsigned char PROGMEM current_ram_perc_buf[] = "100";

// Convert the number into a null-terminated string. The buffer must accomodate the null-terminator.
//
// Returns the amount of digits that were written out.
uint8_t to_string(unsigned char number, unsigned char *buf, size_t buf_size) {
    uint8_t num_digits = 0;
    uint8_t max_num_digits = (uint8_t) buf_size - 1;

    if (max_num_digits == 0) {
        return 0;
    }

    if (number == 0) {
        buf[0] = '0';
        ++num_digits;
    } else {
        while (number > 0 && num_digits < max_num_digits) {
            uint8_t num = number % 10;
            buf[buf_size - num_digits - 1] = num + '0';

            ++num_digits;
            number /= 10;
        }

        for (int i = 0; i < num_digits; i++) {
            buf[i] = buf[buf_size - num_digits + i];
        }
    }
    for (int i = num_digits; i < buf_size; i++) {
        buf[i] = 0x0;
    }

    return num_digits;
}

// Format the hour and minute values into format "HH:MM".
uint8_t format_time(unsigned char hour, unsigned char minute, unsigned char *buf, size_t buf_size) {
    static unsigned char PROGMEM hour_buf[3];
    static unsigned char PROGMEM minute_buf[3];

    if (buf_size != 6) {
        return 2;
    }

    uint8_t num_hour_digits = to_string(hour, hour_buf, 3);
    if (num_hour_digits < 1) {
        return 1;
    }

    uint8_t num_minute_digits = to_string(minute, minute_buf, 3);
    if (num_minute_digits < 1) {
        return 1;
    }

    if (num_hour_digits == 1) {
        hour_buf[1] = hour_buf[0];
        hour_buf[0] = '0';
    }

    if (num_minute_digits == 1) {
        minute_buf[1] = minute_buf[0];
        minute_buf[0] = '0';
    }

    buf[0] = hour_buf[0];
    buf[1] = hour_buf[1];
    buf[2] = ':';
    buf[3] = minute_buf[0];
    buf[4] = minute_buf[1];

    return 0;
}

uint8_t write_battery_perc(void) {
    uint8_t perc = current_battery_perc;
    if (perc > 100) {
        perc = 100;
    }

    static const char PROGMEM battery_100[] = {0x7E, 0x7F, 0x7F, 0x7F, 0x7E, 0x00};
    static const char PROGMEM battery_75[] = {0x7E, 0x7B, 0x7B, 0x7B, 0x7E, 0x00};
    static const char PROGMEM battery_50[] = {0x7E, 0x73, 0x73, 0x73, 0x7E, 0x00};
    static const char PROGMEM battery_25[] = {0x7E, 0x63, 0x63, 0x63, 0x7E, 0x00};

    if (perc <= 25) {
        oled_write_raw(battery_25, sizeof(battery_25));
    } else if (perc <= 50) {
        oled_write_raw(battery_50, sizeof(battery_50));
    } else if (perc <= 75) {
        oled_write_raw(battery_75, sizeof(battery_75));
    } else {
        oled_write_raw(battery_100, sizeof(battery_100));
    }

    oled_advance_char();

    oled_write((char*) current_battery_perc_buf, false);
    oled_write_char('%', false);

    return (uint8_t) strlen((char*) current_battery_perc_buf) + 2;
}

uint8_t write_cpu_perc(void) {
    uint8_t perc = current_cpu_perc;
    if (perc > 100) {
        perc = 100;
    }

    static const char PROGMEM cpu_symbol[] = {0x08, 0x3E, 0x14, 0x3E, 0x08, 0x00};
    oled_write_raw(cpu_symbol, sizeof(cpu_symbol));
    oled_advance_char();

    oled_write((char*) current_cpu_perc_buf, false);
    oled_write_char('%', false);

    return (uint8_t) strlen((char*) current_cpu_perc_buf) + 2;
}

uint8_t write_ram_perc(void) {
    uint8_t perc = current_ram_perc;
    if (perc > 100) {
        perc = 100;
    }

    static const char PROGMEM ram_symbol[] = {0x3E, 0x14, 0x36, 0x14, 0x3E, 0x00};
    oled_write_raw(ram_symbol, sizeof(ram_symbol));
    oled_advance_char();

    oled_write((char*) current_ram_perc_buf, false);
    oled_write_char('%', false);

    return (uint8_t) strlen((char*) current_ram_perc_buf) + 2;
}

static const size_t BYTE_HOUR = 0;
static const size_t BYTE_MINUTE = 1;
static const size_t BYTE_BATTERY = 2;
static const size_t BYTE_CPU = 3;
static const size_t BYTE_RAM = 4;

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (!is_oled_on()) {
        // If we don't do this, any receive will cause the OLED to wake up from sleep,
        // which is kind of silly. We only want to display new data on the screen when
        // the keyboard is in use.
        return;
    }

    format_time(data[BYTE_HOUR], data[BYTE_MINUTE], current_time_buf, sizeof(current_time_buf));
    current_battery_perc = data[BYTE_BATTERY];
    current_cpu_perc = data[BYTE_CPU];
    current_ram_perc = data[BYTE_RAM];

    // We need to compute these on receive as doing it on writing to the OLED takes too long
    to_string(current_battery_perc, current_battery_perc_buf, sizeof(current_battery_perc_buf));
    to_string(current_cpu_perc, current_cpu_perc_buf, sizeof(current_cpu_perc_buf));
    to_string(current_ram_perc, current_ram_perc_buf, sizeof(current_ram_perc_buf));
}

void ensure_blank_line(uint8_t chars_written) {
    static const uint8_t max_chars_per_line = 5;
    if (chars_written % max_chars_per_line != 0) {
        oled_advance_page(true);
    }
    oled_advance_page(true);
}

uint8_t write_layer(char* label, bool is_on) {
    if (is_on) {
        oled_write_char('*', true);
    } else {
        oled_write_char(' ', false);
    }

    oled_write_P(label, is_on);
    return 4;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_write((char*) current_time_buf, false);
        oled_advance_page(true);

        uint8_t bat_chars_written = write_battery_perc();
        ensure_blank_line(bat_chars_written);

        uint8_t cpu_chars_written = write_cpu_perc();
        ensure_blank_line(cpu_chars_written);

        uint8_t ram_chars_written =write_ram_perc();
        ensure_blank_line(ram_chars_written);
        return false;
    } else {
        uint8_t default_layer = get_highest_layer(default_layer_state);
        char *default_layer_label = "CMK";

        oled_write_P(PSTR("LYT:"), false);
        oled_advance_page(true);
        if (default_layer == _COLEMAK) {
            default_layer_label = "CMK";
        } else if (default_layer == _QWERTY) {
            default_layer_label = "QWR";
        }
        oled_write_P(default_layer_label, false);
        ensure_blank_line(3);

        uint8_t highest_layer = get_highest_layer(layer_state);
        oled_write_P(PSTR("LYR:"), false);
        oled_advance_page(true);
        write_layer(default_layer_label, highest_layer == _COLEMAK);
        oled_advance_page(true);
        write_layer("SYM", highest_layer == _LAYER_SYMBOLS);
        oled_advance_page(true);
        ensure_blank_line(write_layer("NUM", highest_layer == _LAYER_NUMERIC));

        uint8_t mods = get_mods();

        oled_write_char('S', mods & MOD_MASK_SHIFT);
        oled_write_char('C', mods & MOD_MASK_CTRL);
        oled_write_char('M', mods & MOD_MASK_ALT);
        oled_write_char('S', mods & MOD_MASK_GUI);

        return false;
    }
}
#endif // OLED_ENABLE
