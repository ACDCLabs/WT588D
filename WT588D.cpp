// Simple library for using the WT588D soundchip breakout board
// The WT588D can play sampled sounds stored in a flash memory on the breakoit board
// The WT588D supports various operation modes to trigger playback of sound files
// The operartion mode used is also stored in the onboard flash and can only be altered by
// flasing the memory.
// Currently only the three-line-serial mode is supported in this library
// The cuurent veriosn of the library only supports the WT588D-16P breakout board with two times eight
// PINs
// written by Norbert Danneberg; Thanks for inspiration and initial code sample to
// "SugarBombs" on the arduino forum
// MIT license.

#include "WT588D.h"


// definition of the commands te WT588D understands
#define WT588D_STARTLOOPPLAY    0xF2
#define WT588D_STOPLOOPPLAY     0xFE
#define WT588D_IOEXTON          0xF5
#define WT588D_IOEXTOFF         0xF6
#define WT588D_MIN_VOLUME       0xE0
#define WT588D_MAX_VOLUME       0xE7

#define MAX_NUM_OF_PLAYLISTS 220

// Constructor
// set the correct pins that connects the WT588D to the arduino when
// calling the constructor
WT588D::WT588D(uint8_t resetPin, uint8_t chipSelectPin,
               uint8_t serialClockPin, uint8_t serialDataPin, uint8_t busyPin) {
    rstPin = resetPin;
    csPin = chipSelectPin;
    sclPin = serialClockPin;
    sdaPin = serialDataPin;
    bsyPin = busyPin;
}

// initialze pins
void WT588D::begin(void) {

    pinMode(rstPin, OUTPUT);
    pinMode(csPin, OUTPUT);
    pinMode(sclPin, OUTPUT);
    pinMode(sdaPin, OUTPUT);
    pinMode(bsyPin, INPUT);
    
    // direct port access probably not necessary since
    // timing is noct critical when sending commands
    csport      = portOutputRegister(digitalPinToPort(csPin));
    cspinmask   = digitalPinToBitMask(csPin);
    
    sclport      = portOutputRegister(digitalPinToPort(sclPin));
    sclpinmask   = digitalPinToBitMask(sclPin);

    sdaport      = portOutputRegister(digitalPinToPort(sdaPin));
    sdapinmask   = digitalPinToBitMask(sdaPin);

    bsyport      = portInputRegister(digitalPinToPort(bsyPin));
    bsypinmask   = digitalPinToBitMask(bsyPin);

    
    *csport |= cspinmask; // set to high - active low
    *sclport |= sclpinmask; // set to high - active low
    *sdaport |= sdapinmask; // set to high - active low
    
    digitalWrite(rstPin, HIGH);  // active low rst

}

void WT588D::sendCommand(uint8_t command){
    
    // a 5ms reset indicates start of new command
    digitalWrite(rstPin, LOW);
    delay(5);
    digitalWrite(rstPin, HIGH);
    
    delay(17); // wait 17 ms according to datasheet
    *csport &= ~cspinmask; // pull chip select low
    delay(2); // wait 2 ms according to datasheet
    
    for(uint8_t  i = 0; i < 8; i++)  {
        *sclport &= ~sclpinmask; // start clock cycle
        // digitalWrite(WT588D_SCL, LOW);
        if(bitRead(command, i)) *sdaport |= sdapinmask; // digitalWrite(WT588D_SDA, HIGH);
        else *sdaport &= ~sdapinmask; // digitalWrite(WT588D_SDA, LOW);
        delay(2);   // wait 2 ms according to datasheet
        *sclport |= sclpinmask;
        //delayMicroseconds(50);
        delay(2);   // wait 2 ms according to datasheet
    } //end for
    delay(1);
    
    // digitalWrite(WT588D_CS, HIGH);
    
    *csport |= cspinmask;  // release the chip select
    
}


// plays the playlist with in number give as arguement
// the chip supports 220 playlists
// The chips starts playing by simple receivung the number of
// the playlist from 0 to 219
void WT588D::playSound(uint8_t playListNumber){
    
    if (playListNumber>MAX_NUM_OF_PLAYLISTS)
        return;
    else {
        sendCommand(playListNumber);
        return;
    }
}

// sets the sound volume
void WT588D::setVolume(uint8_t volume) {
    
    if (volume<= WT588D_MAX_VOLUME && volume >= WT588D_MIN_VOLUME)
        sendCommand (volume);
    
    return;
}

// starts continiously looping the sound that is currently played
void WT588D::startLoopSound() {
    
    sendCommand (WT588D_STARTLOOPPLAY);
    
    return;
}

// stops continiously looping the sound that is currently played
void WT588D::stopLoopSound() {
    
    sendCommand (WT588D_STOPLOOPPLAY);
    
    return;
}


// waits while the busy signal is low or timeout
// returns true if the busy signal is high
// returns false if the busy is longer low than timeout
boolean WT588D::waitForBusy(uint32_t timeout) {

    uint32_t startTime = millis();
    
    while (((*bsyport & bsypinmask) == 0)) {
        if((millis() - startTime) > timeout) return false;
        
    }
    return true;
}

// checks the busy siganl that indicates that currently a
// sound is playing
// returns true if the busy signal is low
// returns false if the busy is high
boolean WT588D::isBusy(void) {
    // Serial.println(*bsyport ,BIN);
    return (( (*bsyport & bsypinmask) == 0) ? true : false);
}



