# AS7341 Spectrum Meter

This is the spectrum meter using [M5StickC Plus](https://shop.m5stack.com/products/m5stickc-plus-esp32-pico-mini-iot-development-kit) and AMS's [AS7341](https://ams.com/en/as7341) spectral light sensor. Please check out the following links for more background and explanations if you can read/translate Japanese.

* Blog: https://tomoto335.hatenablog.com/entry/as7341-spectral-color-sensor
* Qiita: https://qiita.com/tomoto335/items/b9d5716bc9f2f4dd03e6

There are two folders in this project, which contain two application programs.

* `Meter`: Adruino program of the meter device that runs on M5StickC Plus.
* `Viewer`: Processing program that runs on PC/Mac to receive and visualize the data exported by the meter device.

# Meter

## How to build

* Configure the board manager on Arduino for M5StickC.
* Install the dependent libraries as noted in Meter.ino.
* Build the application and upload to the device.

## Wiring

* Connect G0/G26 on M5StickC Plus's hat connector to SDA/SCL of AS7341 sensor.
* The I2C address follows the Adafruit's AS7341 library's default.

## Usage

* Just start the device, and it automatically starts measuring and displaying the 8-band spectrum.
* The maximum value read from the sensor is shown on the top of the screen.
  * When you measure a very weak light, make sure you have the sufficient resolution indicated by this value.
  * When you measure a very strong light, make sure the value is not saturated at the 50000 (full scale) but changes continuously below 50000.
* Click power button to reset. Hold power button to power off.
* Click button A to save the current spectrum to the device's flash memory (SPIFFS).
  * You can save multiple records.
* Hold button A to export the saved records to the USB serial.
  * The exported data contains a JSON-formatted 8-band spectrum record in each line.
  * If you successfully received the data on your PC (or any other target device), hold button A again to erase the saved data on the device and go back to the measurement.
  * If you failed to receive the data, click button A to retry. You can repeat until you succeed.
  * If you want to revert back to the measurement without erasing the saved records, click power button and reset the device.
* Click button B to switch to the long exposure mode (about 5 seconds for extremely weak light sources)
  * This is an experimental feature and the usability may be awful.

# Viewer

## Usage

* Open and run the sketch on Processing.
* The application will display the data stored in `data/data.json`.
  * The data file initially contains a couple of sample records.
* Use arrow keys (left or right) to go foward/back over the records in the data.
* Select the serial port from the drop down on the right top corner, then the application will monitor the selected port for the exported data from the meter device.
  * When the application received the exported data, it appends the received data to `data/data.json` and shows the new data.
  * The name of the data record will be auto-populated based on the timestamp.
* Edit `data/data.json` manually if you need to modify or delete the existing record.
  * You may want to change the auto-populated name to a more descriptive text.
  * Make sure you shut down the application before editing; otherwise, your edit may be overwitten.