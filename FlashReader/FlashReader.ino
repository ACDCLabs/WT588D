// Read out the flash memory of the WT588D chip for test purposes 

// Arduino PIN  - WT588D-16P Board pin  - function 
// --------------------------------------------------
// 10           - 9                     - CS chip select 
// 11           - 5                     - DI Data in 
// 12           - 6                     - DO Data out 
// 13           - 7                     - CLK Clock 


#include <Adafruit_TinyFlash.h>
#include <SPI.h>


int bytesToRead = 1024*10;
byte receivedData;
Adafruit_TinyFlash flash;
void setup() {
 
  pinMode(7, OUTPUT); // reset Pin WT588D
  digitalWrite(7,LOW); // Pull down reset during readout
  Serial.begin(57600);
  int capacity = flash.begin();
    Serial.println("--SOT--");   

  if (!flash.beginRead(0)) Serial.println("Error reading");  //  start at the lowest address

  for (int i =0; i < bytesToRead; i++) {
    receivedData = flash.readNextByte();
    Serial.print(receivedData, HEX);
    Serial.print(" ");
    if ((i+1)%16 ==0) {Serial.println("---"); Serial.print(i+1,HEX); Serial.print(": ");}
  }

  flash.endRead(); // CS high
  Serial.println("--EOT--"); 
 
}

void loop() {


}
