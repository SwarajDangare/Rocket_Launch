#include <SimpleDHT.h>         // HUMITITY & TEMPRATURE
int dhtpin = 5;
SimpleDHT11 dht11; 

//.................................................................

#include <SoftwareSerial.h>    // Load the Software Serial library
SoftwareSerial mySerial(0,1);

//.................................................................

#include <Adafruit_BMP085.h>   // PRESSURE & ALTITUDE
Adafruit_BMP085 bmp;

//.................................................................

#include <Wire.h>
#include <SD.h>                
#include <SPI.h>               // SD CARD
File myFile;
int pinCS = 4 ;

//.................................................................

void setup() {
  Serial.begin(9600);
  
  bmp.begin();

  
 pinMode(53, OUTPUT);
  pinMode(pinCS, OUTPUT);
  SD.begin(pinCS);

  delay(1000);
  if (SD.exists("HTPA.txt")) { //Delete old data files to start fresh
    SD.remove("HTPA.txt");
  }
  
}

void loop() {

  delay(1000);
  
byte temperature = 0;
byte humidity = 0;
int   err = SimpleDHTErrSuccess;




if((err = dht11.read(dhtpin, &temperature,&humidity, NULL)) != SimpleDHTErrSuccess){

return;
}
Serial.print("Current Temperature = ");
Serial.print((int)temperature);
Serial.print("'C\t\t");

Serial.print("Current Humidity = ");
Serial.print((int)humidity);
Serial.print("%\t\t");

Serial.print("Air Pressure = ");
Serial.print(bmp.readPressure());
Serial.print("Pa\t\t");

Serial.print("Altitude = ");
Serial.print(bmp.readAltitude());
Serial.println("m");

Serial.println();


myFile = SD.open("HTPA.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    // Write to file
    myFile.print((int)temperature);
    myFile.print(",");
    myFile.print((int)humidity);
    myFile.print(',');
    myFile.print(bmp.readPressure());
    myFile.print(",");
    myFile.println(bmp.readAltitude());
    myFile.close(); // close the file
  }
   // if the file didn't open, print an error:
  else {
    Serial.println("error opening H,T,P,A.txt");
  }


}
