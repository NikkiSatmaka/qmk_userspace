// Copyright 2022-2024 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file achordion.c
 * @brief Achordion implementation
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/achordion>
 */

#include "achordion.h"

#if !defined(IS_QK_MOD_TAP)
// Attempt to detect out-of-date QMK installation, which would fail with
// implicit-function-declaration errors in the code below.
#error "achordion: QMK version is too old to build. Please update QMK."
#else

// Copy of the `record` and `keycode` args for the current active tap-hold key.
static keyrecord_t tap_hold_record;
static uint16_t tap_hold_keycode = KC_NO;
// Timeout timer. When it expires, the key is considered held.
static uint16_t hold_timer = 0;
// Eagerly applied mods, if any.
static uint8_t eager_mods = 0;
// Flag to determine whether another key is pressed within the timeout.
static bool pressed_another_key_before_release = false;

#ifdef ACHORDION_STREAK
// Timer for typing streak
static uint16_t streak_timer = 0;
#else
// When disabled, is_streak is never true
#define is_streak false
#endif

// Achordion's current state.
enum {
  // A tap-hold key is pressed, but hasn't yet been settled as tapped or held.
  STATE_UNSETTLED,
  // Achordion is inactive.
  STATE_RELEASED,
  // Active tap-hold key has been settled as tapped.
  STATE_TAPPING,
  // Active tap-hold key has been settled as held.
  STATE_HOLDING,
  // This state is set while calling `process_record()`, which will recursively
  // call `process_achordion()`. This state is checked so that we don't process
  // events generated by Achordion and potentially create an infinite loop.
  STATE_RECURSING,
};
static uint8_t achordion_state = STATE_RELEASED;

#ifdef ACHORDION_STREAK
static void update_streak_timer(uint16_t keycode, keyrecord_t* record) {
  if (achordion_streak_continue(keycode)) {
    // We use 0 to represent an unset timer, so `| 1` to force a nonzero value.
    streak_timer = record->event.time | 1;
  } else {
    streak_timer = 0;
  }
}
#endif

// Presses or releases eager_mods through process_action(), which skips the
// usual event handling pipeline. The action is considered as a mod-tap hold or
// release, with Retro Tapping if enabled.
static void process_eager_mods_action(void) {
  action_t action;
  action.code = ACTION_MODS_TAP_KEY(
      eager_mods, QK_MOD_TAP_GET_TAP_KEYCODE(tap_hold_keycode));
  process_action(&tap_hold_record, action);
}

// Calls `process_record()` with state set to RECURSING.
static void recursively_process_record(keyrecord_t* record, uint8_t state) {
  achordion_state = STATE_RECURSING;
#if defined(POINTING_DEVICE_ENABLE) && defined(POINTING_DEVICE_AUTO_MOUSE_ENABLE)
  int8_t mouse_key_tracker = get_auto_mouse_key_tracker();
#endif
  process_record(record);
#if defined(POINTING_DEVICE_ENABLE) && defined(POINTING_DEVICE_AUTO_MOUSE_ENABLE)
  set_auto_mouse_key_tracker(mouse_key_tracker);
#endif
  achordion_state = state;
}

// Sends hold press event and settles the active tap-hold key as held.
static void settle_as_hold(void) {
  if (eager_mods) {
    // If eager mods are being applied, nothing needs to be done besides
    // updating the state.
    dprintln("Achordion: Settled eager mod as hold.");
    achordion_state = STATE_HOLDING;
  } else {
    // Create hold press event.
    dprintln("Achordion: Plumbing hold press.");
    recursively_process_record(&tap_hold_record, STATE_HOLDING);
  }
}

// Sends tap press and release and settles the active tap-hold key as tapped.
static void settle_as_tap(void) {
  if (eager_mods) {  // Clear eager mods if set.
#if defined(RETRO_TAPPING) || defined(RETRO_TAPPING_PER_KEY)
#ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
    neutralize_flashing_modifiers(get_mods());
#endif  // DUMMY_MOD_NEUTRALIZER_KEYCODE
#endif  // defined(RETRO_TAPPING) || defined(RETRO_TAPPING_PER_KEY)
    tap_hold_record.event.pressed = false;
    // To avoid falsely triggering Retro Tapping, process eager mods release as
    // a regular mods release rather than a mod-tap release.
    action_t action;
    action.code = ACTION_MODS(eager_mods);
    process_action(&tap_hold_record, action);
    eager_mods = 0;
  }

  dprintln("Achordion: Plumbing tap press.");
  tap_hold_record.event.pressed = true;
  tap_hold_record.tap.count = 1;  // Revise event as a tap.
  tap_hold_record.tap.interrupted = true;
  // Plumb tap press event.
  recursively_process_record(&tap_hold_record, STATE_TAPPING);

  send_keyboard_report();
#if TAP_CODE_DELAY > 0
  wait_ms(TAP_CODE_DELAY);
#endif  // TAP_CODE_DELAY > 0

  dprintln("Achordion: Plumbing tap release.");
  tap_hold_record.event.pressed = false;
  // Plumb tap release event.
  recursively_process_record(&tap_hold_record, STATE_TAPPING);
}

bool process_achordion(uint16_t keycode, keyrecord_t* record) {
  // Don't process events that Achordion generated.
  if (achordion_state == STATE_RECURSING) {
    return true;
  }

  // Determine whether the current event is for a mod-tap or layer-tap key.
  const bool is_mt = IS_QK_MOD_TAP(keycode);
  const bool is_tap_hold = is_mt || IS_QK_LAYER_TAP(keycode);
  // Check that this is a normal key event, don't act on combos.
  const bool is_key_event = IS_KEYEVENT(record->event);

  // Event while no tap-hold key is active.
  if (achordion_state == STATE_RELEASED) {
    if (is_tap_hold && record->tap.count == 0 && record->event.pressed &&
        is_key_event) {
      // A tap-hold key is pressed and considered by QMK as "held".
      const uint16_t timeout = achordion_timeout(keycode);
      if (timeout > 0) {
        achordion_state = STATE_UNSETTLED;
        // Save info about this key.
        tap_hold_keycode = keycode;
        tap_hold_record = *record;
        hold_timer = record->event.time + timeout;
        pressed_another_key_before_release = false;
        eager_mods = 0;

        if (is_mt) {  // Apply mods immediately if they are "eager."
          const uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(keycode));
          if (achordion_eager_mod(mod)) {
            eager_mods = mod;
            process_eager_mods_action();
          }
        }

        dprintf("Achordion: Key 0x%04X pressed.%s\n", keycode,
                eager_mods ? " Set eager mods." : "");
        return false;  // Skip default handling.
      }
    }

#ifdef ACHORDION_STREAK
    update_streak_timer(keycode, record);
#endif
    return true;  // Otherwise, continue with default handling.
  } else if (record->event.pressed && tap_hold_keycode != keycode) {
    // Track whether another key was pressed while using a tap-hold key.
    pressed_another_key_before_release = true;
  }

  // Release of the active tap-hold key.
  if (keycode == tap_hold_keycode && !record->event.pressed) {
    if (eager_mods) {
      dprintln("Achordion: Key released. Clearing eager mods.");
      tap_hold_record.event.pressed = false;
      process_eager_mods_action();
    } else if (achordion_state == STATE_HOLDING) {
      dprintln("Achordion: Key released. Plumbing hold release.");
      tap_hold_record.event.pressed = false;
      // Plumb hold release event.
      recursively_process_record(&tap_hold_record, STATE_RELEASED);
    } else if (!pressed_another_key_before_release) {
      // No other key was pressed between the press and release of the tap-hold
      // key, plumb a hold press and then a release.
      dprintln("Achordion: Key released. Plumbing hold press and release.");
      recursively_process_record(&tap_hold_record, STATE_HOLDING);
      tap_hold_record.event.pressed = false;
      recursively_process_record(&tap_hold_record, STATE_RELEASED);
    } else {
      dprintln("Achordion: Key released.");
    }

    achordion_state = STATE_RELEASED;
    tap_hold_keycode = KC_NO;
    return false;
  }

  if (achordion_state == STATE_UNSETTLED && record->event.pressed) {
#ifdef ACHORDION_STREAK
    const uint16_t s_timeout =
        achordion_streak_chord_timeout(tap_hold_keycode, keycode);
    const bool is_streak =
        streak_timer && s_timeout &&
        !timer_expired(record->event.time, (streak_timer + s_timeout));
#endif

    // Press event occurred on a key other than the active tap-hold key.

    // If the other key is *also* a tap-hold key and considered by QMK to be
    // held, then we settle the active key as held. This way, things like
    // chording multiple home row modifiers will work, but let's our logic
    // consider simply a single tap-hold key as "active" at a time.
    //
    // Otherwise, we call `achordion_chord()` to determine whether to settle the
    // tap-hold key as tapped vs. held. We implement the tap or hold by plumbing
    // events back into the handling pipeline so that QMK features and other
    // user code can see them. This is done by calling `process_record()`, which
    // in turn calls most handlers including `process_record_user()`.
    if (!is_streak &&
        (!is_key_event || (is_tap_hold && record->tap.count == 0) ||
         achordion_chord(tap_hold_keycode, &tap_hold_record, keycode,
                         record))) {
      settle_as_hold();

#ifdef REPEAT_KEY_ENABLE
      // Edge case involving LT + Repeat Key: in a sequence of "LT down, other
      // down" where "other" is on the other layer in the same position as
      // Repeat or Alternate Repeat, the repeated keycode is set instead of the
      // the one on the switched-to layer. Here we correct that.
      if (get_repeat_key_count() != 0 && IS_QK_LAYER_TAP(tap_hold_keycode)) {
        record->keycode = KC_NO;  // Forget the repeated keycode.
        clear_weak_mods();
      }
#endif  // REPEAT_KEY_ENABLE
    } else {
      settle_as_tap();

#ifdef ACHORDION_STREAK
      update_streak_timer(keycode, record);
      if (is_streak && is_key_event && is_tap_hold && record->tap.count == 0) {
        // If we are in a streak and resolved the current tap-hold key as a tap
        // consider the next tap-hold key as active to be resolved next.
        update_streak_timer(tap_hold_keycode, &tap_hold_record);
        const uint16_t timeout = achordion_timeout(keycode);
        tap_hold_keycode = keycode;
        tap_hold_record = *record;
        hold_timer = record->event.time + timeout;
        achordion_state = STATE_UNSETTLED;
        pressed_another_key_before_release = false;
        return false;
      }
#endif
    }

    recursively_process_record(record, achordion_state);  // Re-process event.
    return false;  // Block the original event.
  }

#ifdef ACHORDION_STREAK
  // update idle timer on regular keys event
  update_streak_timer(keycode, record);
#endif
  return true;
}

void achordion_task(void) {
  if (achordion_state == STATE_UNSETTLED &&
      timer_expired(timer_read(), hold_timer)) {
    settle_as_hold();  // Timeout expired, settle the key as held.
  }

#ifdef ACHORDION_STREAK
#define MAX_STREAK_TIMEOUT 800
  if (streak_timer &&
      timer_expired(timer_read(), (streak_timer + MAX_STREAK_TIMEOUT))) {
    streak_timer = 0;  // Expired.
  }
#endif
}

// Returns true if `pos` on the left hand of the keyboard, false if right.
static bool on_left_hand(keypos_t pos) {
#ifdef SPLIT_KEYBOARD
  return pos.row < MATRIX_ROWS / 2;
#else
  return (MATRIX_COLS > MATRIX_ROWS) ? pos.col < MATRIX_COLS / 2
                                     : pos.row < MATRIX_ROWS / 2;
#endif
}

bool achordion_opposite_hands(const keyrecord_t* tap_hold_record,
                              const keyrecord_t* other_record) {
  return on_left_hand(tap_hold_record->event.key) !=
         on_left_hand(other_record->event.key);
}

// By default, use the BILATERAL_COMBINATIONS rule to consider the tap-hold key
// "held" only when it and the other key are on opposite hands.
__attribute__((weak)) bool achordion_chord(uint16_t tap_hold_keycode,
                                           keyrecord_t* tap_hold_record,
                                           uint16_t other_keycode,
                                           keyrecord_t* other_record) {
  return achordion_opposite_hands(tap_hold_record, other_record);
}

// By default, the timeout is 1000 ms for all keys.
__attribute__((weak)) uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  return 1000;
}

// By default, Shift and Ctrl mods are eager, and Alt and GUI are not.
__attribute__((weak)) bool achordion_eager_mod(uint8_t mod) {
  return (mod & (MOD_LALT | MOD_LGUI)) == 0;
}

#ifdef ACHORDION_STREAK
__attribute__((weak)) bool achordion_streak_continue(uint16_t keycode) {
  // If any mods other than shift or AltGr are held, don't continue the streak
  if (get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) return false;
  // This function doesn't get called for holds, so convert to tap version of
  // keycodes
  if (IS_QK_MOD_TAP(keycode)) keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
  if (IS_QK_LAYER_TAP(keycode)) keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
  // Regular letters and punctuation continue the streak.
  if (keycode >= KC_A && keycode <= KC_Z) return true;
  switch (keycode) {
    case KC_DOT:
    case KC_COMMA:
    case KC_QUOTE:
    case KC_SPACE:
      return true;
  }
  // All other keys end the streak
  return false;
}

__attribute__((weak)) uint16_t achordion_streak_chord_timeout(
    uint16_t tap_hold_keycode, uint16_t next_keycode) {
  return achordion_streak_timeout(tap_hold_keycode);
}

__attribute__((weak)) uint16_t
achordion_streak_timeout(uint16_t tap_hold_keycode) {
  return 200;
}
#endif

#endif  // version check
