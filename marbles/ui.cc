// Copyright 2015 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// User interface.

#include "marbles/ui.h"

#include <algorithm>

#include "stmlib/system/system_clock.h"

#include "marbles/drivers/clock_inputs.h"
#include "marbles/cv_reader.h"
#include "marbles/scale_recorder.h"

namespace marbles {

const int32_t kLongPressDuration = 2000;
const int32_t kMiddlePressDuration = 1000;
const int32_t kSaveLoadConfirmedLedTickCount = 1000;

using namespace std;
using namespace stmlib;

/* static */
const LedColor Ui::palette_[4] = {
  LED_COLOR_GREEN,
  LED_COLOR_YELLOW,
  LED_COLOR_RED,
  LED_COLOR_OFF
};

/* static */
AlternateKnobMapping Ui::alternate_knob_mappings_[ADC_CHANNEL_LAST];
AdditionalAlternateKnobMapping Ui::additional_alternate_knob_mappings_[ADDITIONAL_ALTERNATE_KNOB_LAST];

void Ui::Init(
    Settings* settings,
    CvReader* cv_reader,
    ScaleRecorder* scale_recorder,
    ClockInputs* clock_inputs) {
  settings_ = settings;
  cv_reader_ = cv_reader;
  scale_recorder_ = scale_recorder;
  clock_inputs_ = clock_inputs;

  save_slot_index_ = -1;
  load_slot_index_ = -1;

  alternative_led_blink_ = 0;
  alternative_led_count_ = 0;
  alternative_bootup_count_ = 0;
  mode_ = UI_MODE_ALTERNATIVE_BOOTUP;
  
  leds_.Init();
  switches_.Init();
  queue_.Init();
  
  // Initialize generator from settings_->state();
  fill(&pot_value_[0], &pot_value_[ADC_CHANNEL_LAST], 0.0f);
  
  State* state = settings_->mutable_state();
  alternate_knob_mappings_[ADC_CHANNEL_T_BIAS].unlock_switch = SWITCH_T_MODEL;
  alternate_knob_mappings_[ADC_CHANNEL_T_BIAS].destination = &state->t_pulse_width_mean;
  alternate_knob_mappings_[ADC_CHANNEL_T_JITTER].unlock_switch = SWITCH_T_MODEL;
  alternate_knob_mappings_[ADC_CHANNEL_T_JITTER].destination = &state->t_pulse_width_std;
  alternate_knob_mappings_[ADC_CHANNEL_T_RATE].unlock_switch = SWITCH_X_MODE;
  alternate_knob_mappings_[ADC_CHANNEL_T_RATE].destination = &state->y_divider;
  alternate_knob_mappings_[ADC_CHANNEL_X_SPREAD].unlock_switch = SWITCH_X_MODE;
  alternate_knob_mappings_[ADC_CHANNEL_X_SPREAD].destination = &state->y_spread;
  alternate_knob_mappings_[ADC_CHANNEL_X_BIAS].unlock_switch = SWITCH_X_MODE;
  alternate_knob_mappings_[ADC_CHANNEL_X_BIAS].destination = &state->y_bias;
  alternate_knob_mappings_[ADC_CHANNEL_X_STEPS].unlock_switch = SWITCH_X_MODE;
  alternate_knob_mappings_[ADC_CHANNEL_X_STEPS].destination = &state->y_steps;

  additional_alternate_knob_mappings_[ADDITIONAL_ALTERNATE_KNOB_LOOP_START].adc_parameter = ADC_CHANNEL_DEJA_VU_LENGTH;
  additional_alternate_knob_mappings_[ADDITIONAL_ALTERNATE_KNOB_LOOP_START].unlock_switch = SWITCH_T_RANGE;
  additional_alternate_knob_mappings_[ADDITIONAL_ALTERNATE_KNOB_LOOP_START].destination = &state->loop_start;

  additional_alternate_knob_mapping_mode_ = false;

  setting_modification_flag_ = false;
  output_test_mode_ = false;
  
  if (switches_.pressed_immediate(SWITCH_X_MODE)) {
    if (state->color_blind == 1) {
      state->color_blind = 0;
    } else {
      state->color_blind = 1; 
    }
    settings_->SaveState();
  }
  
  if (switches_.pressed_immediate(SWITCH_X_DEJA_VU)) {
    settings_->ProgramOptionBytes();
  }
  
  deja_vu_lock_ = false;
}

void Ui::SaveState() {
  settings_->SaveState();
}

void Ui::Poll() {
  // 1kHz.
  system_clock.Tick();
  switches_.Debounce();
  
  for (int i = 0; i < SWITCH_LAST; ++i) {
    if (switches_.just_pressed(Switch(i))) {
      queue_.AddEvent(CONTROL_SWITCH, i, 0);
      press_time_[i] = system_clock.milliseconds();
      ignore_release_[i] = false;
    }
    if (switches_.pressed(Switch(i)) && !ignore_release_[i]) {
      int32_t pressed_time = system_clock.milliseconds() - press_time_[i];
      if (pressed_time > kLongPressDuration && !setting_modification_flag_) {
        queue_.AddEvent(CONTROL_SWITCH, i, pressed_time);
        ignore_release_[i] = true;
      }
    }
    if (switches_.released(Switch(i)) && !ignore_release_[i]) {
      queue_.AddEvent(
          CONTROL_SWITCH,
          i,
          system_clock.milliseconds() - press_time_[i] + 1);
      ignore_release_[i] = true;
    }
  }

  if (mode_ == UI_MODE_ALTERNATIVE_CONFIRMED) {
    saveload_confirmed_tick_count_ += 1;
    if (saveload_confirmed_tick_count_ > kSaveLoadConfirmedLedTickCount) {
      saveload_confirmed_tick_count_ = 0;
      mode_ = UI_MODE_NORMAL;
    }
  }
  
  UpdateLEDs();
}

/* static */
LedColor Ui::MakeColor(uint8_t value, bool color_blind) {
  bool slow_blink = (system_clock.milliseconds() & 255) > 128;

  uint8_t bank = value >= 3 ? 1 : 0;
  value -= bank * 3;
  
  LedColor color = palette_[value];
  if (color_blind) {
    uint8_t pwm_counter = system_clock.milliseconds() & 15;
    uint8_t triangle = (system_clock.milliseconds() >> 5) & 31;
    triangle = triangle < 16 ? triangle : 31 - triangle;
  
    if (value == 0) {
      color = pwm_counter < (4 + (triangle >> 2))
          ? LED_COLOR_GREEN
          : LED_COLOR_OFF;
    } else if (value == 1) {
      color = LED_COLOR_YELLOW;
    } else {
      color = pwm_counter == 0 ? LED_COLOR_RED : LED_COLOR_OFF;
    }
  }

  return slow_blink || !bank ? color : LED_COLOR_OFF;
}

/* static */
LedColor Ui::MakeColorForMarkov(uint8_t value, bool color_blind) {
  bool blink = (system_clock.milliseconds() & 127) > 64; // normal was slow_blink

  uint8_t bank = value >= 3 ? 1 : 0;
  value = 2; // normal was -= bank * 3;
  
  LedColor color = palette_[value];
  if (color_blind) {
    uint8_t pwm_counter = system_clock.milliseconds() & 15;
    uint8_t triangle = (system_clock.milliseconds() >> 5) & 31;
    triangle = triangle < 16 ? triangle : 31 - triangle;
  
    if (value == 0) {
      color = pwm_counter < (4 + (triangle >> 2))
          ? LED_COLOR_GREEN
          : LED_COLOR_OFF;
    } else if (value == 1) {
      color = LED_COLOR_YELLOW;
    } else {
      color = pwm_counter == 0 ? LED_COLOR_RED : LED_COLOR_OFF;
    }
  }

  return blink || !bank ? color : LED_COLOR_OFF;
}

/* static */
LedColor Ui::DejaVuColor(DejaVuState state, bool lock) {
  if (state == DEJA_VU_OFF) {
    return LED_COLOR_OFF;
  } else if (state == DEJA_VU_ON) {
    if (lock) {
      int slow_triangle = (system_clock.milliseconds() & 1023) >> 5;
      slow_triangle = slow_triangle >= 16 ? 31 - slow_triangle : slow_triangle;
      int pw = system_clock.milliseconds() & 15;
      return slow_triangle >= pw ? LED_COLOR_GREEN : LED_COLOR_OFF;
    } else {
      return LED_COLOR_GREEN;
    }
  } else {
    int fast_triangle = (system_clock.milliseconds() & 511) >> 4;
    fast_triangle = fast_triangle >= 16 ? 31 - fast_triangle : fast_triangle;
    int pw = system_clock.milliseconds() & 15;

    return fast_triangle >= pw ? LED_COLOR_GREEN : LED_COLOR_OFF;
  }
}

void Ui::UpdateLEDs() {
  bool blink = (system_clock.milliseconds() & 127) > 64;
  bool slow_blink = (system_clock.milliseconds() & 255) > 128;
  bool fast_blink = (system_clock.milliseconds() & 63) > 32;
  const State& state = settings_->state();
  bool cb = state.color_blind == 1;
  
  LedColor scale_color = state.x_scale < 3
      ? (slow_blink ? palette_[state.x_scale] : LED_COLOR_OFF)
      : (fast_blink ? palette_[state.x_scale - 3] : LED_COLOR_OFF);
  
  if (cb) {
    int poly_counter = (system_clock.milliseconds() >> 6) % 12;
    if ((poly_counter >> 1) < (state.x_scale + 1) && (poly_counter & 1)) {
      scale_color = LED_COLOR_YELLOW;
    } else {
      scale_color = LED_COLOR_OFF;
    }
  }
  
  leds_.Clear();
  
  switch (mode_) {
    case UI_MODE_NORMAL:
    case UI_MODE_RECORD_SCALE:
    case UI_MODE_ALTERNATIVE_CONFIRMED:
      {
        if (state.t_model == 6) { // markov
          leds_.set(LED_T_MODEL, MakeColorForMarkov(state.t_model, cb));
        } else {
        leds_.set(LED_T_MODEL, MakeColor(state.t_model, cb));
        }
        if (mode_ == UI_MODE_ALTERNATIVE_CONFIRMED) {
          leds_.set(LED_T_RANGE, fast_blink ? MakeColor(state.t_range, cb) : LED_COLOR_OFF);
        } else {
          leds_.set(LED_T_RANGE, MakeColor(state.t_range, cb));
        }
        leds_.set(
            LED_T_DEJA_VU,
            DejaVuColor(DejaVuState(state.t_deja_vu), deja_vu_lock_));
        
        leds_.set(LED_X_CONTROL_MODE, MakeColor(state.x_control_mode, cb));
        leds_.set(
            LED_X_DEJA_VU,
            DejaVuColor(DejaVuState(state.x_deja_vu), deja_vu_lock_));
        
        if (mode_ == UI_MODE_NORMAL || mode_ == UI_MODE_ALTERNATIVE_CONFIRMED) {
          leds_.set(LED_X_RANGE,
                    state.x_register_mode
                        ? LED_COLOR_OFF
                        : MakeColor(state.x_range, cb));
          leds_.set(LED_X_EXT,
                    state.x_register_mode ? LED_COLOR_GREEN : LED_COLOR_OFF);
        } else {
          leds_.set(LED_X_RANGE, scale_color);
          leds_.set(LED_X_EXT, LED_COLOR_GREEN);
        }
      }
      break;

    case UI_MODE_SELECT_SCALE:
      leds_.set(LED_X_RANGE, scale_color);
      break;
    
    case UI_MODE_CALIBRATION_1:
      leds_.set(LED_T_RANGE, blink ? MakeColor(0, cb) : LED_COLOR_OFF);
      break;

    case UI_MODE_CALIBRATION_2:
      leds_.set(LED_T_RANGE, blink ? MakeColor(1, cb) : LED_COLOR_OFF);
      break;

    case UI_MODE_CALIBRATION_3:
      leds_.set(LED_X_RANGE, blink ? MakeColor(0, cb) : LED_COLOR_OFF);
      break;

    case UI_MODE_CALIBRATION_4:
      leds_.set(LED_X_RANGE, blink ? MakeColor(1, cb) : LED_COLOR_OFF);
      break;
    
    case UI_MODE_PANIC:
      leds_.set(LED_T_MODEL, blink ? LED_COLOR_RED : LED_COLOR_OFF);
      leds_.set(LED_T_RANGE, !blink ? LED_COLOR_RED : LED_COLOR_OFF);
      leds_.set(LED_X_CONTROL_MODE, !blink ? LED_COLOR_RED : LED_COLOR_OFF);
      leds_.set(LED_X_RANGE, blink ? LED_COLOR_RED : LED_COLOR_OFF);
      break;

    case UI_MODE_ALTERNATIVE_BOOTUP:
      if (alternative_bootup_count_ == 0) { // start bootup
        alternative_bootup_count_ = 1;
      }

      if (alternative_led_blink_ != slow_blink) {
        if (alternative_bootup_count_ >= 3) { // end bootup
          mode_ = UI_MODE_NORMAL;
          break;
        }

        alternative_led_blink_ = slow_blink;
        if (alternative_led_count_ >= 2) {
          alternative_led_count_ = 0;
          alternative_bootup_count_ += 1;
        } else {
          alternative_led_count_ += 1;
        }
      }

      switch (alternative_led_count_ % 3) {
        case 0:
          leds_.set(LED_T_DEJA_VU, LED_COLOR_GREEN);
          leds_.set(LED_X_DEJA_VU, LED_COLOR_OFF);
          leds_.set(LED_X_EXT, LED_COLOR_OFF);
          break;
        case 1:
          leds_.set(LED_T_DEJA_VU, LED_COLOR_OFF);
          leds_.set(LED_X_DEJA_VU, LED_COLOR_GREEN);
          leds_.set(LED_X_EXT, LED_COLOR_OFF);
          break;
        case 2:
          leds_.set(LED_T_DEJA_VU, LED_COLOR_OFF);
          leds_.set(LED_X_DEJA_VU, LED_COLOR_OFF);
          leds_.set(LED_X_EXT, LED_COLOR_GREEN);
          break;
        default:
          break;
      }
      break;

    case UI_MODE_ALTERNATIVE:
      if (alternative_led_blink_ != slow_blink) {
        alternative_led_blink_ = slow_blink;
        if (alternative_led_count_ >= 2) {
          alternative_led_count_ = 0;
        } else {
          alternative_led_count_ += 1;
        }
      }

      switch (alternative_led_count_ % 3) {
        case 0:
          leds_.set(LED_T_DEJA_VU, LED_COLOR_GREEN);
          leds_.set(LED_X_DEJA_VU, LED_COLOR_OFF);
          leds_.set(LED_X_EXT, LED_COLOR_OFF);
          break;
        case 1:
          leds_.set(LED_T_DEJA_VU, LED_COLOR_OFF);
          leds_.set(LED_X_DEJA_VU, LED_COLOR_GREEN);
          leds_.set(LED_X_EXT, LED_COLOR_OFF);
          break;
        case 2:
          leds_.set(LED_T_DEJA_VU, LED_COLOR_OFF);
          leds_.set(LED_X_DEJA_VU, LED_COLOR_OFF);
          leds_.set(LED_X_EXT, LED_COLOR_GREEN);
          break;
        default:
          break;
      }

      switch (alternative_settings_mode_) {
        case ALTERNATIVE_SETTINGS_MODE_RESET_IN:
          leds_.set(LED_T_MODEL, LED_COLOR_GREEN);
          leds_.set(LED_X_CONTROL_MODE, LED_COLOR_GREEN);
          UpdateLEDsAlternativeValues(state.x_clock_mode & 0x0F);
          break;
        case ALTERNATIVE_SETTINGS_MODE_LOOP:
          leds_.set(LED_T_MODEL, LED_COLOR_GREEN);
          leds_.set(LED_X_CONTROL_MODE, LED_COLOR_YELLOW);
          UpdateLEDsAlternativeValues(state.loop_cv_mode);
          break;
        case ALTERNATIVE_SETTINGS_MODE_QUANTIZER_SCALE:
          leds_.set(LED_T_MODEL, LED_COLOR_GREEN);
          leds_.set(LED_X_CONTROL_MODE, LED_COLOR_RED);
          UpdateLEDsAlternativeValues(state.quantizer_cv_mode);
          break;
        case ALTERNATIVE_SETTINGS_MODE_QUANTIZER_ROOT:
          leds_.set(LED_T_MODEL, LED_COLOR_YELLOW);
          leds_.set(LED_X_CONTROL_MODE, LED_COLOR_GREEN);
          UpdateLEDsAlternativeValues(state.root_cv_mode);
          break;
        case ALTERNATIVE_SETTINGS_MODE_X_CLOCK_ROUTING:
          leds_.set(LED_T_MODEL, LED_COLOR_YELLOW);
          leds_.set(LED_X_CONTROL_MODE, LED_COLOR_YELLOW);
          UpdateLEDsAlternativeValues((state.x_clock_mode & 0xF0) >> 4);
          break;
        case ALTERNATIVE_SETTINGS_MODE_DEJA_VU_BUFFER_LENGTH:
          leds_.set(LED_T_MODEL, LED_COLOR_YELLOW);
          leds_.set(LED_X_CONTROL_MODE, LED_COLOR_RED);
          UpdateLEDsAlternativeValues(state.deja_vu_buffer_length_mode);
          break;
        case ALTERNATIVE_SETTINGS_MODE_CHORD:
          leds_.set(LED_T_MODEL, LED_COLOR_RED);
          leds_.set(LED_X_CONTROL_MODE, LED_COLOR_GREEN);
          UpdateLEDsAlternativeValues(state.chord_mode);
          break;
        default:
          break;
      }
      break;
  }
  leds_.Write();
}

inline void Ui::UpdateLEDsAlternativeValues(int value) {
  switch (value) {
    case 0:
      leds_.set(LED_T_RANGE, LED_COLOR_GREEN);
      leds_.set(LED_X_RANGE, LED_COLOR_GREEN);
      break;
    case 1:
      leds_.set(LED_T_RANGE, LED_COLOR_GREEN);
      leds_.set(LED_X_RANGE, LED_COLOR_YELLOW);
      break;
    case 2:
      leds_.set(LED_T_RANGE, LED_COLOR_GREEN);
      leds_.set(LED_X_RANGE, LED_COLOR_RED);
      break;
    case 3:
      leds_.set(LED_T_RANGE, LED_COLOR_YELLOW);
      leds_.set(LED_X_RANGE, LED_COLOR_GREEN);
      break;
    case 4:
      leds_.set(LED_T_RANGE, LED_COLOR_YELLOW);
      leds_.set(LED_X_RANGE, LED_COLOR_YELLOW);
      break;
    case 5:
      leds_.set(LED_T_RANGE, LED_COLOR_YELLOW);
      leds_.set(LED_X_RANGE, LED_COLOR_RED);
      break;
    case 6:
      leds_.set(LED_T_RANGE, LED_COLOR_RED);
      leds_.set(LED_X_RANGE, LED_COLOR_GREEN);
      break;
    case 7:
      leds_.set(LED_T_RANGE, LED_COLOR_RED);
      leds_.set(LED_X_RANGE, LED_COLOR_YELLOW);
      break;
    case 8:
      leds_.set(LED_T_RANGE, LED_COLOR_RED);
      leds_.set(LED_X_RANGE, LED_COLOR_RED);
      break;
    default:
      break;
  }
}

void Ui::FlushEvents() {
  queue_.Flush();
}

void Ui::OnSwitchPressed(const Event& e) {

}

void Ui::OnSwitchReleased(const Event& e) {
  if (setting_modification_flag_) {
    for (int i = 0; i < ADC_CHANNEL_LAST; ++i) {
      cv_reader_->mutable_channel(i)->UnlockPot();
    }
    setting_modification_flag_ = false;
    return;
  }
  
  // Check if the other switch is still pressed.
  if (e.control_id == SWITCH_T_RANGE && switches_.pressed(SWITCH_X_RANGE)) {
    mode_ = UI_MODE_CALIBRATION_1;
    ignore_release_[SWITCH_T_RANGE] = ignore_release_[SWITCH_X_RANGE] = true;
    return;
  }

  State* state = settings_->mutable_state();

  if (mode_ == UI_MODE_ALTERNATIVE) {
    if (e.control_id == SWITCH_T_DEJA_VU) {
      // save slot A
      save_slot_index_ = 0;
      load_slot_index_ = -1;
      saveload_confirmed_tick_count_ = 0;
      ignore_release_[SWITCH_T_DEJA_VU] = true;
      mode_ = UI_MODE_ALTERNATIVE_CONFIRMED;
      ExitAdditionalAlternateKnobMapping();
      return;
    }
    if (e.control_id == SWITCH_X_DEJA_VU) {
      // load slot A
      load_slot_index_ = 0;
      save_slot_index_ = -1;
      saveload_confirmed_tick_count_ = 0;
      ignore_release_[SWITCH_X_DEJA_VU] = true;
      mode_ = UI_MODE_ALTERNATIVE_CONFIRMED;
      ExitAdditionalAlternateKnobMapping();
      return;
    }
    if (e.control_id == SWITCH_T_RANGE) {
      // change alternative settings mode
      alternative_settings_mode_ += 1;
      if (alternative_settings_mode_ >= ALTERNATIVE_SETTINGS_MODE_COUNT) {
        alternative_settings_mode_ = 0;
      }
      ignore_release_[SWITCH_T_RANGE] = true;
      return;
    }
    if (e.control_id == SWITCH_X_EXT) {
      // exit alternative settings mode
      save_slot_index_ = -1;
      load_slot_index_ = -1;
      saveload_confirmed_tick_count_ = 0;
      ignore_release_[SWITCH_X_EXT] = true;
      if (e.data >= kLongPressDuration) {
        // all reset of alternate function settings
        state->loop_start = 0;
        state->x_clock_mode = 0; // normal
        state->quantizer_cv_mode = 0; // normal t jitter cv
        state->root_cv_mode = 0; // normal x range cv
        state->loop_cv_mode = 0; // normal t rate cv
        state->deja_vu_buffer_length_mode = 0;
        state->chord_mode = 0;
      }
      mode_ = UI_MODE_ALTERNATIVE_CONFIRMED;
      ExitAdditionalAlternateKnobMapping();
      SaveState();
      return;
    }
    if (e.control_id == SWITCH_X_RANGE) {
      const uint8_t x_clock_mode = state->x_clock_mode & 0x0F;
      const uint8_t x_clock_routing_mode = (state->x_clock_mode & 0xF0) >> 4;
      // change alternative settings value
      switch (alternative_settings_mode_) {
        case ALTERNATIVE_SETTINGS_MODE_RESET_IN:
          if (x_clock_mode == 0) {
            state->x_clock_mode = (x_clock_routing_mode << 4) | 1; // t reset
          } else if (x_clock_mode == 1) {
            state->x_clock_mode = (x_clock_routing_mode << 4) | 2; // t hold
          } else {
            state->x_clock_mode = (x_clock_routing_mode << 4) | 0;
          }
          break;
        case ALTERNATIVE_SETTINGS_MODE_LOOP:
          if (state->loop_cv_mode == 0) {
            state->loop_cv_mode = 1; // loop length cv
          } else if (state->loop_cv_mode == 1) {
            state->loop_cv_mode = 2; // loop start position cv
          } else if (state->loop_cv_mode == 2) {
            state->loop_cv_mode = 3; // loop start position cv with fixed end position 16
          } else if (state->loop_cv_mode == 3) {
            state->loop_cv_mode = 4; // normal t rate cv, loop start position knob with fixed end position 16
          } else {
            state->loop_cv_mode = 0;
          }
          break;
        case ALTERNATIVE_SETTINGS_MODE_QUANTIZER_SCALE:
          if (state->quantizer_cv_mode == 0) {
            state->quantizer_cv_mode = 1; // x quantizer scale select cv
          } else {
            state->quantizer_cv_mode = 0;
          }
          break;
        case ALTERNATIVE_SETTINGS_MODE_QUANTIZER_ROOT:
          if (state->root_cv_mode == 0) {
            state->root_cv_mode = 1; // x quantizer adding root offset 1v/oct cv
          } else if (state->root_cv_mode == 1) {
            state->root_cv_mode = 2; // x quantizer reflecting root 1v/oct cv
          } else {
            state->root_cv_mode = 0;
          }
          break;
        case ALTERNATIVE_SETTINGS_MODE_X_CLOCK_ROUTING:
          if (x_clock_routing_mode == 0) {
            state->x_clock_mode = (1 << 4) | x_clock_mode; // T1
          } else if (x_clock_routing_mode == 1) {
            state->x_clock_mode = (2 << 4) | x_clock_mode; // T2
          } else if (x_clock_routing_mode == 2) {
            state->x_clock_mode = (3 << 4) | x_clock_mode; // T3
          } else {
            state->x_clock_mode = 0 | x_clock_mode; // T1_T2_T3
          }
          break;
        case ALTERNATIVE_SETTINGS_MODE_DEJA_VU_BUFFER_LENGTH:
          if (state->deja_vu_buffer_length_mode == 0) {
            state->deja_vu_buffer_length_mode = 1; // 128
          } else if (state->deja_vu_buffer_length_mode == 1) {
            state->deja_vu_buffer_length_mode = 2; // 128 base 2
          } else if (state->deja_vu_buffer_length_mode == 2) {
            state->deja_vu_buffer_length_mode = 3; // 192 base 3
          } else if (state->deja_vu_buffer_length_mode == 3) {
            state->deja_vu_buffer_length_mode = 4; // 320 base 5
          } else if (state->deja_vu_buffer_length_mode == 4) {
            state->deja_vu_buffer_length_mode = 5; // 233 fibonacci
          } else {
            state->deja_vu_buffer_length_mode = 0;
          }
          break;
        case ALTERNATIVE_SETTINGS_MODE_CHORD:
          if (state->chord_mode == 0) {
            state->chord_mode = 1; // chord
          } else if (state->chord_mode == 1) {
            state->chord_mode = 2; // chord
          } else if (state->chord_mode == 2) {
            state->chord_mode = 3; // chord
          } else if (state->chord_mode == 3) {
            state->chord_mode = 4; // chord
          } else if (state->chord_mode == 4) {
            state->chord_mode = 5; // chord
          } else {
            state->chord_mode = 0;
          }
          break;
        default:
          break;
      }

      ignore_release_[SWITCH_X_RANGE] = true;
      return;
    }
    if (e.control_id == SWITCH_X_MODE) {
      ignore_release_[SWITCH_X_MODE] = true;
      return;
    }
    if (e.control_id == SWITCH_T_MODEL) {
      ignore_release_[SWITCH_T_MODEL] = true;
      return;
    }
  }
  
  switch (e.control_id) {
    case SWITCH_T_DEJA_VU:
      if (state->t_deja_vu == DEJA_VU_OFF) {
        state->t_deja_vu = e.data >= kLongPressDuration
            ? DEJA_VU_LOCKED
            : DEJA_VU_ON;
      } else if (state->t_deja_vu == DEJA_VU_LOCKED) {
        state->t_deja_vu = DEJA_VU_ON;
      } else {
        state->t_deja_vu = e.data >= kLongPressDuration
            ? DEJA_VU_LOCKED
            : DEJA_VU_OFF;
      }
      break;

    case SWITCH_X_DEJA_VU:
      if (state->x_deja_vu == DEJA_VU_OFF) {
        state->x_deja_vu = e.data >= kLongPressDuration
            ? DEJA_VU_LOCKED
            : DEJA_VU_ON;
      } else if (state->x_deja_vu == DEJA_VU_LOCKED) {
        state->x_deja_vu = DEJA_VU_ON;
      } else {
        state->x_deja_vu = e.data >= kLongPressDuration
            ? DEJA_VU_LOCKED
            : DEJA_VU_OFF;
      }
      break;
    
    case SWITCH_T_MODEL:
      {
        if (state->t_model == 6) { // markov
          if (e.data >= kLongPressDuration) {
            state->t_model = 2; // it will become 5 by following judgement.
          } else {
            state->t_model = 1; // it will become 2 by following judgement.
          }
        }
        uint8_t bank = state->t_model / 3;
        if (e.data >= kLongPressDuration) {
          if (!bank) {
            state->t_model += 3;
          }
        } else {
          if (bank) {
            state->t_model -= 3;
          } else {
            state->t_model = (state->t_model + 1) % 3;
          }
        }
        SaveState();
      }
      break;

    case SWITCH_T_RANGE:
      {
        if (mode_ >= UI_MODE_CALIBRATION_1 && mode_ <= UI_MODE_CALIBRATION_4) {
          NextCalibrationStep();
          SaveState();
        } else {
          if (e.data >= kMiddlePressDuration) {
            alternative_settings_mode_ = 0;
            mode_ = UI_MODE_ALTERNATIVE;
            ignore_release_[SWITCH_T_RANGE] = true;
            EnterAdditionalAlternateKnobMapping();
          } else if (switches_.pressed(SWITCH_T_MODEL)) { // pressing T model and T range
            if (state->t_model != 6) {
              state->t_model = 6; // markov
              ignore_release_[SWITCH_T_MODEL] = true;
              SaveState();
            }
          } else {
            state->t_range = (state->t_range + 1) % 3;
            SaveState();
          }
        }
      }
      break;
    
    case SWITCH_X_MODE:
      state->x_control_mode = (state->x_control_mode + 1) % 3;
      SaveState();
      break;
      
    case SWITCH_X_EXT:
      if (mode_ == UI_MODE_RECORD_SCALE) {
        int scale_index = settings_->state().x_scale;
        bool success = true;
        if (e.data >= kLongPressDuration) {
          settings_->ResetScale(scale_index);
        } else {
          success = scale_recorder_->ExtractScale(
              settings_->mutable_scale(scale_index));
        }
        if (success) {
          settings_->SavePersistentData();
          settings_->set_dirty_scale_index(scale_index);
        }
        mode_ = UI_MODE_NORMAL;
      } else if (e.data >= kLongPressDuration) {
        mode_ = UI_MODE_RECORD_SCALE;
        scale_recorder_->Clear();
      } else {
        state->x_register_mode = !state->x_register_mode;
        SaveState();
      }
      break;

    case SWITCH_X_RANGE:
      if (mode_ >= UI_MODE_CALIBRATION_1 && mode_ <= UI_MODE_CALIBRATION_4) {
        NextCalibrationStep();
      } else if (e.data >= kLongPressDuration) {
        if (mode_ == UI_MODE_NORMAL) {
          mode_ = UI_MODE_SELECT_SCALE;
        }
      } else if (mode_ == UI_MODE_SELECT_SCALE) {
        state->x_scale = (state->x_scale + 1) % kNumScales;
      } else {
        if (!state->x_register_mode) {
          state->x_range = (state->x_range + 1) % 3;
        }
      }
      SaveState();
      break;
  }
}

void Ui::TerminateScaleRecording() {
  for (int i = 0; i < ADC_CHANNEL_LAST; ++i) {
    cv_reader_->mutable_channel(i)->UnlockPot();
  }
  mode_ = UI_MODE_NORMAL;
}

void Ui::NextCalibrationStep() {
  switch (mode_) {
    case UI_MODE_CALIBRATION_1:
      cv_reader_->CalibrateOffsets();
      cv_reader_->CalibrateRateC1();
      mode_ = UI_MODE_CALIBRATION_2;
      break;

    case UI_MODE_CALIBRATION_2:
      cv_reader_->CalibrateRateC3();
      mode_ = UI_MODE_CALIBRATION_3;
      break;

    case UI_MODE_CALIBRATION_3:
      cv_reader_->CalibrateSpreadC1();
      mode_ = UI_MODE_CALIBRATION_4;
      break;
    
    case UI_MODE_CALIBRATION_4:
      if (cv_reader_->CalibrateSpreadC3()) {
        settings_->SavePersistentData();
        mode_ = UI_MODE_NORMAL;
      } else {
        mode_ = UI_MODE_PANIC;
      }
      break;
      
    default:
      break;
  }
}

void Ui::UpdateHiddenParameters() {
  // Check if some pots have been moved.
  for (int i = 0; i < ADC_CHANNEL_LAST; ++i) {
    float new_value = cv_reader_->channel(i).unscaled_pot();
    float old_value = pot_value_[i];
    bool changed = fabs(new_value - old_value) >= 0.008f;
    if (changed) {
      pot_value_[i] = new_value;

      if (additional_alternate_knob_mapping_mode_) {
        for (int j = 0; j < ADDITIONAL_ALTERNATE_KNOB_LAST; ++j) {
          AdditionalAlternateKnobMapping additional_mapping = additional_alternate_knob_mappings_[j];
          if (additional_mapping.adc_parameter == i) {
            *additional_mapping.destination = static_cast<uint8_t>(new_value * 255.0f);
          }
        }
      }

      AlternateKnobMapping mapping = alternate_knob_mappings_[i];
      if (switches_.pressed(mapping.unlock_switch)) {
        if (mapping.unlock_switch == SWITCH_T_RANGE && new_value < 0.1f) {
          new_value = 0.0f;
        }
        *mapping.destination = static_cast<uint8_t>(new_value * 255.0f);
        cv_reader_->mutable_channel(i)->LockPot();

        // The next time a switch is released, we unlock the pots.
        setting_modification_flag_ = true;
      }
    }
  }
}

void Ui::EnterAdditionalAlternateKnobMapping() {
  // for (int j = 0; j < ADDITIONAL_ALTERNATE_KNOB_LAST; ++j) {
  //   AdditionalAlternateKnobMapping additional_mapping = additional_alternate_knob_mappings_[j];
  //   int i = additional_mapping.adc_parameter; 
  //   cv_reader_->mutable_channel(i)->LockPot();
  // }
  // additional_alternate_knob_mapping_mode_ = true;
}

void Ui::ExitAdditionalAlternateKnobMapping() {
  // for (int j = 0; j < ADDITIONAL_ALTERNATE_KNOB_LAST; ++j) {
  //   AdditionalAlternateKnobMapping additional_mapping = additional_alternate_knob_mappings_[j];
  //   int i = additional_mapping.adc_parameter; 
  //   cv_reader_->mutable_channel(i)->UnlockPot();
  // }
  // additional_alternate_knob_mapping_mode_ = false;
}

void Ui::DoEvents() {
  while (queue_.available()) {
    Event e = queue_.PullEvent();
    if (e.control_type == CONTROL_SWITCH) {
      if (e.data == 0) {
        OnSwitchPressed(e);
      } else {
        OnSwitchReleased(e);
      }
    }
  }
  
  UpdateHiddenParameters();
  
  if (queue_.idle_time() > 800 && mode_ == UI_MODE_PANIC) {
    mode_ = UI_MODE_NORMAL;
  }
  if (mode_ == UI_MODE_SELECT_SCALE) {
    if (queue_.idle_time() > 4000) {
      mode_ = UI_MODE_NORMAL;
      queue_.Touch();
    }
  } else if (queue_.idle_time() > 1000) {
    queue_.Touch();
  }
}

uint8_t Ui::HandleFactoryTestingRequest(uint8_t command) {
  uint8_t argument = command & 0x1f;
  command = command >> 5;
  uint8_t reply = 0;
  switch (command) {
    case FACTORY_TESTING_READ_POT:
    case FACTORY_TESTING_READ_CV:
      reply = cv_reader_->adc_value(argument);
      break;
    
    case FACTORY_TESTING_READ_NORMALIZATION:
      reply = clock_inputs_->is_normalized(ClockInput(argument)) ? 255 : 0;
      break;      
    
    case FACTORY_TESTING_READ_GATE:
      reply = argument >= SWITCH_LAST
          ? clock_inputs_->value(ClockInput(argument - SWITCH_LAST))
          : switches_.pressed(Switch(argument));
      break;
      
    case FACTORY_TESTING_GENERATE_TEST_SIGNALS:
      output_test_mode_ = static_cast<bool>(argument);
      fill(
          &output_test_forced_dac_code_[0],
          &output_test_forced_dac_code_[4],
          0);
      break;
      
    case FACTORY_TESTING_CALIBRATE:
      if (argument == 0) {
        // Revert all settings before getting into calibration mode.
        settings_->mutable_state()->t_deja_vu = 0;
        settings_->mutable_state()->x_deja_vu = 0;
        settings_->mutable_state()->t_model = 0;
        settings_->mutable_state()->t_range = 1;
        settings_->mutable_state()->x_control_mode = 0;
        settings_->mutable_state()->x_range = 2;
        settings_->mutable_state()->x_register_mode = 0;
        settings_->SavePersistentData();
        
        mode_ = UI_MODE_CALIBRATION_1;
      } else {
        NextCalibrationStep();
      }
      {
        const CalibrationData& cal = settings_->calibration_data();
        float voltage = (argument & 1) == 0 ? 1.0f : 3.0f;
        for (int i = 0; i < 4; ++i) {
          output_test_forced_dac_code_[i] =  static_cast<uint16_t>(
              voltage * cal.dac_scale[i] + cal.dac_offset[i]);
        }
      }
      queue_.Touch();
      break;
      
    case FACTORY_TESTING_FORCE_DAC_CODE:
      {
        int channel = argument >> 2;
        int step = argument & 0x3;
        if (step == 0) {
          output_test_forced_dac_code_[channel] = 0xaf35;
        } else if (step == 1) {
          output_test_forced_dac_code_[channel] = 0x1d98;
        } else {
          CalibrationData* cal = settings_->mutable_calibration_data();
          cal->dac_offset[channel] = static_cast<float>(
              calibration_data_ & 0xffff);
          cal->dac_scale[channel] = static_cast<float>(
              calibration_data_ >> 16) * -0.125f;
          output_test_forced_dac_code_[channel] = static_cast<uint16_t>(cal->dac_scale[channel] + cal->dac_offset[channel]);
          settings_->SavePersistentData();
        }
      }
      break;
      
    case FACTORY_TESTING_WRITE_CALIBRATION_DATA_NIBBLE:
      calibration_data_ <<= 4;
      calibration_data_ |= argument & 0xf;
      break;
  }
  return reply;
}

}  // namespace marbles
