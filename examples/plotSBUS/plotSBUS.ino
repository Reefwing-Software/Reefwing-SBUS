/******************************************************************
  @file       serialSBUS
  @brief      Display SBUS data via the Serial Port.
  @author     David Such
  @copyright  Please see the accompanying LICENSE file.

  Code:        David Such
  Version:     1.0
  Date:        27/03/22

  1.0 Original Release          27/03/22

  This example sketch assumes that inverted SBUS data is being fed
  to pin 2 (D0/Rx) on the Nano 33 BLE (i.e., Serial1).

  This sketch is designed to be used with the Arduino IDE Serial 
  Plotter (CTRL+SHFT+L). The vertical (y-axis) on the plotter
  auto scales and the x-axis is a fixed 500 points with each tick
  mark indicative of a Serial.println(). With a 20 ms delay you see
  10 seconds of data (i.e., 500 x 20 ms).

  If you want to stop autoscaling, add the following to loop():
  Serial.println("Min:MIN_SBUS, Max:MAX_SBUS");

  

******************************************************************/

#include <NexgenSBUS.h>

#define MIN_SBUS          172
#define MAX_SBUS          1811

uint16_t channels[16];
bool failSafe, lostFrame;

SBUS x8R(Serial1);

/******************************************************************
   Serial Plot Helper Function

   Usage:  plot("Humidity",humidity_value,false); // label, data , plots to follow?
           plot("Pressure",pressure_value,true);  // label, data, last plot.

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

  Serial.println("Parsing SBUS packets...\n");
  x8R.begin();
}

void loop() {
  // Check for a valid SBUS packet from the X8R receiver
  if (x8R.read(&channels[0], &failSafe, &lostFrame)) {
    plot("CH 1", channels[0], false);
    plot("CH 2", channels[1], false);
    plot("CH 3", channels[2], false);
    plot("CH 4", channels[3], true);
  }

  delay(120);  //  This delay shows 60 secs of data.
}