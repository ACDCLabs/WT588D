#ifndef _WT588D_H_
#define _WT588D_H_

#include <Arduino.h>

#ifdef __SAM3X8E__
typedef volatile RwReg PortReg;
typedef uint32_t PortMask;
#else
typedef volatile uint8_t PortReg;
typedef uint8_t PortMask;
#endif


class WT588D {
    
 public:
    WT588D(uint8_t resetPin, uint8_t chipSelectPin, uint8_t serialClockPin,
           uint8_t serialDataPin, uint8_t busyPin);
    
    void  begin(void);
    
    void    setVolume(uint8_t volume);
    void    playSound(uint8_t playListNumber);
    void    startLoopSound(void);
    void    stopLoopSound(void);
    boolean  isBusy(void);
    
 private:
    int8_t rstPin, csPin, sclPin, sdaPin, bsyPin; // pin numbers
    PortReg *csport, *sclport, *sdaport, *bsyport;
    PortMask cspinmask, sclpinmask, sdapinmask, bsypinmask;

    boolean  waitForBusy(uint32_t timeout = 200L);
    void     sendCommand(uint8_t cmd);
};

#endif // _WT588D_H_
