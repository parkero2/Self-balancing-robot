#include <Servo.h>
#include <Wire.h>
#include <math.h>
#include <SPI.h>
#include "ArduPID.h"
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

//TB6612FNG pin definitions
int lsp = 3;
int rsp = 5;
//Left/right drive
int lf = 6;
int lb = 4;
int rf = 7;
int rb = 2;

//An array to hold temporary values
int datavalues[6];
int motorData[2];
int data[6];
Nrf24l Mirf = Nrf24l(10, 9);

void driveSpeed(int lSpeed, int rSpeed = -1) {
  //Calling this method allows for 1 or 2 arguments.
  lSpeed, rSpeed = abs(lSpeed), abs(rSpeed); ;
  motorData[0], motorData[1] = motorData[0] < 0 ? -lSpeed : lSpeed, motorData[1] < 0 ? -rSpeed : rSpeed;
  if (rSpeed < 0) {
    motorData[0], motorData[1] = lSpeed, lSpeed;
    analogWrite(lsp, lSpeed);
    analogWrite(rsp, lSpeed);
    return;
  }
  motorData[0], motorData[1] = lSpeed, rSpeed;
  analogWrite(lsp, lSpeed);
  analogWrite(rsp, rSpeed);
}

void halt() {
  //NOTE: DOES NOT SET SPEED TO 0
  digitalWrite(lf, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(rb, LOW);
}

void forward() {
  motorData[0], motorData[1] = abs(motorData[0]), abs(motorData[1]);
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
}

void backward() {
  motorData[0], motorData[1] = -abs(motorData[0]), -abs(motorData[1]);
  digitalWrite(lf, LOW);
  digitalWrite(lb, HIGH);
  digitalWrite(rf, LOW);
  digitalWrite(rb, HIGH);
}

void setup() {
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"BALLS"); //Set your own address (receiver address) using 5 characters
    Mirf.payload = sizeof(data);
    Mirf.channel = 90;             //Set the used channel
    Mirf.config();
    Serial.begin(9600);
    Serial.println("Radio frequency initialized");
}

void loop() {
    if (Mirf.dataReady()) {
        Mirf.getData((byte *) data);//Read data and store to data variable
        //interpret the data
        Serial.println(data[sizeof(data)-1]);
        datavalues[int(data[0]) + 1] = data[1, sizeof(data) - 1];
    }
    driveSpeed(abs(int(floor(map(0, 250, 0, 1024, datavalues[0])))), abs(int(floor(map(0, 250, 0, 1024, datavalues[2])))));
    if (int(floor(map(0, 250, 0, 1024, datavalues[0]))) - 512 < 0) {
        digitalWrite(lf, LOW);
        digitalWrite(lb, HIGH);
    }
    else {
        digitalWrite(lf, HIGH);
        digitalWrite(lb, LOW);
    }
    if (int(floor(map(0, 250, 0, 1024, datavalues[2]))) - 512 < 0) {
        digitalWrite(rf, LOW);
        digitalWrite(rb, HIGH);
    }
    else {
        digitalWrite(rf, HIGH);
        digitalWrite(rb, LOW);
    }
    //My test code
    /*forward();
    Serial.println("Forward");
    delay(2000);
    backward();
    Serial.println("Backward");
    delay(2000);*/
}
/**
 * prefixes
 * 1 = LJSX
 * 2 = LJSY
 * 3 = RJSX
 * 4 = RJSY
 * 5 = LSW
 * 6 = RSW 
 */
