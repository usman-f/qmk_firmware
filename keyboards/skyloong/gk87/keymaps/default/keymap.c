// Version 1

#include QMK_KEYBOARD_H

// Track Alt+Tab state for switching windows
bool is_alt_tab_active = false;

// Custom keycodes for macros
enum custom_keycodes {
    ALT_TAB = SAFE_RANGE,  // Super Alt+Tab macro
    DESK_NEXT,             // Switch to next virtual desktop
    DESK_PREV              // Switch to previous virtual desktop
};

// Define Home Row Mod for Layer Switching
#define LT2_L LT(2, KC_L)  // Layer 2 when held, KC_L when tapped

// Structure to track key press events
typedef struct {
    uint16_t keycode;  // Key pressed
    uint16_t time;     // Timestamp of press
} key_event_t;

#define MAX_TRACKED_KEYS 10  // Maximum number of keys tracked

// Queue for tracking key press order
static key_event_t key_event_queue[MAX_TRACKED_KEYS];
static uint8_t queue_length = 0;

// Adds key event to queue
void add_to_queue(uint16_t keycode, uint16_t time) {
    if (queue_length < MAX_TRACKED_KEYS) {
        key_event_queue[queue_length].keycode = keycode;
        key_event_queue[queue_length].time = time;
        queue_length++;
    }
}

// Checks if a key was the first pressed in the sequence
bool was_first_pressed(uint16_t keycode) {
    return queue_length > 0 && key_event_queue[0].keycode == keycode;
}

// Resets the key event queue (useful when changing layers)
void reset_queue(void) {
    queue_length = 0;
}

// Define keyboard layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(
        KC_ESC,              KC_F1,    KC_F2,    KC_F3,    KC_F4,       KC_F5,     KC_F6,     KC_F7,    KC_F8,     KC_F9,    KC_F10,   KC_F11,       KC_F12,   KC_MUTE,  BL_TOGG, LSG(KC_S),
        KC_GRV,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5,        KC_6,     KC_7,       KC_8,     KC_9,      KC_0,   KC_MINS,   KC_EQL,      KC_BSPC,    KC_INS,  KC_HOME,   KC_PGUP,
        KC_TAB,     KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,        KC_Y,     KC_U,       KC_I,     KC_O,      KC_P,   KC_LBRC,  KC_RBRC,      KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,
        KC_LCTL,     KC_A,     KC_S,     KC_D,     KC_F,     KC_G,        KC_H,     KC_J,       KC_K,     LT2_L,   KC_SCLN,   KC_QUOT,                 KC_ENT,
        KC_LSFT,     KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,        KC_N,     KC_M,    KC_COMM,   KC_DOT,   KC_SLSH,                           KC_RSFT,               KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,             KC_SPC,   KC_SPC,                 KC_SPC,             KC_RALT,     MO(1),   KC_MENU,                KC_RCTL,   KC_LEFT,  KC_DOWN,    KC_RGHT
    ),

    [1] = LAYOUT_all(
        _______,             KC_F14,   KC_F15, G(KC_TAB),  KC_WSCH,  G(C(KC_S)),   KC_SLEP,   KC_MPRV,  KC_MPLY,   KC_MNXT,   KC_MUTE,  KC_VOLD,     KC_VOLU,    KC_PSCR,  KC_SCRL,   KC_PAUS,
        _______,  _______,  _______,  _______,   _______,  _______,     _______,   _______,   _______,  _______,   BL_TOGG,   BL_BRTG,  BL_BRTG,      KC_DEL,     KC_INS,  KC_HOME,   KC_PGUP,
        _______,  _______,  _______,  _______,   _______,  _______,     _______,   _______,   _______,  _______,   _______,   _______,  _______,     _______,     KC_DEL,   KC_END,   KC_PGDN,
        _______,  _______,  _______,  _______,   _______,  _______,     _______,   _______,   _______,  _______,   _______,   _______,               _______,
        _______,  _______,  _______,  _______,   _______,  _______,     _______,   _______,   _______,  _______,   _______,                          _______,                BL_UP,
        _______,  _______,  _______,             _______,  _______,                _______,             _______,   _______,   _______,               _______,    KC_LEFT,  BL_DOWN,   KC_RGHT
    ),

    [2] = LAYOUT_all(
        _______,                      _______,      _______,      _______,      _______,       _______,   _______,   _______,  _______,   _______,        _______,  _______,     _______,     _______,  _______,  _______,
        _______,        _______,      _______,      _______,      _______,      _______,       _______,   _______,   _______,  _______,   _______,        _______,  _______,     _______,     _______,  _______,  _______,
        ALT_TAB,   LCTL(KC_F13), LCTL(KC_F14), LCTL(KC_F15), LCTL(KC_F16), LCTL(KC_F17),       _______,    KC_F21,    KC_F22,   KC_F23,    KC_F24,   LCTL(KC_F20),  _______,     _______,     _______,  _______,  _______,
        DESK_PREV,   LGUI(KC_1),   LGUI(KC_2),   LGUI(KC_3),      ALT_TAB,    DESK_NEXT,  LCTL(KC_F18),    KC_F17,    KC_F18,   KC_F19,    KC_F20,   LCTL(KC_F21),               _______,
        _______,        _______,      _______,      _______,      _______,      _______,  LCTL(KC_F19),    KC_F13,    KC_F14,   KC_F15,    KC_F16,                               _______,               _______,
        _______,        _______,      _______,                    _______,      _______,                  _______,             _______,   _______,        _______,               _______,     _______,  _______,  _______
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(BL_DOWN, BL_UP), ENCODER_CCW_CW(C(MS_WHLD), C(MS_WHLU))},
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(BL_DOWN, BL_UP), ENCODER_CCW_CW(C(MS_WHLD), C(MS_WHLU))},
    [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(BL_DOWN, BL_UP), ENCODER_CCW_CW(C(MS_WHLD), C(MS_WHLU))}
};
#endif

// Process key presses with custom Tap-Hold resolution
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        add_to_queue(keycode, record->event.time);
    }

    switch (keycode) {
        
        case KC_L:
        if (record->event.pressed) {
            if (record->tap.count && !record->tap.interrupted) {
                // If it's a tap (quick press and release), send 'L'
                tap_code(KC_L);
                return false;  // Stops further processing
            } else {
                // Otherwise, activate Layer 2
                layer_on(2);
                return false;  // Stops further processing
            }
        }

        case ALT_TAB:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            return false;

        case DESK_NEXT:
            if (record->event.pressed) {
                tap_code16(LCTL(LGUI(KC_RGHT))); // Switch to next virtual desktop
            }
            return false;

        case DESK_PREV:
            if (record->event.pressed) {
                tap_code16(LCTL(LGUI(KC_LEFT))); // Switch to previous virtual desktop
            }
            return false;
    }

    return true;
}

// Reset queue when changing layers
layer_state_t layer_state_set_user(layer_state_t state) {
    reset_queue();
    if (is_alt_tab_active) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }
    return state;
}
