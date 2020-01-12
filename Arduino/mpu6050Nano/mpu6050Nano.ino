// Library inclusions
#include <Wire.h>

// Global variables
const int MPU = 0x68;       // MPU6050 I2C address
float AccX, AccY, AccZ;     // linear acceleration from accelerometer
float GyroX, GyroY, GyroZ;  // angular velocity from gyro
float elapsedTime;          // time tracking variable
float currentTime;          // time tracking variable
float previousTime;         // time tracking variable
int c = 0;

void setup() {
  
  // Serial communication
  Serial.begin(115200);
  
  // Initialize I2C communication on MPU
  Wire.begin();                      // initialize comunication
  Wire.beginTransmission(MPU);       // start communication with MPU6050
  Wire.write(0x6B);                  // talk to the register 6B
  Wire.write(0x00);                  // reset by placing 0 into the 6B register
  Wire.endTransmission(true);        // end the transmission

  // Set the accelerometer to full-scale acceleration (+/-16g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                 // point to 0x1C (Register 28)
  Wire.endTransmission(true);
  Wire.requestFrom(MPU, 1);
  byte x = Wire.read();             // grab the value at 0x1C (Register 28)
  x = x | 0b00011000;               // append '11' into Bit4 and Bit3 to set AFS_SEL to '3'
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                 // point to 0x1C (Register 28)
  Wire.write(x);                    // write the value at 0x1C (Register 28); full-scale range is now +/- 16g
  Wire.endTransmission(true);
    
}

void loop() {
  
  // Read acceleromter data
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);                 // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);   // Read 6 registers total, each axis value is stored in 2 registers

  // Convert to acceleration in [g]
  AccX = (Wire.read() << 8 | Wire.read()) / 2048.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 2048.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 2048.0; // Z-axis value
    
  // Print the values on the serial monitor
  Serial.print(AccX);
  Serial.print(",");
  Serial.print(AccY);
  Serial.print(",");
  Serial.println(AccZ);
  
}
