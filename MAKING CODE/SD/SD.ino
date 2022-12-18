#include <SPI.h>

#include <SD.h>


File myFile;
int pinCS =53 ;//Pin 10 on Arduino Uno
void setup() {
    
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  pinMode(9, OUTPUT); 
  if (SD.begin(pinCS))
  
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  
}
void loop() {
  // SD Card Initialization
  
  
  // Create/Open file 
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing to file...");
    // Write to file
    myFile.println("Testing text 1, 2 ,3...");
    myFile.close(); // close the file
    Serial.println("Done.");
    digitalWrite(9, HIGH);
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
  // Reading the file
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("Read:");
    // Reading the whole file
    while (myFile.available()) {
      Serial.write(myFile.read());
   }
    myFile.close();
  }
  else {
    Serial.println("error opening test.txt");
  }
}
