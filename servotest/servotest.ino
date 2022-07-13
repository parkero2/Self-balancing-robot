#include <Servo.h>
#include <Wire.h>
#include <math.h>

//Servo pin definitions
int upperL = A6;
int lowerL = A7;
int upperR = 10;
int lowerR = 9;

//TB6612FNG pin definitions
int lsp = 5;
int rsp = 4;
//Left/right drive
int lf = 8;
int lb = 7;
int rf = 3;
int rb = 4;

void halt() {
  //NOTE: DOES NOT SET SPEED TO 0
  digitalWrite(lf, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(rb, LOW);
}

void driveSpeed(int lSpeed, int rSpeed = -1) {
  //Calling this method allows for 1 or 2 arguments.
  if (rSpeed < 0) {
    lsp, rsp = lSpeed;
    return;
  }
  lsp, rsp = lSpeed, rSpeed;
}

void forward() {
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
}

void backward() {
  digitalWrite(lf, LOW);
  digitalWrite(lb, HIGH);
  digitalWrite(rf, LOW);
  digitalWrite(rb, HIGH);
}

void rotateR() {
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(rb, HIGH);
}

void rotatel() {
  digitalWrite(lf, LOW);
  digitalWrite(lb, HIGH);
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
}

const int MPU = 0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void getGyro() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read(); 
}

void setup() {
  // Setup servo motors
  Servo UL;
  Servo LL;
  Servo UR;
  Servo LR;
  UL.attach(upperL);
  LL.attach(lowerL);
  UR.attach(upperR);
  LR.attach(lowerR);

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);

  Serial.begin(9600);
}

/**
 * REFERENCE:
 * halt()      |   Stop moving (does not affect speed)
 * driveSpeed()|   Change the speed of the motors. Takes arguments (left speed, right speed [OPTIONAL])
 * forward()   |   Go forward
 * backward()  |   Go backward
 * rotateR()   |   Rotate right (on the spot)
 * rotateL()   |   Rotate left (on the spot)
 * getGyro()   |   Update the gyro's current ppostion
 */

void loop() {
  getGyro();
}
