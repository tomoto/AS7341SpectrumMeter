#pragma once

#include <M5GFX.h>

#include "GainSettings.h"
#include "SensorData.h"

struct View {
  M5GFX& gfx;
  bool isClearRequired = true;

  View(M5GFX& gfx) : gfx(gfx) {}

  void showMessage(const char* message, uint16_t color = TFT_WHITE) {
    isClearRequired = true;
    gfx.clearDisplay();
    gfx.setTextColor(color);
    gfx.setCursor(2, 2, 2);
    gfx.print(message);
  }

  void showError(const char* message) { showMessage(message, TFT_ORANGE); }

  void draw(const SensorData& d, const GainSettings& gainSettings) {
    gfx.startWrite();

    if (isClearRequired) {
      gfx.clearDisplay();
      isClearRequired = false;
    }

    for (const Filter& f : Filters::filters) {
      const int BAR_WIDTH = 30;
      const int CHART_BOTTOM = 135;
      const int CHART_HEIGHT = 118;
      int x = f.index * BAR_WIDTH;
      int h = map(d.balancedValues[f.index], 0, d.maxBalancedValue, 0, 120);
      gfx.fillRect(x, CHART_BOTTOM - h, BAR_WIDTH - 1, h,
                   gfx.color24to16(f.color24));
      gfx.fillRect(x, CHART_BOTTOM - CHART_HEIGHT, BAR_WIDTH - 1,
                   CHART_HEIGHT - h, TFT_BLACK);
    }
    gfx.setCursor(2, 2, 2);
    gfx.setTextColor(
        d.maxRawValue == gainSettings.getSelectedGain().getMaxValue()
            ? TFT_ORANGE
            : TFT_WHITE,
        TFT_BLACK);
    gfx.printf("Gain %s / Max %d       ", gainSettings.getSelectedGain().name,
               d.maxRawValue);
    gfx.endWrite();
  }
};
