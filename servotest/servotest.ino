#include <Servo.h>
#include <Wire.h>
#include <math.h>

//Servo pin definitions
int upperL = 12;
int lowerL = 11;
int upperR = 10;
int lowerR = 9;

//Declare servos
Servo UL;
Servo LL;
Servo UR;
Servo LR;

const int MPU = 0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//TB6612FNG pin definitions
int lsp = 5;
int rsp = 4;
//Left/right drive
int lf = 6;
int lb = 7;
int rf = 3;
int rb = 4;

int motorData[6];
//lsp, rsp, UL, LL, UR, LR

void halt() {
  //NOTE: DOES NOT SET SPEED TO 0
  digitalWrite(lf, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(rb, LOW);
}

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

void jump() {
  //Create a for loop to 90 to create a slower lowing effect
  for (int i = 0; i < 90; i++) {
    //Set the servo position
    //Set all the servo positions to the same value
    LL.write(i);
    UL.write(i);
    LR.write(i);
    UR.write(i);
    //Wait for 1ms
    delay(1);
  }
  delay(1000);
  //Set all the servo positions to 0
  LL.write(0);
  UL.write(0);
  LR.write(0);
  UR.write(0);
  //while AcX is positive check gyroscope
  //float[] gyroPos = {}
  while (AcX > 0) {
    getGyro();
    delay(50);
    //add the current AcX value to the gyroPos array
    //gyroPos[] += AcX;
  }
}

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

void sendData() {
  Wire.beginTransmission(69);//Begin transmission to nodeMCU
  Wire.write(itoa(motorData));//Send the data
  Wire.endTransmission();//End transmission
}

void setup() {
  // Setup servo motors
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

  //If GyX is positive, move forward
  if (GyX > 0) {
    forward();
  }
  //If GyX is negative, move backward
  else if (GyX < 0) {
    backward();
  }
  //If GyX is 0, halt
  else {
    halt();
  }
  //if random int between 0 and 100 is equal to 69 (1% chance), jump
  if (random(0, 100) == 69) {
    jump();
  }
}
