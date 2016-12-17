// RFM69HCW Example Sketch
// Send serial input characters from one RFM69 node to another
// Based on RFM69 library sample code by Felix Rusu
// http://LowPowerLab.com/contact
// Modified for RFM69HCW by Mike Grusin, 4/16

// This sketch will show you the basics of using an
// RFM69HCW radio module. SparkFun's part numbers are:
// 915MHz: https://www.sparkfun.com/products/12775
// 434MHz: https://www.sparkfun.com/products/12823

// See the hook-up guide for wiring instructions:
// https://learn.sparkfun.com/tutorials/rfm69hcw-hookup-guide

// Uses the RFM69 library by Felix Rusu, LowPowerLab.com
// Original library: https://www.github.com/lowpowerlab/rfm69
// SparkFun repository: https://github.com/sparkfun/RFM69HCW_Breakout

// Include the RFM69 and SPI libraries:

#include <RFM69.h>
#include <SPI.h>

// Addresses for this node. CHANGE THESE FOR EACH NODE!

#define NETWORKID     99   // Must be the same for all nodes
#define MYNODEID      2   // My node ID
#define TONODEID      1   // Destination node ID

// RFM69 frequency, uncomment the frequency of your module:

//#define FREQUENCY   RF69_433MHZ
#define FREQUENCY     RF69_915MHZ

// AES encryption (or not):

#define ENCRYPT       true // Set to "true" to use encryption
#define ENCRYPTKEY    "DR_WHO_TOM_BAKER" // Use the same 16-byte key on all nodes

// Use ACKnowledge when sending messages (or not):

#define USEACK        true // Request ACKs or not

// Packet sent/received indicator LED (optional):

#define MOS           9 // LED positive pin
//#define GND           8 // LED ground pin
unsigned long startTardis = 0; // will store last recv time

// Create a library object for our RFM69HCW module:

RFM69 radio;

void setup()
{
  // Open a serial port so we can send keystrokes to the module:

  Serial.begin(9600);
  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" ready");  

  // Set up the indicator LED (optional):

  pinMode(MOS,OUTPUT);
  digitalWrite(MOS,LOW);
//  pinMode(GND,OUTPUT);
//  digitalWrite(GND,LOW);

  // Initialize the RFM69HCW:
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  // Turn on encryption if desired:

  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);
}

void loop()
{
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    start_tardis();
  }
  // RECEIVING
  // In this section, we'll check with the RFM69HCW to see
  // if it has received any packets:

  if (radio.receiveDone()) // Got one!
  {
    // Print out the information:

    Serial.print("received from node ");
    Serial.print(radio.SENDERID, DEC);
    Serial.print(", message [");

    // The actual message is contained in the DATA array,
    // and is DATALEN bytes in size:

    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);

    // RSSI is the "Receive Signal Strength Indicator",
    // smaller numbers mean higher power.

    Serial.print("], RSSI ");
    Serial.println(radio.RSSI);

    // Send an ACK if requested.
    // (You don't need this code if you're not using ACKs.)

    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.println("ACK sent");
    }
    start_tardis();
  }
  tardis_blink();
}
/**
 * Starts the tardis if it isn't running.
 */
void start_tardis()
{
  unsigned long now = millis();
  unsigned long duration = now - startTardis;
  if (0 == startTardis) {
    startTardis = now;
  }
  else if (duration > 60000) {
    startTardis = now;
  }
}
/**
 * There are 2 tardis functions. 
 * This function switches the power on/off
 */
void tardis_blink() 
{
  unsigned long now = millis();
  unsigned long duration = now - startTardis;
  if (duration > 60000) {
      analogWrite(MOS,0);
  }
  else if (startTardis > 0) {
    analogWrite(MOS,255);
  }
}
/**
 * There are 2 tardis functions. 
 * This function ramps the power on/off
 */
void tardis_breath()
{
  unsigned long now = millis();
  unsigned long duration = now - startTardis;
  if (duration > 60000) {
      analogWrite(MOS,0);
  }
  else if (startTardis > 0) {
    int phase = duration % 4095;
    int level = (phase % 2047) / 8;
//    Serial.print("phase =");
//    Serial.println(phase);
//    Serial.print("level =");
//    Serial.println(level);
    if (phase < 2048) {
      // going up
//      Serial.println("going up");
      analogWrite(MOS, level);
    }
    else {
      // going down
//      Serial .println("going down");
      analogWrite(MOS, 255 - level);
    }
  }
}

