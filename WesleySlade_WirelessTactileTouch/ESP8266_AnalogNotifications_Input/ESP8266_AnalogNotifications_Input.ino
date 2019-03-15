

/*
  IXD 2020 - Year 3 -  DESN 37900 - 
  Interactive Objects and Environments 2: Physical Computing - Winter 2019
  
  All code, circuits, physical devices ,and/or presentation is original work 
  of Wesley Slade, Carlo Dormiendo and Natelee Hutchinson, unless otherwise referenced.
*/

/*    
 * Analog Input code provided by Tom Igoe.
 * ESP8266 UDP Code Example provided by ESP8266Wifi & WifiUdp library.
 * 
 */

int sensorPin = A0;    // AnalogRead pin for ESP
int sensorValue = 0;  //Variable for Force Sensitive Resistor
int sensorPing[2];   //Variable for FSR polling/averaging

//Packets to send over UDP, triggering different patterns/intensities of tactile feedback 

char  hapticPacket1[] = "1";
char  hapticPacket2[] = "2";
char  hapticPacket3[] = "3";


/*Libaries Used
  ESP82Wifi.h
  Wire.h (In earlier prototypes)
  WifiUDP.h
*/
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


/* Wifi setup
*/

//const char* ssid = "BELL678";
//const char* password = "3A447E2E";

//const char* ssid = "IXD-AA1A-DEV";
//const char* password = "ixd4life!";

const char* ssid = "Bestfriend Me on Linkedin";
const char* password = "wesleyslade";


//192.168.43.113
IPAddress remote (192, 168, 43, 19);
int espPort = 8888;


/* UDP setup
 * Some comments from UDP example left for clarity
*/

WiFiUDP Udp;
unsigned int localUdpPort = 8888;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);

/*
 * Wifi Connection setup from Wifi/UDP examples included in libraries
 */
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);

  /*
   * Pause for 0.5 seconds to allow full feedback loop to allow for slow networks
   */
  delay(500);

  sensorPing[0] = 0;
  sensorPing[1] = 0;
  sensorPing[2] = 0;

/*
 *If FSR is touched, average out the analog input over 0.6 seconds and write it to the array 
 */

  if (sensorValue >= 50) {

    for (int i = 0; i < 3; i++)
    {
      sensorPing[i] = analogRead(sensorPin);
      delay(200);
      Serial.println("test");
    }

/*
 * Averaging out the data
 */
    sensorValue = sensorPing[0] += sensorPing[1] += sensorPing[2] /= 3;

/*
 * Take the averaged data and see if it meets threshholds from highest to lowest.
 * If it does meet a threshhold, Send a UDP packet to other device that activates haptics.
 */

    if (sensorValue >= 800) {
      Udp.beginPacket(remote, espPort);
      Serial.print("HapticThreshhold3Active");
      Udp.write(hapticPacket3);
      Udp.endPacket();
    } else if (sensorValue >= 400) {
      Udp.beginPacket(remote, espPort);
      Serial.print("HapticThreshhold2Active");
      Udp.write(hapticPacket2);
      Udp.endPacket();
    } else if (sensorValue >= 100) {
      Udp.beginPacket(remote, espPort);
      Serial.print("HapticThreshhold1Active");
      Udp.write(hapticPacket1);
      Udp.endPacket();

    } else {
/*
 * If no sensor value is detected, return to the top of the loop and start again.
 */
      
      return;
    }
/*
 * If UDP packet was activated, pause for another 0.5 seconds to allow for slow networks
 */  
    delay(500);


  }

}
