// Library inclusions
#include <Wire.h>

// Global variables
const int MPU1 = 0x68;      // MPU6050 I2C address
const int MPU2 = 0x69;      // MPU6050 I2C address
float accX1, accY1, accZ1;  // linear acceleration from accelerometer1
float accX2, accY2, accZ2;  // linear acceleration from accelerometer2
int x1,y1,z1;               // linear acceleration from accelerometer1 converted to int
int x2,y2,z2;               // linear acceleration from accelerometer2 converted to int
unsigned long t;            // time
unsigned int tLast;         // last time
unsigned int dt;            // time step




void setup() {
  
  // Begin serial communication
  Serial.begin(115200);
  
  // Initialize I2C communication on MPU1
  Wire.begin();                      // initialize comunication
  Wire.beginTransmission(MPU1);      // start communication with MPU6050
  Wire.write(0x6B);                  // talk to the register 6B
  Wire.write(0x00);                  // reset by placing 0 into the 6B register
  Wire.endTransmission(true);        // end the transmission

  // Initialize I2C communication on MPU2
  Wire.begin();                      // initialize comunication
  Wire.beginTransmission(MPU2);      // start communication with MPU6050
  Wire.write(0x6B);                  // talk to the register 6B
  Wire.write(0x00);                  // reset by placing 0 into the 6B register
  Wire.endTransmission(true);        // end the transmission

  // Set the MPU1 accelerometer to full-scale acceleration (+/-16g)
  Wire.beginTransmission(MPU1);
  Wire.write(0x1C);                 // point to 0x1C (Register 28)
  Wire.endTransmission(true);
  Wire.requestFrom(MPU1,1);
  byte x = Wire.read();             // grab the value at 0x1C (Register 28)
  x = x | 0b00011000;               // append '11' into Bit4 and Bit3 to set AFS_SEL to '3'
  Wire.beginTransmission(MPU1);
  Wire.write(0x1C);                 // point to 0x1C (Register 28)
  Wire.write(x);                    // write the value at 0x1C (Register 28); full-scale range is now +/- 16g
  Wire.endTransmission(true);

  // Set the MPU2 accelerometer to full-scale acceleration (+/-16g)
  Wire.beginTransmission(MPU2);
  Wire.write(0x1C);                 // point to 0x1C (Register 28)
  Wire.endTransmission(true);
  Wire.requestFrom(MPU2,1);
  x = Wire.read();                  // grab the value at 0x1C (Register 28)
  x = x | 0b00011000;               // append '11' into Bit4 and Bit3 to set AFS_SEL to '3'
  Wire.beginTransmission(MPU2);
  Wire.write(0x1C);                 // point to 0x1C (Register 28)
  Wire.write(x);                    // write the value at 0x1C (Register 28); full-scale range is now +/- 16g
  Wire.endTransmission(true);
  
}

void loop() {
  
  // Read acceleromter data on MPU1
  Wire.beginTransmission(MPU1);
  Wire.write(0x3B);                             // start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU1,6,true);                // read 6 registers total, each axis value is stored in 2 registers
  accX1 = (Wire.read()<<8|Wire.read())/2048.0;  // x-axis acceleration [g]
  accY1 = (Wire.read()<<8|Wire.read())/2048.0;  // y-axis acceleration [g]
  accZ1 = (Wire.read()<<8|Wire.read())/2048.0;  // z-axis acceleration [g]
  Wire.endTransmission(true);

  // Read acceleromter data on MPU2
  Wire.beginTransmission(MPU2);
  Wire.write(0x3B);                             // start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU2,6,true);                // read 6 registers total, each axis value is stored in 2 registers
  accX2 = (Wire.read()<<8|Wire.read())/2048.0;  // x-axis acceleration [g]
  accY2 = (Wire.read()<<8|Wire.read())/2048.0;  // y-axis acceleration [g]
  accZ2 = (Wire.read()<<8|Wire.read())/2048.0;  // z-axis acceleration [g]
  Wire.endTransmission(true);

  // Convert accelerometer data to int
  x1 = accX1*100;
  y1 = accY1*100;
  z1 = accZ1*100;
  x2 = accX2*100;
  y2 = accY2*100;
  z2 = accZ2*100;

  // Time calculations
  tLast = t;
  t = micros();
  dt = t-tLast;

  // Print    
  Serial.print(dt);
  Serial.print(',');
  Serial.print(x1);
  Serial.print(',');
  Serial.print(y1);
  Serial.print(',');
  Serial.print(z1);
  Serial.print(',');
  Serial.print(x2);
  Serial.print(',');
  Serial.print(y2);
  Serial.print(',');
  Serial.println(z2);

}
