#ifndef WaterTempTransmitter_h
#define WaterTempTransmitter_h

#include <Arduino.h>

#define WTT_PULSEDURATION 500
#define WTT_ZERO 2000
#define WTT_ONE 4500
#define WTT_PREAMBLE 9500
#define WTT_BITS 28
#define WTT_REPETITIONS 8

class WaterTempTransmitter {
public:
    // pin is the pin connected to the data of the 433 transmitter
    // id is an 8 bit identifier (you can create your own)
    // The channel which can be (1,2,3)
    WaterTempTransmitter(byte pin, byte id, byte channel);

    void send(float temperature, bool battery=false, bool beep=false);

protected:
    void sendPacket(byte pin, unsigned long packet, byte nbits);
    void sendFrame(byte pin, unsigned long packet, byte nbits, byte repetitions);
protected:
    byte _pin; 
    byte _id; 
    byte _chan;
};

#endif

