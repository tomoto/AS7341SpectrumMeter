# AS7341 Spectrum Meter

This is the spectrum meter using AMS's [AS7341](https://ams.com/en/as7341) spectral light sensor. In this project, I specifically used [Adafruit's AS7341 breakout board](https://www.adafruit.com/product/4698) and microcontroller [M5StickC Plus](https://shop.m5stack.com/products/m5stickc-plus-esp32-pico-mini-iot-development-kit). Please check out the following links for more background and explanations if you can read/translate Japanese.

* Blog: https://tomoto335.hatenablog.com/entry/as7341-spectral-color-sensor
* Qiita: https://qiita.com/tomoto335/items/b9d5716bc9f2f4dd03e6

![AS7341 Sensor](./images/AS7341Sensor.jpg?raw=true "AS7341 Sensor")

![Spectrum Meter](./images/SpectrumMeter.jpg?raw=true "Spectrum Meter")

There are two folders in this project, and they host the following applications respectively.

* `Meter`: Adruino program of the meter device (that runs on M5StickC Plus).
* `Viewer`: Processing program to receive and visualize the data exported by the meter device (that runs on PC/Mac).

# Meter

## How to build

* Configure the board manager on Arduino for M5StickC.
* Install the dependent libraries as noted in [Meter.ino](./Meter/Meter.ino).
* Build the application and upload to the device.

## Wiring

* Connect G0/G26 of M5StickC Plus's hat connector to SDA/SCL of AS7341 respectively.
* Note the I2C address is assumed as Adafruit's AS7341 library's default. Change the code if you use a different sensor board that has a different address.

## How it works

* Just start the device, and it automatically starts displaying the 8-band spectrum detected by the sensor.
  * The bar height is relative to the strongest spectrum band. It does not represent any absolute strength of the light.
* The maximum value read from the sensor is shown on the top of the screen.
  * If this value is too small, the light may be too weak and the result may be prone to noise.
  * If this value gets stuck at 50000, the light may be too strong and the result may be inaccurate due to satulation.

## How to operate

* Click (= short press) power button to reset. Hold (= long press) power button to turn off.
* Click button A to save the current spectrum to the device's flash memory (SPIFFS).
  * You can save multiple records.
* Hold button A to export the saved records to the USB serial.
  * The exported data contains a batch of JSON-formatted 8-band spectrum records, one record per line.
  * After the data is exported, make sure you successfully received the data on your PC (or any other external devices). If everything is fine, hold button A again to erase the saved data inside the device and go back to the measurement.
  * Or, if you failed to receive the data, click button A to retry.
  * Or, if you want to revert back to the measurement without erasing the saved records, click power button to reset .
* Click button B to switch to the long exposure mode (about 5 seconds for extremely weak light sources)
  * This is an experimental feature and the usability may be awful.

# Viewer

## How it works

* Open and run the sketch on Processing, then the application shows the first record of the spectrum data stored in [data.json](./Viewer/data/data.json).
  * The data file initially contains a couple of sample records.

## How to operate

* Use arrow keys (left or right) to go foward/back over the records in the data.
* Select the serial port from the drop down on the right top corner, then the application will monitor the selected port for the data exported by the meter device.
  * When the application received the data, the new data will be added to to `data.json` and shown on the screen.
  * The name of the record will be auto-populated based on the timestamp.
  * Press [p] to refresh the serial port list.
* Edit `data.json` manually if you need to modify or delete the existing record.
  * You may want to change the auto-populated name to a more descriptive text.
  * Make sure you shut down the application before editing; otherwise, your edit may be overwitten.