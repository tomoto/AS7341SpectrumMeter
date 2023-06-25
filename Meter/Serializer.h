#pragma once

#include <ArduinoJson.h>

#include "GainSettings.h"
#include "SensorData.h"

struct Serializer {
  StaticJsonDocument<1024> json;

  String serialize(const SensorData& data, const GainSettings& gainSettings) {
    json.clear();
    auto rawArray = json.createNestedArray("raw");
    for (int value : data.rawValues) {
      rawArray.add(value);
    }
    auto balancedArray = json.createNestedArray("balanced");
    for (double value : data.balancedValues) {
      balancedArray.add(int(value));
    }
    json["gain"] = gainSettings.getSelectedGain().name;
    // TODO: It would be ideal to generate the name from the timestamp
    // json["name"] = "";

    String result;
    serializeJson(json, result);
    return result;
  }
};
