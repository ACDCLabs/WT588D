// Winbond serial flash loader.  Runs on normal Arduino (e.g. Uno,
// NOT Trinket), works with 'AudioXfer' Processing code on host PC.
// Modified by purgedsoul for uploading bin file to WTD588-16P sound module.
// Use at your own risk!

#include <Adafruit_TinyFlash.h>
#include <SPI.h>

//#define LED A0

// Blink LED (not pin 13) to indicate flash error
/*void error(uint32_t d) {
  for(;;) {
    digitalWrite(LED, HIGH);
    delay(d);
    digitalWrite(LED, LOW);
    delay(d);
  }
}
*/

void setup() {
  Adafruit_TinyFlash flash;
  
  uint8_t            buffer[256];
  
  int                c, index      = 0, 
                     rmdr          = 0,
                     numpages      = 0,
                     page          = 0;
                     
  uint32_t           capacity,
                     address       = 0,
                     bytesExpected = 0x7FFFFFFF,
                     bytesReceived = 0,
                     filesize=0;

//  pinMode(LED, OUTPUT);
//  digitalWrite(LED, LOW);

  Serial.begin(57600);
  Serial.println("HELLO");      // ACK to host

  capacity = flash.begin();
  Serial.println(capacity);     // Chip size to host
//  if(!capacity) error(250);     // Fast blink

  

  while(!Serial.find("ERASE")); // Wait for ERASE command from host
  if(!flash.eraseChip()) {
    Serial.println("ERROR");
//    error(500);                 // Slow blink
  }
  
//while(Serial.available() == 0) {}
   while(!Serial.find("CAPACITY")); 
    filesize = Serial.parseInt();
  
    numpages = filesize / 256;
    rmdr = filesize % 256;
    
    if(rmdr != 0) numpages = numpages + 1;
    
  Serial.println("READY");      // ACK to host

  for(;;) {
    // Buffer data until a full page is ready or last packet arrives.
    if((c = Serial.read()) >= 0) {
      buffer[index] = c;
      index=index+1;
      if((index >= sizeof(buffer))) {
        if(flash.writePage(address, buffer)) {
          
//          digitalWrite(LED, HIGH);
          Serial.print('.');
        } else {
          Serial.print('X');
        }
        
//        digitalWrite(LED, LOW);
        if(page >= numpages) break; //(address >= capacity) || 
        index = 0;
        page = page + 1;
        address = address + sizeof(buffer);
      }//if ((index >= sizeof(buffer)))
    } //if ((c = Serial.read()) >= 0)
  } //for(;;)
  //if filesize > 0
} // void setup()

void loop() { }

