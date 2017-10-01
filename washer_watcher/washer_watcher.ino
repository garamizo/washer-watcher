
#define MPU_addr 0x68
#define TO_CLOUD

#include <Wire.h>

#ifdef TO_CLOUD
#include "ThingSpeak.h"

//#define USE_WIFI101_SHIELD
//#define USE_ETHERNET_SHIELD

#if defined(ARDUINO_AVR_YUN)
    #include "YunClient.h"
    YunClient client;
#else
  #if defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_ARCH_ESP8266)
    // Use WiFi
    #ifdef ARDUINO_ARCH_ESP8266
      #include <ESP8266WiFi.h>
    #else
      #include <SPI.h>
      #include <WiFi101.h>
    #endif
    char ssid[] = "Galaxy S5 5664";    //  your network SSID (name) 
    char pass[] = "idbt3270";   // your network password
    int status = WL_IDLE_STATUS;
    WiFiClient  client;
  #elif defined(USE_ETHERNET_SHIELD)
    // Use wired ethernet shield
    #include <SPI.h>
    #include <Ethernet.h>
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    EthernetClient client;
  #endif
#endif

unsigned long myChannelNumber = 315859;
const char * myWriteAPIKey = "AEHJ15QUUHUQADLU";
#endif

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  #ifdef TO_CLOUD
  #ifdef ARDUINO_AVR_YUN
    Bridge.begin();
  #else   
    #if defined(ARDUINO_ARCH_ESP8266) || defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000)
      WiFi.begin(ssid, pass);
    #else
      Ethernet.begin(mac);
    #endif
  #endif

  ThingSpeak.begin(client);
  #endif

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  Serial.begin(9600);

  delay(1000);

}

float read_acceleration() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);  // request a total of 14 registers
  
  int16_t AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  int16_t AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  int16_t AcZ = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  return( 9.81 * sqrt(AcX*AcX + AcY*AcY + AcZ*AcZ) / 2048.0 );
}


void loop() {
  static long int t1 = millis();
  static long int t2 = millis();

  if(millis() > t1) {
    t1 += 20000;

    float vmax = 0, vmin = 1e3;
    for(int k = 0; k < 100; k++) {
      float acc_abs = read_acceleration(); // g/sec^2

      vmax = acc_abs > vmax ? acc_abs : vmax;
      vmin = acc_abs < vmin ? acc_abs : vmin;
      
      delay(10);
    }
    
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    #ifdef TO_CLOUD
    ThingSpeak.writeField(myChannelNumber, 1, vmax-vmin, myWriteAPIKey);
    #else
    Serial.println(vmax-vmin);
    #endif
  }

  if(millis() > t2) {
    t2 += 500;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}



