/************
 * WaterTempTransmitter
 *
 * Author: Abdullah Tahiri ( http://alejandro-anv.blogspot.cz/2012/12/hackeando-termometro-inalambrico.html )
 * Code taken from comment at http://dephiox.blogspot.cz/2012/12/i-recently-stumbeled-over-433-mhz.html
 ************/

#include "WaterTempTransmitter.h"

WaterTempTransmitter::WaterTempTransmitter(byte pin, byte id, byte channel)
{
    _pin=pin;
    _id=id;
    _chan=(channel & 0x03); // it is just two bits (never trust the user ! ;-P)
    pinMode(_pin, OUTPUT);
}

void WaterTempTransmitter::send(float temperature, bool battery, bool beep)
{
    int16_t temp= (int16_t)(temperature * 10);
    unsigned long packet= _id ;
    packet<<= 12;
    packet|= (temp & 0x8000)==0x8000?((temp & 0x07FF) | 0x0800):(temp & 0x07FF);
    packet<<= 2;
    packet|=_chan;
    packet<<=2;
    packet|= (battery==false)?0x2:00;
    packet|= (beep==true)?0x1:00;

    // Checksum (summatory of nibbles +1)
    unsigned long temppacket=packet;
    byte checksum= (byte)temppacket;
    for(byte i=1;i<7;i++)
        checksum+=(temppacket >>= 4);
    checksum-=1;

    temppacket=checksum; // variable reuse
    temppacket<<= 24;
    packet|=temppacket;

    sendFrame(_pin, packet, WTT_BITS, WTT_REPETITIONS);
}

void WaterTempTransmitter::sendPacket(byte pin, unsigned long packet, byte nbits)
{
    //send preamble
    digitalWrite(pin, HIGH);
    delayMicroseconds(WTT_PULSEDURATION);
    digitalWrite(pin, LOW);
    delayMicroseconds(WTT_PREAMBLE);

    for(int8_t i=(nbits-1);i>=0;i--)
    {
        if( (packet >> i) & 0x00000001)
        {// send 1
            digitalWrite(pin, HIGH);
            delayMicroseconds(WTT_PULSEDURATION);
            digitalWrite(pin, LOW);
            delayMicroseconds(WTT_ONE);
        } 
        else
        {// send 0
            digitalWrite(pin, HIGH);
            delayMicroseconds(WTT_PULSEDURATION);
            digitalWrite(pin, LOW);
            delayMicroseconds(WTT_ZERO); 
        }
    }
}

void WaterTempTransmitter::sendFrame(byte pin, unsigned long packet, byte nbits, byte repetitions)
{
    for(byte j=0;j<repetitions;j++)
        sendPacket(pin, packet, nbits);
}

