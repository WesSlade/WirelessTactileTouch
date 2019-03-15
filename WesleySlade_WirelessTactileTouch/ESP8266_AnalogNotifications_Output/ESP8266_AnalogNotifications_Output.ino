
/*
  IXD 2020 - Year 3 -  DESN 37900 -
  Interactive Objects and Environments 2: Physical Computing - Winter 2019
  
  All code, circuits, physical devices, and/or presentation is original work
  of Wesley Slade, Carlo Dormiendo and Natelee Hutchinson, unless otherwise referenced.
*/

/*    
 * Analog Input code provided by Tom Igoe.
 * ESP8266 UDP Code Example provided by ESP8266Wifi & WifiUdp library.
 * 
 */
 
/*Haptic Motor Variables
  Short Pulse = 200ms
  Long Pulse = 500ms
  Pause = 2000ms

  Low Strength = 400PWM
  Medium Strength = 600PWM
  High Strength = 1000PWM
*/
/* Motors */
int hapticMotor1 = 12;
int hapticMotor2 = 13;
int ledPin1 = 14;


/* Pauses */
int sPulse = 200;
int lPulse = 800;
int pause = 2000;

/* Controller Variables */
int pager1 = 1;
/*Libaries Used
  ESP82Wifi.h
  Wire.h (In earlier prototypes)
  WifiUDP.h
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/* Wifi setup */

//const char* ssid = "BELL678";
//const char* password = "3A447E2E";

//const char* ssid = "IXD-AA1A-DEV";
//const char* password = "ixd4life!";

const char* ssid = "Bestfriend Me on Linkedin";
const char* password = "wesleyslade";

/* variable to hold the UDP message */
const char* c;


WiFiUDP Udp;
unsigned int localUdpPort = 8888;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back



int motorCode;




void setup()
{

  /* Intializing Wifi connection along with Serial confirmation */
  Serial.begin(9600);
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  /* Haptic Pin setup */
  pinMode(hapticMotor1, OUTPUT);
  pinMode(hapticMotor2, OUTPUT);
  pinMode(ledPin1, OUTPUT);
}


void loop()
{

  /* UDP Communcation
     Modified example code
  */
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    c = incomingPacket;

    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
    Serial.println(Udp.remotePort());
    Serial.print(Udp.remoteIP());

    /*
       ATOI is a native C function that translates ASCII to an integer. ToInt is the proper syntax in Arduino.
       This function allows the ESP controlling the haptic motor to translate the UDP message it recieves to a simple to use integer
    */
    motorCode = atoi(c);

    /* Checking to make sure the motorCode is correct for debugging */
    Serial.print(motorCode);

    /*
       Earlier versions of this code used a Switch statement instead of If Statements. Either would work in this circumstance.

       This code runs a for loop based on the force measured on the other ESP. The For Loop runs 3 times to turn on the haptic motor(s) and associated LED.
    */

    if (motorCode == 1) {
      for (int i = 0; i < 3; i++) {
        analogWrite(hapticMotor1, 400);
        delay(sPulse);
        analogWrite(hapticMotor1, 0);
        delay(sPulse);
        Serial.println("Low Power");

      }
    } else if (motorCode == 2) {
      for (int i = 0; i < 3; i++) {
        analogWrite(hapticMotor1, 600);
        analogWrite(ledPin1, 400);
        delay(lPulse);
        analogWrite(hapticMotor1, 0);
        analogWrite(ledPin1, 0);
        delay(lPulse);
        Serial.println("Medium Power");

      }

    } else if (motorCode == 3) {
      for (int i = 0; i < 3; i++) {
        analogWrite(hapticMotor1, 1000);
        analogWrite(hapticMotor2, 1000);
        analogWrite(ledPin1, 1000);
        delay(lPulse);
        analogWrite(hapticMotor1, 0);
        analogWrite(hapticMotor2, 0);
        analogWrite(ledPin1, 0);
        delay(lPulse);
        Serial.println("High Power");

      }

    }
  }
}
