#include "WT588D.h"


// set the correct pin connections for the WT588D chip
#define WT588D_RST 7  //Module pin "REST" or pin # 1
#define WT588D_CS 6   //Module pin "P02" or pin # 11
#define WT588D_SCL 9  //Module pin "P03" or pin # 10
#define WT588D_SDA 8  //Module pin "P01" or pin # 12
#define WT588D_BUSY 5 //Module pin "LED/BUSY" or pin # 15

// define playlistnumbers for convenience
// these playlistnumbers have been set in the flashing tool "WT588D VoiceChip Beta 1.6.exe"
// and downloaded into the flash memory
#define SABER_ON 0
#define SABER_OFF 1
#define HUM_NO_MOVE_2 2
#define HUM_NO_MOVE 3
#define HUM_SLOW_MOVE 4
#define HUM_FAST_MOVE 5
#define HUM_CLASH 6
#define CLASH 7
#define CLASH_2 8
#define SABER_ON_2 9



WT588D myWT588D(WT588D_RST, WT588D_CS, WT588D_SCL, WT588D_SDA, WT588D_BUSY);
boolean saberIsOn = false;

void setup() {

  // Serial.begin(57600);  // debugging
  
  // initialize the chip and port mappiing
  myWT588D.begin();
}

void loop()
{

  if (!saberIsOn) {

    saberIsOn = true;

    if ( ! myWT588D.isBusy() ) {
      myWT588D.playSound(SABER_ON);
      delay(100);
    }

  }

  // loopOnAllSounds();
  if (saberIsOn ) {
    if (! myWT588D.isBusy() ) {
      myWT588D.playSound(HUM_NO_MOVE);
      delay(100);
      
    }
  }

}

void loopOnAllSounds(void) {

  for (int playListNumber = 0; playListNumber < 9; playListNumber ++) {

    myWT588D.playSound(playListNumber);

    delay(100); //give the module time to start playing
    Serial.print("Playing: ");
    Serial.println(playListNumber);
    while (myWT588D.isBusy()) {
      /*Serial.println("WT588D busy");*/
    }

    delay(200);
  }
}


