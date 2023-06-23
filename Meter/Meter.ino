#include <Adafruit_AS7341.h>  // depends on https://github.com/adafruit/Adafruit_AS7341
#include <M5Unified.h>  // depends on https://github.com/m5stack/M5Unified

#include "DataFile.h"
#include "Filter.h"
#include "GainSettings.h"
#include "SensorData.h"
#include "Serializer.h"
#include "View.h"

Adafruit_AS7341 as7341;

M5GFX& gfx = M5.Display;
View view(gfx);
GainSettings gainSettings;
DataFile dataFile(view);
Serializer serializer;
SensorData sensorData;

static void processReset() {
  if (M5.BtnPWR.wasClicked()) {
    ESP.restart();
  } else if (M5.BtnPWR.wasHold()) {
    M5.Power.powerOff();
  }
}

static void halt() {
  while (true) {
    M5.update();
    processReset();
    delay(100);
  }
}

void setup() {
  M5.begin();

  gfx.begin();
  gfx.setRotation(1);

  dataFile.begin();

  Wire.begin(0, 26);

  view.showMessage("Initializing AS7341...");
  if (!as7341.begin()) {
    view.showError("Failed to initialize AS7341.");
    halt();
  }
  as7341.startReading();

  gainSettings.configureSensor(as7341);

  M5.Speaker.begin();
  M5.Power.begin();
}

static void exportData() {
  String data = dataFile.read();
  if (data == "") {
    view.showMessage("No saved data.\n");
    delay(1000);
    return;
  }

  while (true) {
    view.showMessage("Exporting...");
    Serial.print(data);
    delay(1000);
    view.showMessage(
        "* Click to retry\n* Hold to clear and exit\n* Reset to keep data");

    while (true) {
      M5.update();
      processReset();
      if (M5.BtnA.wasClicked()) {
        break;
      } else if (M5.BtnA.wasHold()) {
        dataFile.clear();
        return;
      }
      delay(50);
    }
  }
}

void loop() {
  static bool gainModeSwitchClicked = false;

  M5.update();
  processReset();

  if (as7341.checkReadingProgress()) {
    // data is ready
    sensorData.read(as7341);
    view.draw(sensorData, gainSettings);

    gainSettings.adjust(as7341, sensorData, gainModeSwitchClicked);
    gainModeSwitchClicked = false;

    // start next cycle
    as7341.startReading();
  }

  if (M5.BtnA.wasHold()) {
    exportData();
  } else if (M5.BtnA.wasClicked()) {
    view.showMessage("Saving...");
    String line = serializer.serialize(sensorData, gainSettings) + "\n";
    dataFile.save(line.c_str());
    M5.Speaker.setVolume(255);
    M5.Speaker.tone(2000, 200);
    delay(1000);
  }

  gainModeSwitchClicked |= M5.BtnB.wasClicked();

  delay(50);
}
