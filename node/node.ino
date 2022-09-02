#include <Wire.h>
#include <WebSocketServer.h>

WebSocketServer webSocket(81);

char[] data;

void setup() {
    Wire.begin(69);//start the I2C bus
    Wire.onReceive(i2cRecieve)

}

void i2cRecieve(int bytes)  {
    data = new char[Wire.read()];
} 

void loop() {
}