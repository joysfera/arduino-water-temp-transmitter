/*
 * Transmit temperature like Water Pool Thermometer - KW9043
 *
 * Petr Stehlik in 2013-2016
 *
 * GPLv3
 */

#include <WaterTempTransmitter.h>

#define TX433MHZ_PIN        3    // Transmitter attached to this Arduino pin
#define SENSOR_ID           0
#define TX_CHANNEL          3

WaterTempTransmitter tx = WaterTempTransmitter(TX433MHZ_PIN, SENSOR_ID, TX_CHANNEL);

void setup()
{
}

void loop()
{
    static float tempC = 69.9;
    static byte battCnt = 0;
    if (battCnt == 0) battCnt = 5;

    boolean battLow = (--battCnt == 0);
    boolean beep = (random(1, 7) == 6);

    tx.send(tempC, battLow, beep);

    delay(5000);
    tempC = 20 - tempC;
}
