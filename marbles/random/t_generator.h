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
// Generator for the T outputs.

#ifndef MARBLES_RANDOM_T_GENERATOR_H_
#define MARBLES_RANDOM_T_GENERATOR_H_

#include "stmlib/stmlib.h"

#include "marbles/ramp/ramp_divider.h"
#include "marbles/ramp/ramp_extractor.h"
#include "marbles/ramp/ramp_generator.h"
#include "marbles/ramp/slave_ramp.h"
#include "marbles/random/distributions.h"
#include "marbles/random/random_sequence.h"
#include "stmlib/dsp/hysteresis_quantizer.h"

namespace marbles {

enum TGeneratorModel {
  T_GENERATOR_MODEL_COMPLEMENTARY_BERNOULLI,
  T_GENERATOR_MODEL_CLUSTERS,
  T_GENERATOR_MODEL_DRUMS,

  T_GENERATOR_MODEL_INDEPENDENT_BERNOULLI,
  T_GENERATOR_MODEL_DIVIDER,
  T_GENERATOR_MODEL_THREE_STATES,
  
  T_GENERATOR_MODEL_MARKOV,
};

enum TGeneratorRange {
  T_GENERATOR_RANGE_0_25X,
  T_GENERATOR_RANGE_1X,
  T_GENERATOR_RANGE_4X,
};

const size_t kNumTChannels = 2;
const size_t kMarkovHistorySize = 16;
const size_t kNumDrumPatterns = 18;
const size_t kDrumPatternSize = 8;

struct DividerPattern {
  Ratio ratios[kNumTChannels];
  int32_t length;
};

struct Ramps {
  float* external;
  float* master;
  float* slave[kNumTChannels];
};

const size_t kNumDividerPatterns = 17;
const size_t kNumInputDividerRatios = 9;

class TGenerator {
 public:
  TGenerator() { }
  ~TGenerator() { }
  
  void Init(RandomStream* random_stream, float sr);
  void Process(
      bool use_external_clock,
      const stmlib::GateFlags* external_clock,
      Ramps ramps,
      bool* gate,
      size_t size,
      const stmlib::GateFlags* external_reset,
      bool external_hold);
  
  inline void set_model(TGeneratorModel model) {
    model_ = model;
  }
  
  inline void set_range(TGeneratorRange range) {
    range_ = range;
  }
  
  inline void set_rate(float rate) {
    rate_ = rate;
  }
  
  inline void set_bias(float bias) {
    bias_ = bias;
  }
  
  inline void set_jitter(float jitter) {
    jitter_ = jitter;
  }
  
  inline void set_deja_vu(float deja_vu) {
    sequence_.set_deja_vu(deja_vu);
  }

  inline void set_length(int length) {
    sequence_.set_length(length);
  }

  /**
   * set start
   * @param start start number (1-)
   */
  inline void set_start(int start) {
    sequence_.set_start(start);
  }

  inline void set_pulse_width_mean(float pulse_width_mean) {
    pulse_width_mean_ = pulse_width_mean;
  }
  
  inline void set_pulse_width_std(float pulse_width_std) {
    pulse_width_std_ = pulse_width_std;
  }

  inline void save_slot(int slot_index) {
    sequence_.save_slot(slot_index);

    for (size_t i = 0; i < kMarkovHistorySize; ++i) {
      streak_counter_slot_[slot_index][i] = streak_counter_[i];
      markov_history_slot_[slot_index][i] = markov_history_[i];
    }
    markov_history_ptr_slot_[slot_index] = markov_history_ptr_;
    drum_pattern_step_slot_[slot_index] = drum_pattern_step_;
    drum_pattern_index_slot_[slot_index] = drum_pattern_index_;
  }

  inline void load_slot(int slot_index) {
    sequence_.load_slot(slot_index);

    for (size_t i = 0; i < kMarkovHistorySize; ++i) {
      streak_counter_[i] = streak_counter_slot_[slot_index][i];
      markov_history_[i] = markov_history_slot_[slot_index][i];
    }
    markov_history_ptr_ = markov_history_ptr_slot_[slot_index];
    drum_pattern_step_ = drum_pattern_step_slot_[slot_index];
    drum_pattern_index_ = drum_pattern_index_slot_[slot_index];
  }

  inline void set_deja_vu_buffer_size(int bufferSize) {
    sequence_.set_deja_vu_buffer_size(bufferSize);
  }

 private:
  union RandomVector {
    struct {
      float pulse_width[kNumTChannels];
      float u[kNumTChannels];
      float p;
      float jitter;
    } variables;
    float x[2 * kNumTChannels + 2];
  };
  
  void ConfigureSlaveRamps(const RandomVector& v);
  int GenerateComplementaryBernoulli(const RandomVector& v);
  int GenerateIndependentBernoulli(const RandomVector& v);
  int GenerateThreeStates(const RandomVector& v);
  int GenerateDrums(const RandomVector& v);
  int GenerateMarkov(const RandomVector& v);
  void ScheduleOutputPulses(const RandomVector& v, int bitmask);

  float RandomPulseWidth(int i, float u) {
    if (pulse_width_std_ == 0.0f) {
      return 0.05f + 0.9f * pulse_width_mean_;
    } else {
      return 0.05f + 0.9f * BetaDistributionSample(
          u,
          pulse_width_std_,
          pulse_width_mean_);  // Jon Brooks
          // i & 1 ? 1.0f - pulse_width_mean_);
    }
  }
  
  float one_hertz_;
  
  TGeneratorModel model_;
  TGeneratorRange range_;
  
  float rate_;
  float bias_;
  float jitter_;
  float pulse_width_mean_;
  float pulse_width_std_;
  
  float master_phase_;
  float jitter_multiplier_;
  float phase_difference_;
  float previous_external_ramp_value_;
  
  bool use_external_clock_;

  bool gate_holding_;
  // float hold_master_phase_;

  int32_t divider_pattern_length_;
  int32_t streak_counter_[kMarkovHistorySize];
  int32_t markov_history_[kMarkovHistorySize];
  int32_t markov_history_ptr_;
  size_t drum_pattern_step_;
  size_t drum_pattern_index_;

  RandomSequence sequence_;
  RampDivider ramp_divider_;
  RampExtractor ramp_extractor_;
  RampGenerator ramp_generator_;

  SlaveRamp slave_ramp_[kNumTChannels];
  
  stmlib::HysteresisQuantizer bias_quantizer_;
  stmlib::HysteresisQuantizer rate_quantizer_;
  
  static DividerPattern divider_patterns[kNumDividerPatterns];
  static DividerPattern fixed_divider_patterns[kNumDividerPatterns];
  static Ratio input_divider_ratios[kNumInputDividerRatios];
  static uint8_t drum_patterns[kNumDrumPatterns][kDrumPatternSize];
  
  int32_t streak_counter_slot_[kLoopSlotNum][kMarkovHistorySize];
  int32_t markov_history_slot_[kLoopSlotNum][kMarkovHistorySize];
  int32_t markov_history_ptr_slot_[kLoopSlotNum];
  float drum_pattern_step_slot_[kLoopSlotNum];
  float drum_pattern_index_slot_[kLoopSlotNum];
  int length_;

  DISALLOW_COPY_AND_ASSIGN(TGenerator);
};

}  // namespace marbles

#endif  // MARBLES_RANDOM_T_GENERATOR_H_
