#pragma once

#include <FS.h>
#include <SPIFFS.h>

#include "View.h"

struct DataFile {
  View& view;

  DataFile(View& view) : view(view) {}

  const char* dataFilePath = "/data.json";

  void begin() {
    if (!SPIFFS.begin(true)) {
      Serial.println("SPIFFS mount Failed.");
      delay(1000);
      return;
    }
  }

  void save(const char* data) {
    File file = SPIFFS.open(dataFilePath, FILE_APPEND, true);
    if (!file) {
      view.showError("Cannot save the data.");
    }
    file.print(data);
    file.close();
  }

  String read() {
    File file = SPIFFS.open(dataFilePath);
    if (!file || file.isDirectory()) {
      view.showError("Cannot read the data.");
      return "";
    }

    String data;
    while (file.available()) {
      data += char(file.read());
    }
    file.close();
    return data;
  }

  void clear() { SPIFFS.remove(dataFilePath); }
};