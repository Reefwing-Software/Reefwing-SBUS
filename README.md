# Nexgen SBUS
 
 SBUS Library for the Arduino Nano 33 BLE and Portenta H7.

 ## The SBUS Protocol

 In the old days you would take PWM control data from a pin on your radio control receiver, with each pin representing a channel on your remote control transmitter. A channel could be throttle, yaw, pitch, roll, etc. It is now more common to use a digital protocol to transmit this information. An example of this is SBUS. The protocol is designed to condense 16 individual channels into a single data frame.

 The SBUS protocol was developed by Futaba to control servos with each channel assigned to a different servo. It is based on RS232 but to make life difficult they inverted the signals (i.e., 0 = VCC and 1 = GND), put the most significant bit first (UART has LSB) and used a non-standard baud rate of 100,000. 
 
 The other configuration details are (8E2):

- 1 start bit
- 8 data bits
- 1 even parity bit
- 2 stop bits

Each SBUS message is 25 bytes long and takes 3 ms to be transmitted. A Futaba transceiver in analogue mode will send a message every 14 ms, and in high speed mode, every 7 ms. The FrSKY transceivers generally send a message every 10 ms. 

A message consists of:

- A header byte (0x0F);
- 16 x 11 bit channels (total of 22 bytes);
- 1 flag byte, which includes two digital channels [0 0 0 0 fail_safe frame_lost ch18 ch17]; and
- 1 footer byte (0x00).

Each of the 16 channels can have a value in the range of 172–1811, which we will map to 1000–2000.

We are using the FrSKY X8R Remote Control Receiver which provides 8 channels using PWM or 16 channels via SBUS. It also gives us signal strength via the RSSI pin and we can transmit telemetry over the Smart Port connection. FrSKY SBUS is inverted compared to Futaba SBUS.

## Inverting SBUS
We captured an SBUS packet from the FrSKY X8R using our protocol analyser. From this we determined that it is the opposite of what Futaba SBUS expects (i.e., normal!), LOW is GND and HIGH is VCC (5V).

We need to invert this signal to get a Futaba compatible SBUS packet. In addition, the FrSKY X8R is generating 5V logic and our Nano 33 BLE pins are not 5V tolerant. Consequently, we need to shift the voltage down to 3.3V before feeding the inverted signal to the Nano.

Refer to Part 2 of our article on [How to Write your own Flight Controller Software](https://reefwing.medium.com/how-to-write-your-own-flight-controller-software-part-2-bc7f27214bb2), to see a circuit which will perform this level shifting and inversion.

## Examples

### 1. Serial SBUS

The serialSBUS example sketch will print out SBUS data to the Serial port. It assumes that inverted SBUS data is being fed to pin 2 (D0/Rx) on the Nano 33 BLE (i.e., Serial1).

The data will be output on Serial via the USB port. This sketch is a simple example of how you might assign the SBUS data from a remote control drone.

We assign our transmitter controls as follows:

  SBUS Channels: 
  
  - Throttle:     channels[0]
  - Aileron:      channels[1] (Roll)
  - Elevator:     channels[2] (Pitch)
  - Rudder:       channels[3] (Yaw)
  - BUZZER        channels[4]
  - RSSI:         channels[5]
  - Rx Voltage:   channels[6]
  - ARM / DISARM  channels[7]
  - FLIGHT MODE   channels[8]

SBUS channels start at 1 but our `channels[]` array starts at zero. Keep this in mind when parsing the data.

The transmitter channels can be assigned to whatever input you wish. The variables `lostFrame` and `failSafe`, returned by the SBUS class are booleans.

The channel data is returned in an array as a 16 bit integer. 

It is possible to assign a channel to a switch on your remote control transmitter. We have done this for channel 8 (`channels[7]`) in the example (ARM/DISARM).

```c++
if (rxArm >= SWITCH_ON) { Serial.println("ARMED"); }
if (rxArm < SWITCH_ON) { Serial.println("DISARMED"); }
```

The SBUS protocol returns a value between 172 and 1811 on each standard channel. The constant `SWITCH_ON` is halfway between these two values.

### 2. Plot SBUS

This example sketch will plot the first 4 channels of SBUS data to the Arduino IDE Serial Plotter. The SBUS protocol returns a value between 172 and 1811 on each standard channel. The channel data  is returned in an array as a 16 bit unsigned integer. 

The variables `lostFrame` and `failSafe`, returned by the SBUS class are booleans.

This example sketch assumes that inverted SBUS data is being fed to pin 2 (D0/Rx) on the Nano 33 BLE (i.e., Serial1).

This sketch is designed to be used with the Arduino IDE Serial Plotter (`CTRL+SHFT+L`). The vertical (y-axis) on the plotter auto scales and the x-axis is a fixed 500 points with each tick mark indicative of a Serial.println(). With a 20 ms delay you see
10 seconds of data (i.e., 500 x 20 ms).

If you want to stop autoscaling, add the following to loop():

```c++
Serial.println("Min: MIN_SBUS, Max: MAX_SBUS");
```