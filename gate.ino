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
#define MYNODEID      1   // My node ID
#define TONODEID      2   // Destination node ID

// RFM69 frequency, uncomment the frequency of your module:

//#define FREQUENCY   RF69_433MHZ
#define FREQUENCY     RF69_915MHZ

// AES encryption (or not):

#define ENCRYPT       true // Set to "true" to use encryption
#define ENCRYPTKEY    "DR_WHO_TOM_BAKER" // Use the same 16-byte key on all nodes

// Use ACKnowledge when sending messages (or not):

#define USEACK        true // Request ACKs or not

// Create a library object for our RFM69HCW module:

RFM69 radio;

const byte interruptPin = 3;
volatile unsigned long interrupted = 0;
unsigned long last = 0;

void setup()
{
  // Open a serial port so we can send keystrokes to the module:
  Serial.begin(9600);
  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" ready");  

  // Initialize the RFM69HCW:
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  // Turn on encryption if desired:
  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);

  // setup gate interupt pin
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), start_send, RISING);
}

void loop()
{
  // Set up a "buffer" for characters that we'll send:

  static char sendbuffer[62];
  static int sendlength = 0;
  static int prev_int = 0;
  // SENDING

  // In this section, we'll gather serial characters and
  // send them to the other node if we (1) get a carriage return,
  // or (2) the buffer is full (61 characters).

  // If there is any serial input, add it to the buffer:

  if (Serial.available() > 0)
  {
    char input = Serial.read();

    if (input != '\r') // not a carriage return
    {
      sendbuffer[sendlength] = input;
      sendlength++;
    }

    // If the input is a carriage return, or the buffer is full:

    if ((input == '\r') || (sendlength == 61)) // CR or buffer full
    {
      // Send the packet!


      Serial.print("sending to node ");
      Serial.print(TONODEID, DEC);
      Serial.print(", message [");
      for (byte i = 0; i < sendlength; i++)
        Serial.print(sendbuffer[i]);
      Serial.println("]");
      sendRadio(sendbuffer, sendlength);
    }
  }
  else if (interrupted - last >  1000) {
    String sendstring = "engage!";
    sendRadio(sendstring.c_str(), sendstring.length());
    last = interrupted;
    prev_int++;
    Serial.print("interrupt number: ");
    Serial.print(prev_int, DEC);
    Serial.println();
  }
}
/**
 * Interrupt service routing called on rising edge of the gate button press
 */
void start_send() {
  //Serial.println("start_send");
  interrupted = millis();
}
/**
 * Send radio sends the buffer to the radio
 * @param sendbuffer The data to send
 * @param sendlentgh The amount of data in the buffer
 */
void sendRadio(const char *sendbuffer, int sendlength) {
  if (sendlength > 62) sendlength = 62;

  // There are two ways to send packets. If you want
  // acknowledgements, use sendWithRetry():
  if (USEACK)
  {
    if (radio.sendWithRetry(TONODEID, sendbuffer, sendlength))
      Serial.println("ACK received!");
    else
      Serial.println("no ACK received");
  }
  else // don't use ACK
  {
    radio.send(TONODEID, sendbuffer, sendlength);
  }
  sendlength = 0; // reset the packet
}

