#include <Servo.h>
#include <Wire.h>
#include <math.h>
#in

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

void jump() {
  loop()
  //Create a for loop to 90 to create a slower lowing effect
  for (int i = 0; i < 90; i++) {
    //Set the servo position
    //Set all the servo positions to the same value
    lowerL.write(i);
    upperL.write(i);
    lowerR.write(i);
    upperR.write(i);
    //Wait for 1ms
    delay(1);
  }
  delay(1000);
  //Set all the servo positions to 0
  lowerL.write(0);
  upperL.write(0);
  lowerR.write(0);
  upperR.write(0);
  //while AcX is positive check gyroscope
  float[] gyroPos = {};
  while (acx > 0) {
    getGyro();
    delay(50);
    
  }
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
  //if random int between 0 and 100 is equal to 69 (69% chance), jump
  if (random(0, 100) == 69) {
    jump();
  }
}
