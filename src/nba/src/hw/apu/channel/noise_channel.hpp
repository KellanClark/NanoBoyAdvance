/*
 * Copyright (C) 2021 fleroviux
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once

#include <nba/integer.hpp>

#include "hw/apu/channel/base_channel.hpp"
#include "hw/apu/registers.hpp"
#include "scheduler.hpp"

namespace nba::core {

class NoiseChannel : public BaseChannel {
public:
  NoiseChannel(Scheduler& scheduler, BIAS& bias);

  void Reset();
  auto GetSample() -> s8 override { return sample; }
  void Generate(int cycles_late);
  auto Read (int offset) -> u8;
  void Write(int offset, u8 value);

private:
  constexpr int GetSynthesisInterval(int ratio, int shift) {
    int interval = 64 << shift;

    if (ratio == 0) {
      interval /= 2;
    } else {
      interval *= ratio;
    }

    return interval;
  }

  u16 lfsr;
  s8 sample = 0;

  Scheduler& scheduler;
  std::function<void(int)> event_cb = [this](int cycles_late) {
    this->Generate(cycles_late);
  };

  int frequency_shift;
  int frequency_ratio;
  int width;
  bool dac_enable;

  BIAS& bias;
  int skip_count;
};

} // namespace nba::core
