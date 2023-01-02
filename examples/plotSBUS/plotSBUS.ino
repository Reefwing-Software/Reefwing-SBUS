/******************************************************************
  @file       plotSBUSino
  @brief      Display SBUS data using the IDE Serial Plotter.
  @author     David Such
  @copyright  Please see the accompanying LICENSE file.

  Code:        David Such
  Version:     2.0.1
  Date:        02/01/23

  1.0.0 Original Release        27/03/22
  2.0.0 Change Repo & Branding  15/12/22
  2.0.1 Updated README          02/01/23

  This sketch will plot the first 4 channels of SBUS data to the
  Arduino IDE Serial Plotter. The SBUS protocol returns a value 
  between 172 and 1811 on each standard channel. The channel data 
  is returned in an array as a 16 bit unsigned integer. 

  The variables `lostFrame` and `failSafe`, returned by the SBUS 
  class are booleans.

  This example sketch assumes that inverted SBUS data is being fed
  to pin 2 (D0/Rx) on the Nano 33 BLE (i.e., Serial1).

  This sketch is designed to be used with the Arduino IDE Serial 
  Plotter (CTRL+SHFT+L). The vertical (y-axis) on the plotter
  auto scales and the x-axis is a fixed 500 points with each tick
  mark indicative of a Serial.println(). With a 10 ms delay you see
  5 seconds of data (i.e., 500 x 10 ms).

******************************************************************/

#include <ReefwingSBUS.h>

uint16_t channels[16];
bool failSafe, lostFrame;

SBUS x8R(Serial1);

/******************************************************************
   Serial Plot Helper Function

   Usage:  plot("Humidity",humidity_value,false); // label, data , plots to follow?
           plot("Pressure",pressure_value,true);  // label, data, last plot.

   Note:  labels can't contain a space, but can be an empty String
          (i.e., "").

 ******************************************************************/

void plot(String label, int value, bool last) {
  Serial.print(label); // May be an empty string.
  if (label != "") Serial.print(":");
  Serial.print(value);
  if (last == false) Serial.print(",");
  else Serial.println();
}

void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  //  Start Parsing SBUS packets...
  x8R.begin();
}

void loop() {
  // Check for a valid SBUS packet from the X8R receiver
  if (x8R.read(&channels[0], &failSafe, &lostFrame)) {
    plot("CH_1", channels[0], false);
    plot("CH_2", channels[1], false);
    plot("CH_3", channels[2], false);
    plot("CH_4", channels[3], true);
  }

  delay(10);  //  This delay shows 60 secs of data.
}