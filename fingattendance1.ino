/* FISAT FIngerprint Attendance System v1.0
 *  Sample Code developed by Aravind Jayan for the sensor front of the system
 *  Developed for Federal Labz
 *  contact: aravind@mak3r.space
 */

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"
#include <Time.h>

    
RTC_DS1307 rtc;


// pin #2 is TX from sensor (BLACK wire)
// pin #3 is RX from arduino  (RED wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


void timex () {
  rtc.begin();
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    
    Serial.println();
}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      digitalWrite(9,HIGH);
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      digitalWrite(8,HIGH);
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      digitalWrite(8,HIGH);
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      digitalWrite(8,HIGH);
      //Serial.println("Imaging error");
      return p;
    default:
      digitalWrite(8,HIGH);
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
    digitalWrite(9,HIGH);
      //Serial.println("Image converted");
      delay(100);
      break;
    case FINGERPRINT_IMAGEMESS:
      digitalWrite(8,HIGH);
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    digitalWrite(8,HIGH);
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      digitalWrite(8,HIGH);
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      digitalWrite(8,HIGH);
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      digitalWrite(8,HIGH);
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    digitalWrite(8,HIGH);
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    digitalWrite(8,HIGH);
    return p;
  } else {
    //Serial.println("Unknown error");
    digitalWrite(8,HIGH);
    return p;
  }   
  
  // found a match!

        Serial.print("#"); 
        Serial.print(finger.fingerID); 
        Serial.println();
        digitalWrite(10,HIGH);
        timex();
        delay(500);
}


void setup()  
{
  pinMode(8,OUTPUT);     //RED LED
  pinMode(9,OUTPUT);     //ORANGE LED
  pinMode(10,OUTPUT);    //GREEN LED
  while (!Serial);
  Serial.begin(9600);
  Serial.println("FISAT® FingerPrint Attendance System v1.0");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  
  getFingerprintID();
  
  delay(50);            //don't ned to run this at full speed.
}
