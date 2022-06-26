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
// Sequence of random values.

#ifndef MARBLES_RANDOM_RANDOM_SEQUENCE_H_
#define MARBLES_RANDOM_RANDOM_SEQUENCE_H_

#include "stmlib/stmlib.h"

#include "marbles/random/random_stream.h"

#include <algorithm>

namespace marbles {

const int kDejaVuBufferSize = 320;//16;
const int kHistoryBufferSize = 320;//16;

const int kLoopSlotNum = 1;

const float kMaxUint32 = 4294967296.0f;

class RandomSequence {
 public:
  RandomSequence() { }
  ~RandomSequence() { }
  
  inline void Init(RandomStream* random_stream) {
    random_stream_ = random_stream;
    for (int i = 0; i < kDejaVuBufferSize; ++i) {
      loop_[i] = random_stream_->GetFloat();
      for (int j = 0; j < kLoopSlotNum; ++j) {
        loop_slot_[j][i] = loop_[i];
      }
    }
    std::fill(&history_[0], &history_[kHistoryBufferSize], 0.0f);
    for (int i = 0; i < kLoopSlotNum; ++i) {
      std::fill(&history_slot_[i][0], &history_slot_[i][kHistoryBufferSize], 0.0f);
    }

    loop_write_head_ = 0;
    for (int i = 0; i < kLoopSlotNum; ++i) {
      loop_write_head_slot_[i] = loop_write_head_;
    }
    length_ = 8;
    step_ = 0;

    record_head_ = 0;
    replay_head_ = -1;
    replay_start_ = 0;
    deja_vu_ = 0.0f;
    replay_hash_ = replay_shift_ = 0;

    redo_read_ptr_ = &loop_[0];
    redo_write_ptr_ = NULL;
    redo_write_history_ptr_ = NULL;

    start_ = 0;
    currentDejaVuBufferSize = 16;
    currentHistoryBufferSize = 16;
  }
  
  inline void Clone(const RandomSequence& source) {
    random_stream_ = source.random_stream_;
    
    std::copy(
        &source.loop_[0],
        &source.loop_[kDejaVuBufferSize],
        &loop_[0]);
    std::copy(
        &source.history_[0],
        &source.history_[kHistoryBufferSize],
        &history_[0]);
    
    loop_write_head_ = source.loop_write_head_;
    length_ = source.length_;
    step_ = source.step_;
    
    record_head_ = source.record_head_;
    replay_head_ = source.replay_head_;
    replay_start_ = source.replay_start_;
    replay_hash_ = source.replay_hash_;
    replay_shift_ = source.replay_shift_;
    
    deja_vu_ = source.deja_vu_;
    
    redo_read_ptr_ = source.redo_read_ptr_
        ? &loop_[source.redo_read_ptr_ - &source.loop_[0]]
        : NULL;
    redo_write_ptr_ = source.redo_write_ptr_
        ? &loop_[source.redo_write_ptr_ - &source.loop_[0]]
        : NULL;
    redo_write_history_ptr_ = source.redo_write_history_ptr_
        ? &history_[source.redo_write_history_ptr_ - &source.history_[0]]
        : NULL;

    for (int i = 0; i < kLoopSlotNum; ++i) {
      std::copy(
          &source.loop_slot_[i][0],
          &source.loop_slot_[i][kDejaVuBufferSize],
          &loop_slot_[i][0]);
      std::copy(
          &source.history_slot_[i][0],
          &source.history_slot_[i][kHistoryBufferSize],
          &history_slot_[i][0]);
      loop_write_head_slot_[i] = source.loop_write_head_slot_[i];
    }
    start_ = source.start_;
  }
  
  inline void Record() {
    replay_start_ = record_head_;
    replay_head_ = -1;
  }
  
  inline void ReplayPseudoRandom(uint32_t hash) {
    replay_head_ = replay_start_;
    replay_hash_ = hash;
    replay_shift_ = 0;
  }

  inline void ReplayShifted(uint32_t shift) {
    replay_head_ = replay_start_;
    replay_hash_ = 0;
    replay_shift_ = shift;
  }
  
  inline float GetReplayValue() const {
    uint32_t h = (replay_head_ - 1 - replay_shift_ + \
        2 * currentHistoryBufferSize) % currentHistoryBufferSize;
    if (!replay_hash_) {
      return history_[h];
    } else {
      uint32_t word = static_cast<float>(history_[h] * kMaxUint32);
      word = (word ^ replay_hash_) * 1664525L + 1013904223L;
      return static_cast<float>(word) / kMaxUint32;
    }
  }
  
  inline float RewriteValue(float value) {
    // RewriteValue(x) returns what the most recent call to NextValue would have
    // returned if its second argument were x instead. This is used to "rewrite
    // history" when the module acquires data from an external source (ASR,
    // randomizer or quantizer mode).
    if (replay_head_ >= 0) {
      return GetReplayValue();
    }
    
    if (redo_write_ptr_) {
      *redo_write_ptr_ = 1.0f + value;
    }
    float result = *redo_read_ptr_;
    if (result >= 1.0f) {
      result -= 1.0f;
    } else {
      result = 0.5f;
    }
    if (redo_write_history_ptr_) {
      *redo_write_history_ptr_ = result;
    }
    return result;
  }
  
  inline float NextValue(bool deterministic, float value) {
    if (replay_head_ >= 0) {
      replay_head_ = (replay_head_ + 1) % currentHistoryBufferSize;
      return GetReplayValue();
    }
    
    const float p_sqrt = 2.0f * deja_vu_ - 1.0f;
    const float p = p_sqrt * p_sqrt;
    float rho = random_stream_->GetFloat();

    if (rho < p && deja_vu_ <= 0.5f) {
      // Generate a new value and put it at the end of the loop.
      redo_write_ptr_ = &loop_[loop_write_head_];
      *redo_write_ptr_ = deterministic
          ? 1.0f + value
          : random_stream_->GetFloat();
      loop_write_head_ = (loop_write_head_ + 1) % currentDejaVuBufferSize;
      step_ = start_ + length_ - 1;
      if (step_ >= currentDejaVuBufferSize) {
        step_ -= currentDejaVuBufferSize;
      }
    } else {
      // Do not generate a new value, just replay the loop or jump randomly.
      // through it.
      redo_write_ptr_ = NULL;
      if (rho < p) {
        step_ = static_cast<int>(
            random_stream_->GetFloat() * static_cast<float>(length_));
        if (step_ >= currentDejaVuBufferSize) {
          if (start_ + length_ == currentDejaVuBufferSize) {
            step_ = start_ + (step_ - currentDejaVuBufferSize);
          } else {
            step_ -= currentDejaVuBufferSize;
          }
        }
      } else {
        step_ = step_ + 1;
        if (step_ >= currentDejaVuBufferSize) {
          if (start_ + length_ == currentDejaVuBufferSize) {
            step_ = start_;
          } else {
            step_ = 0;
          }
        } else if (step_ >= (start_ + length_)) {
          step_ = start_;
        }
      }
    }
    uint32_t i = loop_write_head_ + currentDejaVuBufferSize - length_ + step_;
    redo_read_ptr_ = &loop_[i % currentDejaVuBufferSize];
    float result = *redo_read_ptr_;
    if (result >= 1.0f) {
      result -= 1.0f;
    } else if (deterministic) {
      // We ask for a deterministic value (shift register), but the loop
      // contain random values. return 0.5f in this case!
      result = 0.5f;
    }
    redo_write_history_ptr_ = &history_[record_head_];
    *redo_write_history_ptr_ = result;
    record_head_ = (record_head_ + 1) % currentHistoryBufferSize;
    return result;
  }
  
  inline void NextVector(float* destination, size_t size) {
    float seed = NextValue(false, 0.0f);
    uint32_t word = static_cast<float>(seed * kMaxUint32);
    while (size--) {
      *destination++ = static_cast<float>(word) / kMaxUint32;
      word = word * 1664525L + 1013904223L;
    }
  }
  
  inline void set_deja_vu(float deja_vu) {
    deja_vu_ = deja_vu;
  }
  
  inline void set_length(int length) {
    if (length < 1 || length > kDejaVuBufferSize) {
      return;
    }
    length_ = length;
    recalc_step();
  }

  /**
   * set start
   * @param start start number (1-)
   */
  inline void set_start(int start) {
    if (start < 1 || start > kDejaVuBufferSize) {
      return;
    }
    int start_index = start - 1;
    start_ = start_index;
    recalc_step();
  }

  inline void recalc_step() {
    int tempEndIndex = start_ + length_ - 1;
    if (tempEndIndex >= currentDejaVuBufferSize) {
      if (step_ > (tempEndIndex - currentDejaVuBufferSize)
          && step_ < start_) {
        step_ = start_;
      }
    } else {
      if (step_ > tempEndIndex) {
        step_ = start_;
      } else if (step_ < start_) {
        step_ = start_;
      }
    }
  }

  inline float deja_vu() const {
    return deja_vu_;
  }
  
  inline int length() const {
    return length_;
  }

  inline void save_slot(int slot_index) {
    for (int i = 0; i < kDejaVuBufferSize; ++i) {
      loop_slot_[slot_index][i] = loop_[i];
    }
    for (int i = 0; i < kHistoryBufferSize; ++i) {
      history_slot_[slot_index][i] = history_[i];
    }
    loop_write_head_slot_[slot_index] = loop_write_head_;
  }

  inline void load_slot(int slot_index) {
    for (int i = 0; i < kDejaVuBufferSize; ++i) {
      loop_[i] = loop_slot_[slot_index][i];
    }
    for (int i = 0; i < kHistoryBufferSize; ++i) {
      history_[i] = history_slot_[slot_index][i];
    }
    loop_write_head_ = loop_write_head_slot_[slot_index];
  }

  inline void reset_step() {
    step_ = start_ + length_;
    // next process() call, step_ will be resetted to start position.
  }

  inline int step() {
    return step_;
  }

  inline bool deja_vu_ing() {
    return deja_vu_ == 0.5f;
  }

  inline void set_deja_vu_buffer_size(int bufferSize) {
    if (currentDejaVuBufferSize != bufferSize) {
      currentDejaVuBufferSize = bufferSize;
      currentHistoryBufferSize = bufferSize;
      recalc_step();
    }
  }

 private:
  RandomStream* random_stream_;
  float loop_[kDejaVuBufferSize];
  float history_[kHistoryBufferSize];
  int loop_write_head_;
  int length_;
  int step_;
  
  // Allows to go back in the past and get the same results again from NextValue
  // calls. Allows the 3 X channels to be locked to the same random loop.
  int record_head_;
  int replay_head_;
  int replay_start_;
  uint32_t replay_hash_;
  uint32_t replay_shift_;
  
  float deja_vu_;
  
  float* redo_read_ptr_;
  float* redo_write_ptr_;
  float* redo_write_history_ptr_;
  
  float loop_slot_[kLoopSlotNum][kDejaVuBufferSize];
  float history_slot_[kLoopSlotNum][kHistoryBufferSize];
  int loop_write_head_slot_[kLoopSlotNum];
  // start index (0-)
  int start_;
  int currentDejaVuBufferSize;
  int currentHistoryBufferSize;

  DISALLOW_COPY_AND_ASSIGN(RandomSequence);
};

}  // namespace marbles

#endif  // MARBLES_RANDOM_RANDOM_SEQUENCE_H_
