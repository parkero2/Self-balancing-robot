#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

int data[3];
Nrf24l Mirf = Nrf24l(10, 9);

void setup() {
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  Mirf.setRADDR((byte *)"BALLS"); //Set your own address (receiver address) using 5 characters
  Mirf.payload = sizeof(data);
  Mirf.channel = 90;             //Set the used channel
  Mirf.config();
  Serial.println("Radio frequency initialized");
}

void loop() {
    //Send the first two indexes of the data variable
    Mirf.send((byte *) data[0] + (byte *) data[1]);
    while (Mirf.isSending());
    delay(100);
}