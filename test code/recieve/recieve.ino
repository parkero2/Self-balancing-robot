#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

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
    if (!Mirf.isSending() && Mirf.dataReady()) {
        Mirf.getData((byte *) data);//Read data and store to data variable
    }
    delay(10);//Add a short delay
}