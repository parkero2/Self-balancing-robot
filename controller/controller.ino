#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
Nrf24l Mirf = Nrf24l(10, 9);

//Allocate space in memory for data values obtained from the soensors and inputs.
int value[3];
int datavalues[6];

//Pin declarations
//The joystick analog values (direction and intensity)
const int LJSX = A0;
const int LJSY = A1;
const int RJSX = A2;
const int RJSY = A3;

const int LSW = 3; //joycon switches
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
  Mirf.channel = 69;//Set the channel to communicate on
  Mirf.config();
  Mirf.setTADDR((byte *) "BALLS");
  for (int i = 0; i < 6; i++)
  {
    if (i < 4) //These are pins for analog sensors such as joysticks
    {
      pinMode(pins[i], INPUT); //Declare the pins as inputs
      return;
    }
    pinMode(pins[i], INPUT_PULLUP); //These pins are allocated for buttons and as such should be connected to the Arduino's internal resistor.
  }
}

void loop() {
  for (int i = 0; i < 6; i++) //A loop to iterate through the sensors and their values
  {
    if (i < 4)
    {
      Mirf.send(byte(i + 1) + (byte *) analogRead(pins[i])); //Transmit the data containing the analog joystick positions
      while (Mirf.isSending());//A condition that will stop the controller from continuing it's program until the data is done sending
      return; //Remove the need for an else statment by telling the controller to ignore the rest of the loop and go back to the for loop
    }
    Mirf.send(byte(i + 1) + (byte *) digitalRead(pins[i]));
    while (Mirf.isSending());
  }
}