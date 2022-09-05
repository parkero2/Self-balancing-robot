#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
Nrf24l Mirf = Nrf24l(10, 9);
int value[3];
int datavalues[6];

//Pin declarations
const int LJSX = A0;
const int LJSY = A1;
const int RJSX = A2;
const int RJSY = A3;

const int LSW = 3;
const int RSW = 4;

//add all the pins to an array
const int pins[] = {LJSX, LJSY, RJSX, RJSY, LSW, RSW};

void setup()
{
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  //Set your own address (sender address) using 5 characters
  Mirf.setRADDR((byte *)"RANGA");
  Mirf.payload = sizeof(value);
  Mirf.channel = 69;//Set the channel used
  Mirf.config();
  Mirf.setTADDR((byte *) "BALLS");
  for (int i = 0; i < 6; i++)
  {
    if (i < 4)
    {
      pinMode(pins[i], INPUT);
      return;
    }
    pinMode(pins[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 0; i < 6; i++)
  {
    if (i < 4)
    {
      Mirf.send(byte(i + 1) + (byte *) analogRead(pins[i]));
      while (Mirf.isSending());
      return;
    }
    Mirf.send(byte(i + 1) + (byte *) digitalRead(pins[i]));
    while (Mirf.isSending());
  }
}