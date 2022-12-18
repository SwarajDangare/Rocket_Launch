#include <SD.h> //Load SD card library
#include<SPI.h> //Load SPI Library

#include <TinyGPS++.h>
 //Create the GPS Object

#include <SoftwareSerial.h>
static const int RXPin = 10, TXPin = 11;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial mySerial(RXPin, TXPin); //Initialize the Software Serial port

int chipSelect = 53; //chipSelect pin for the SD card Reader
File mySensorData; //Data object you will write your sesnor data to


void setup() {
  
  Serial.begin(9600); //Turn on serial monitor
  
  mySerial.begin(GPSBaud);
  
  SPI.begin();
  
   pinMode(9, OUTPUT);

  SD.begin(chipSelect); //Initialize the SD card reader
  
 Serial.println("LAUNCHEDPAD");
 
  if (SD.exists("GPSData.txt")) { //Delete old data files to start fresh
    SD.remove("GPSData.txt");
  }

}

void loop()  {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (mySerial.available() > 0)
    if (gps.encode(mySerial.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected: check wiring.");
    while(true);
  } 
 }

void displayInfo()
{
  Serial.print("  Location: ");
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 4);
    Serial.print(",");
    Serial.print(gps.location.lng(), 4);
    Serial.print(",");
    
  }
  else
  {
    Serial.print(" location INVALID");
  }
  if (gps.altitude.isValid())
  {
    Serial.print(gps.altitude.meters(), 4);
    Serial.print(" ");
  }
  else
  {
    Serial.print(" altitude INVALID");
  }

  Serial.print("  Date/Time: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print("Date INVALID");
  }

  Serial.print(" ");
  if (gps.time.isValid())
  {
    if (gps.time.hour()<0)
    Serial.print("0");
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute()<0)
    Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second()<0)
    Serial.print("0");
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond()<0)
    Serial.print("0");
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(" Time INVALID");
  }

  Serial.println();

   mySensorData = SD.open("GPSData.txt", FILE_WRITE);
 if (mySensorData)
  {
  mySensorData.print(gps.location.lat(), 4); //writing decimal degree longitude value to SD card
  mySensorData.print(",");
   
  mySensorData.print(gps.location.lng(), 4);
  mySensorData.print(","); //write comma to SD card
  
  mySensorData.print(gps.altitude.meters(), 4); //write altitude to file
  mySensorData.print(" ");  //format with one white space to delimit data sets

  mySensorData.close();
  digitalWrite(9,HIGH);
  }
}
