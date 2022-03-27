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

  The data will be output on Serial via the USB port.

  We assign our transmitter controls as follows:

  SBUS Channels - Throttle:     channels[0]
                - Aileron:      channels[1] (Roll)
                - Elevator:     channels[2] (Pitch)
                - Rudder:       channels[3] (Yaw)
                - BUZZER        channels[4]
                - RSSI:         channels[5]
                - Rx Voltage:   channels[6]
                - ARM / DISARM  channels[7]
                - FLIGHT MODE   channels[8]

******************************************************************/

#include <NexgenSBUS.h>

#define MIN_RATE_ROLL    -60
#define MIN_RATE_PITCH   -60
#define MAX_RATE_ROLL     60
#define MAX_RATE_PITCH    60

#define MIN_PULSEWIDTH    1000
#define IDLE_PULSEWIDTH   1100
#define MAX_PULSEWIDTH    2000

#define MIN_YAW          -270
#define MAX_YAW           270

#define MIN_THROTTLE      0
#define MAX_THROTTLE      100

#define MIN_SBUS          172
#define MAX_SBUS          1811
#define SWITCH_ON         991

SBUS x8R(Serial1);

uint16_t channels[16];
uint16_t lostFrameCtr = 0;
uint16_t rssi = 0, rxBat = 0, rxArm = 0, rxBuzzer = 0, rxMode = 0;
int16_t rxThrottle = 0, rxRoll = 0, rxPitch = 0, rxYaw = 0;

bool failSafe;
bool lostFrame;
bool armed = false;

void setup() {
  //  Start Serial and wait for connection from computer
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Parsing SBUS packets...\n");
  x8R.begin();

  //  Display SBUS Channel Header
  Serial.println("CH 1\tCH 2\tCH 3\tCH 4\tCH 5\tCH 6\tCH 7\tCH 8\tCH 9");
}

void loop() {
    // Check for valid SBUS packet from the X8R receiver
  if (x8R.read(&channels[0], &failSafe, &lostFrame)) {
    rxThrottle = map(channels[0], MIN_SBUS, MAX_SBUS, MIN_PULSEWIDTH, MAX_PULSEWIDTH);
    rxRoll  = map(channels[1], MIN_SBUS, MAX_SBUS, MIN_RATE_ROLL, MAX_RATE_ROLL);
    rxPitch = map(channels[2], MIN_SBUS, MAX_SBUS, MIN_RATE_PITCH, MAX_RATE_PITCH);
    rxYaw = map(channels[3], MIN_SBUS, MAX_SBUS, MIN_YAW, MAX_YAW);

    //  AUX Channels
    rxBuzzer = channels[4];
    rssi     = channels[5];
    rxBat    = channels[6];
    rxArm    = channels[7];
    rxMode   = channels[8];

    //  if (lostFrame) { lostFrameCtr++; }
    //  if (rxArm >= SWITCH_ON) { Serial.println("ARMED"); }
    //  if (rxArm < SWITCH_ON) { Serial.println("DISARMED"); }

    //  Display SBUS Data
    Serial.print(rxThrottle); Serial.print("\t"); Serial.print(rxRoll); Serial.print("\t"); Serial.print(rxPitch); Serial.print("\t"); Serial.print(rxYaw); Serial.print("\t");
    Serial.print(rxBuzzer); Serial.print("\t"); Serial.print(rssi); Serial.print("\t"); Serial.print(rxBat); Serial.print("\t"); Serial.print(rxArm); Serial.print("\t"); Serial.println(rxMode);
  }

  delay(10);
}