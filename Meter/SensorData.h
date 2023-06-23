#pragma once

#include <Adafruit_AS7341.h>

#include "Filter.h"

struct SensorData {
  std::array<int, Filters::COUNT> rawValues;
  std::array<double, Filters::COUNT> balancedValues;

  int maxRawValue;
  double maxBalancedValue;

  void read(Adafruit_AS7341& as7341) {
    uint16_t channelData[12];
    as7341.getAllChannels(channelData);

    for (const Filter& f : Filters::filters) {
      int value = channelData[f.channel];
      rawValues[f.index] = value;
      balancedValues[f.index] = value * Filters::whiteBalance[f.index];
    }

    maxRawValue = *std::max_element(rawValues.begin(), rawValues.end());
    maxBalancedValue =
        *std::max_element(balancedValues.begin(), balancedValues.end());
  }
};
