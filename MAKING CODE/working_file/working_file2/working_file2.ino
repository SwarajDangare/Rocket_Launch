#include <SimpleDHT.h>                                            // HUMITITY & TEMPRATURE
int dhtpin = 5;
SimpleDHT11 dht11; 

//----------------------------------------------------------------

#include <Wire.h>
#include <SD.h>                
#include <SPI.h>                                                  // SD CARD
int chipSelect = 53;
File mySensorData; 

//----------------------------------------------------------------

#include <Adafruit_BMP085.h>                                      // PRESSURE & ALTITUDE
Adafruit_BMP085 bmp;

//----------------------------------------------------------------

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 10, TXPin = 11;
static const uint32_t GPSBaud = 9600;                             // GPS & Seraial Port
TinyGPSPlus gps;
SoftwareSerial mySerial(RXPin, TXPin); 
byte Hour = gps.time.hour()+5;           
byte Min = gps.time.minute()+30; // For converting UST to IST                             
//----------------------------------------------------------------



void setup() {
  
   Serial.begin(9600);                               //Turn on serial monitor
   Serial.println("\t\t\tLAUNCHEDPAD");                    
   Serial.println(" ");                              
   Serial.println(" ");
  //..............................................   
  mySerial.begin(GPSBaud);                           //Turn on serial for GPS
  //..............................................   
  bmp.begin();                                       //Turn on BMP180 sensor
  //..............................................   
  SPI.begin();                                       //Turn on SPI library
  //..............................................   
  SD.begin(chipSelect);                              //Initialize the SD card reader  
  delay(500);                                        
  //..............................................   
  if (SD.exists("GPSData.txt"))                      
  {                                                  
      SD.remove("GPSData.txt");                      //Delete old GPSData files to start fresh
  }                                                  
  if (SD.exists("HTPA.txt"))                         
  {                                                  
      SD.remove("HTPA.txt");                         //Delete old HTPA files to start fresh
  }                                                  
  delay(500);                                        
  //..............................................   
  pinMode(9, OUTPUT);                                //Declare output for LED(GPSData.TXT file)
  pinMode(8, OUTPUT);                                //Declare output for LED(HTPA.TXT file)
  //..............................................   
}

void loop()  {
  
  while (mySerial.available() > 0)                   
                                                     
  if (gps.encode(mySerial.read()))                   // This sketch displays information every time a new sentence is correctly encoded.
      displayGPSInfo();   
                                
                                                     
  if (millis() > 5000 && gps.charsProcessed() < 10) 
  {                                                  
    Serial.println("No GPS detected: check wiring.");
    while(true);                                     
  }                                                  
                                                      
      displayHTPAData();
                                    
 }

void displayGPSInfo()
{
    
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
  }
  else
  {
    Serial.print(" Time INVALID");
  }

  Serial.println();
 //..........................................  
  Serial.print("   Location: ");
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 4);
    Serial.print(",");
    Serial.print(gps.location.lng(), 4);
    Serial.print(",");    
  }
  else
  {
    Serial.print(" location INVALID ");
  }
    Serial.print(bmp.readAltitude(),4);
    Serial.println();
  
//------------------------------------------------------------------------------------  

 mySensorData = SD.open("GPSData.txt", FILE_WRITE);
 
  if (mySensorData)
  {
  mySensorData.print(gps.location.lat(), 4); 
  mySensorData.print(",");
   
  mySensorData.print(gps.location.lng(), 4);
  mySensorData.print(",");
  
  mySensorData.print(bmp.readAltitude(),4);
  mySensorData.print(" ");  
     
  mySensorData.close();
  digitalWrite(9,HIGH);
  }
   else
  {
    Serial.println("error opening GPSData.txt");
  }

}



void displayHTPAData(){

byte temperature = 0;
byte humidity = 0;
int   err = SimpleDHTErrSuccess;

 if((err = dht11.read(dhtpin, &temperature,&humidity, NULL)) != SimpleDHTErrSuccess){

  return;
  }

Serial.print("Current Temperature = ");
Serial.print((int)temperature);
Serial.print("'C\t");

Serial.print("Current Humidity = ");
Serial.print((int)humidity);
Serial.print("%\t");

Serial.print(" Air Pressure = ");
Serial.print(bmp.readPressure());
Serial.print("Pa\t");

Serial.print(" Altitude = ");
Serial.print(bmp.readAltitude(),4);
Serial.println("m");
Serial.println();



mySensorData = SD.open("HTPA.txt", FILE_WRITE);
  
  
  if (mySensorData)                              // if the file opened okay, write to it
  {
    mySensorData.print((int)temperature);
    mySensorData.print(",");
    mySensorData.print((int)humidity);
    mySensorData.print(',');
    mySensorData.print(bmp.readPressure());
    mySensorData.print(",");
    mySensorData.print(bmp.readAltitude());
    mySensorData.print(",");
      
     mySensorData.print(gps.date.day());
     mySensorData.print("/");
     mySensorData.print(gps.date.month());
     mySensorData.print("/");
     mySensorData.print(gps.date.year());
     mySensorData.print(",");
       mySensorData.print("0");
       mySensorData.print(gps.time.hour());
       mySensorData.print(":");
       mySensorData.print("0");
       mySensorData.print(gps.time.minute());
       mySensorData.print(":");
       mySensorData.print("0");
       mySensorData.println(gps.time.second());
      
    mySensorData.close(); // close the file
    digitalWrite(8,HIGH);
  }
  else                                           // if the file didn't open, print an error
  {
    Serial.println("error opening H,T,P,A.txt");  
  }


}
