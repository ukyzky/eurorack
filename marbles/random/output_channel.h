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
// Random generation channel.

#ifndef MARBLES_RANDOM_OUTPUT_CHANNEL_H_
#define MARBLES_RANDOM_OUTPUT_CHANNEL_H_

#include "stmlib/stmlib.h"
#include "stmlib/utils/gate_flags.h"

#include "marbles/random/lag_processor.h"
#include "marbles/random/quantizer.h"

namespace marbles {

class RandomSequence;

struct ScaleOffset {
  ScaleOffset(float s, float o) {
    scale = s;
    offset = o;
  }
  
  ScaleOffset() { scale = 1.0f; offset = 0.0f; }
  
  float scale;
  float offset;
  inline float operator()(float x) { return x * scale + offset; }
};

class OutputChannel {
 public:
  OutputChannel() { }
  ~OutputChannel() { }
  
  void Init();
  
  void LoadScale(int i, const Scale& scale) {
    quantizer_[i].Init(scale);
  }
  
  void Process(
      RandomSequence* random_sequence,
      const float* phase,
      float* output,
      size_t size,
      size_t stride,
      const stmlib::GateFlags* external_reset,
      bool external_hold,
      float* used_voltages = NULL,
      int num_used_voltages = 0);

  float QuantizeEx(float voltage, float amount, float root, float* used_voltages = NULL, int num_used_voltages = 0);
  float QuantizeEx2(float voltage, float amount, float root, float* used_voltages = NULL, int num_used_voltages = 0);

  inline void set_spread(float spread) {
    spread_ = spread;
  }
  
  inline void set_bias(float bias) {
    bias_ = bias;
  }
  
  inline void set_scale_index(int i) {
    scale_index_ = i;
  }
  
  inline void set_steps(float steps) {
    steps_ = steps;
  }
  
  inline void set_register_mode(bool register_mode) {
    register_mode_ = register_mode;
  }

  inline void set_register_value(float register_value) {
    register_value_ = register_value;
  }
  
  inline void set_register_transposition(float register_transposition) {
    register_transposition_ = register_transposition;
  }
  
  inline void set_scale_offset(const ScaleOffset& scale_offset) {
    scale_offset_ = scale_offset;
  }
  
  inline float Quantize(float voltage, float amount, float* used_voltages = NULL, int num_used_voltages = 0) {
    quantized_ = true;
    if (used_voltages) {
      return quantizer_[scale_index_].ProcessEx(voltage, amount, false, used_voltages, num_used_voltages);
    }
    return quantizer_[scale_index_].Process(voltage, amount, false);
  }
  
  inline void set_root_mode(int root_mode) {
    root_mode_ = root_mode;
  }

  inline void set_chord_mode(int chord_mode) {
    chord_mode_ = chord_mode;
  }

  inline void set_slew_rate(float slew_rate) {
    slew_rate_ = slew_rate;
  }

  inline void set_same_note_probability(float probability) {
    same_note_probability_ = probability;
  }

  inline void set_same_note(int same_note) {
    same_note_ = same_note;
  }

  inline float get_quantized_voltage() {
    return quantized_ ? quantized_voltage_ : -99.0f; // magic number for never hit as used
  }

 private:
  float GenerateNewVoltage(RandomSequence* random_sequence);
  
  float spread_;
  float bias_;
  float steps_;
  int scale_index_;
  
  bool gate_holding_;
  // float before_reset_voltage_;

  bool register_mode_;
  float register_value_;
  float register_transposition_;
  
  float previous_steps_;
  float previous_phase_;
  uint32_t reacquisition_counter_;
  
  float previous_voltage_;
  float voltage_;
  float quantized_voltage_;
  
  ScaleOffset scale_offset_;
  
  LagProcessor lag_processor_;
  
  Quantizer quantizer_[6];

  int root_mode_;

  int chord_mode_;
  float slew_rate_; // for chord_mode_
  float same_note_probability_; // for chord_mode_
  int same_note_; // for chord_mode_
  bool quantized_; // for chord_mode_
  
  DISALLOW_COPY_AND_ASSIGN(OutputChannel);
};

}  // namespace marbles

#endif  // MARBLES_RANDOM_OUTPUT_CHANNEL_H_
