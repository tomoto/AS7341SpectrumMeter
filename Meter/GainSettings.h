#pragma once

#include <Adafruit_AS7341.h>

#include "SensorData.h"

struct GainSettings {
  struct Gain {
    uint16_t astep;
    uint8_t atime;
    as7341_gain_t gain;
    const char* name;

    int getMaxValue() const { return min((astep + 1) * (atime + 1), 65535); }
  };

  const std::array<Gain, 8> gains{{
      {5000 - 1, 10 - 1, AS7341_GAIN_0_5X, ".5"},
      {5000 - 1, 10 - 1, AS7341_GAIN_2X, "2"},
      {5000 - 1, 10 - 1, AS7341_GAIN_8X, "8"},
      {5000 - 1, 10 - 1, AS7341_GAIN_32X, "S"},
      {5000 - 1, 10 - 1, AS7341_GAIN_128X, "L"},
      {5000 - 1, 10 - 1, AS7341_GAIN_512X, "M"},
      {5000 - 1, 40 - 1, AS7341_GAIN_512X, "H"},
      {50000 - 1, 40 - 1, AS7341_GAIN_512X, "X"},
  }};

  const int X = gains.size() - 1;

  int selected = X - 1;

  void adjust(Adafruit_AS7341& as7341, const SensorData& data,
              bool switchLongMode) {
    bool reconfigure = false;
    if (switchLongMode) {
      selected = (selected == X) ? X - 1 : X;
      reconfigure = true;
    }

    if (selected < X) {
      if (data.maxRawValue > 40000 && selected > 0) {
        --selected;
        reconfigure = true;
      } else if (data.maxRawValue < 10000 && selected < X - 1) {
        ++selected;
        reconfigure = true;
      }
    }

    if (reconfigure) {
      configureSensor(as7341);
    }
  }

  void configureSensor(Adafruit_AS7341& as7341) const {
    const Gain& g = gains[selected];
    as7341.setASTEP(g.astep);
    as7341.setATIME(g.atime);
    as7341.setGain(g.gain);
  }

  const Gain& getSelectedGain() const { return gains[selected]; }
};
