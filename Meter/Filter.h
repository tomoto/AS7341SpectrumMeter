#pragma once

#include <Adafruit_AS7341.h>

// Specification of each optical filter
struct Filter {
  int index;
  int wavelength;
  int width;
  as7341_color_channel_t channel;
  uint32_t color24;
};

// Definition of all the filters
struct Filters {
  static const int COUNT = 8;
  static const std::array<Filter, COUNT> filters;
  static const std::array<float, COUNT> whiteBalance;
};