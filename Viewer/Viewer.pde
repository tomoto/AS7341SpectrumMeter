import processing.serial.*;

import controlP5.*;

import java.util.Date;
import java.io.FileWriter;

// Data file (in data folder)
final String dataFile = "data.json";

final String fontName = "Corbel";

// Specification of each optical filter
static class Filter {
  public int wavelength;
  public int width;
  public color col;
  
  public Filter(int wavelength, int width, color col) {
    this.wavelength = wavelength;
    this.width = width;
    this.col = col;
  }
}

// Filter spec from Section 6 of
// https://ams.com/documents/20143/36005/AS7341_DS000504_3-00.pdf/5eca1f59-46e2-6fc5-daf5-d71ad90c9b2b
// Wavelength to RGB conversion from
// https://www.johndcook.com/wavelength_to_RGB.html
final Filter[] filters = {
  new Filter(415, 26, #7600ed),
    new Filter(445, 30, #0028ff),
    new Filter(480, 36, #00d5ff),
    new Filter(515, 39, #1fff00),
    new Filter(555, 39, #b3ff00),
    new Filter(590, 40, #ffdf00),
    new Filter(630, 50, #ff4f00),
    new Filter(680, 52, #ff0000),
  };

// Corrective coefficient from Figure 10 of
// https://ams.com/documents/20143/36005/AS7341_AN000633_1-00.pdf/fc552673-9800-8d60-372d-fc67cf075740
// with my own correction for F1~F3
static final float[] whiteBalance = {
  3.20 * 0.8,
  3.00 * 0.8,
  2.07 * 0.8,
  1.30,
  1.07,
  0.93,
  0.78,
  0.71,
};

//// Corrective coefficient from Figure 19 of
//// https://cdn.sparkfun.com/assets/0/8/e/2/3/AS7341_DS000504_3-00.pdf
//// with my own correction for F1
//static final float[] whiteBalance = {
//  1.0 / 0.17 * 0.6,
//  1.0 / 0.30,
//  1.0 / 0.38,
//  1.0 / 0.50,
//  1.0 / 0.60,
//  1.0 / 0.65,
//  1.0 / 0.75,
//  1.0,
//};


// View data
JSONArray json;
int displayedIndex = 0;

// Serial communication data
Serial port;
String portName;
int indexInBatch = 0;

// View components
PFont font;
ControlP5 cp5;
ScrollableList portDropdown;
Textlabel indexLabel;

void setup() {
  size(500, 320);
  surface.setTitle("AS7341 Spectrum Viewer");
  
  // Read data file
  try {
    json = loadJSONArray(dataFile);
  } catch(Exception e) {
    json = new JSONArray();
    e.printStackTrace();
  }
  
  font = createFont(fontName, 16);
  
  // Create view controls
  cp5 = new ControlP5(this);
  
  portDropdown = cp5.addScrollableList("portDropdown")
   .setPosition(390, 10)
   .close();
  portDropdown.getCaptionLabel().set("Serial port to read");
  populatePorts();
  
  indexLabel = cp5.addLabel("indexLabel")
   .setPosition(10, 10)
   .setText("0/0");
}

void populatePorts() {
  portDropdown.clear().addItem("<NO PORT>", null);
  for (String portName : Serial.list()) {
    portDropdown.addItem(portName, portName);
  }
}

void drawChart() {
  if (json.size() == 0) {
    return; // no data
  }
  
  final int chartBottom = 280;
  final int chartHeight = 220;
  final int chartLeft = 50;
  final int barWidth = 50;
  final color textColor = color(255, 255, 255, 200);
  
  JSONObject record = json.getJSONObject(displayedIndex);
  String name = record.getString("name");
  if (name == null) {
    name = "";
  }
  
  float[] values = new float[filters.length];
  JSONArray rawValues = record.getJSONArray("raw");
  for (int i = 0; i < values.length; i++) {
    values[i] = rawValues.getFloat(i) * whiteBalance[i];
  }
  // You may also take the pre-balanced values provided by the sensor device
  // float[] values = record.getJSONArray("balanced").getFloatArray();
  float maxValue = max(values);
  
  // bars
  textFont(font);
  textAlign(CENTER);
  for (int i = 0; i < values.length; i++) {
    Filter f = filters[i];
    fill(red(f.col), green(f.col), blue(f.col), 220);
    noStroke();
    float h = values[i] / maxValue * chartHeight; 
    quad(chartLeft + barWidth * (i - 0.1), chartBottom,
      chartLeft + barWidth * (i + 0.1), chartBottom - h,
      chartLeft + barWidth * (i + 0.9), chartBottom - h,
      chartLeft + barWidth * (i + 1.1), chartBottom);
    
    textSize(18);
    text(Integer.toString(f.wavelength), i * 50 + 75, 300);
  }
  
  // scale
  stroke(textColor);
  for (float i = 0; i <= 1.0; i += 0.5) {
    float y = chartBottom - chartHeight * i;
    line(chartLeft - 20, y, chartLeft + barWidth * filters.length + 20, y);
  }
  
  // title
  fill(textColor);
  textSize(24);
  text(name, width / 2, 40);
  
  // index
  indexLabel.setText(String.format("%d/%d", displayedIndex + 1, json.size()));
}

void readData() {
  if (port == null && portDropdown.getValue() != 0) {
    // delayed update when the port is eliminated
    portDropdown.setValue(0);
  }
  
  // read the data from serial
  if (port != null && port.available() > 0) {
    String line = port.readStringUntil('\n');
    if (line != null && line.startsWith("{")) {
      try {
        // parse the json
        JSONObject record = JSONObject.parse(line);
        // supply the name based on the timestamp and the index in the batch
        String id = String.format("%s #%d", new Date().toString(), ++indexInBatch);
        record.setString("id", id);
        record.setString("name", id);
        // add the new data
        json.append(record);
        
        // move the screen to the first data of this batch          
        if (indexInBatch == 1) {
          displayedIndex = json.size() - 1;
        }
      } catch(Exception e) {
        e.printStackTrace();
      }
    }
  } else {
    if (indexInBatch > 0) {
      // batch ended, save the data
      saveJSONArray(json, "data/" + dataFile);
      indexInBatch = 0;
    }
  }
}

void draw() {
  background(0);
  drawChart();
  readData();
}

void keyPressed() {
  if (key == CODED) {
    // right/left arrow to switch the data to display
    if (keyCode == RIGHT) {
      displayedIndex = (displayedIndex + 1) % json.size();
    } else if (keyCode == LEFT) {
      displayedIndex = (displayedIndex + json.size() - 1) % json.size();
    }
  } else if (key == 'p') {
    // reload port list
    closePort();
    populatePorts();
  }
}

void closePort() {
  if (port != null) {
    port.stop();
    port = null;
  }
}

public void controlEvent(ControlEvent theEvent) {
  // port selection
  if (theEvent.getController() == portDropdown) {
    String portName = (String)portDropdown.getItem((int)theEvent.getValue()).get("value");
    closePort();
    if (portName != null) {
      try {
        port = new Serial(this, portName, 115200);
      } catch(Exception e) {
        e.printStackTrace();
        // continue without port
      }
    }
  }
}
