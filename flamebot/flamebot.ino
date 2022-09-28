#include <Servo.h>
#include <Wire.h>
#include <math.h>
#include <SPI.h>
#include "ArduPID.h"
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

//TB6612FNG pin definitions
int lsp = 5;
int rsp = 6;
//Left/right drive
int lf = 7;
int lb = 8;
int rf = 4;
int rb = 3;

int datavalues[6];

Nrf24l Mirf = Nrf24l(A1, A2);

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

void rotateR() {
  motorData[0], motorData[1] = abs(motorData[0]), -abs(motorData[1]);
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(rb, HIGH);
}

void rotatel() {
  motorData[0], motorData[1] = -abs(motorData[0]), abs(motorData[1]);
  digitalWrite(lf, LOW);
  digitalWrite(lb, HIGH);
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
}

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
    if (Mirf.dataReady()) {
        Mirf.getData((byte *) data);//Read data and store to data variable
        //interpret the data
        datavalues[int(data[0]) + 1] = data[1, sizeof(data) - 1];
    }
    driveSpeed(abs(int(floor(map(0, 250, -128, 128, datavalues[0])))), abs(int(floor(map(0, 250, -128, 128, datavalues[2])))));
    if (int(floor(map(0, 250, -128, 128, datavalues[0]))) < 0) {
        digitalWrite(lf, LOW);
        digitalWrite(lb, HIGH);
    }
    else {
        digitalWrite(lf, HIGH);
        digitalWrite(lb, LOW);
    }
    if (int(floor(map(0, 250, -128, 128, datavalues[1]))) < 0) {
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