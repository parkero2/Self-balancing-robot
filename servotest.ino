#include <Servo.h>
#include <Wire.h>
#include <math.h>

int upperL = 8;
int lowerL = 9;
int upperR = 10;
int lowerR = 11;

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

void loop() {
  getGyro();
  );
}
