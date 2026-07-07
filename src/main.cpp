//General
#include <Wire.h>
int DEBUG = 1;

//imu
#include "FastIMU.h"
#define IMU_ADDRESS 0x68
#define PERFORM_CALIBRATION
MPU6500 IMU;

//bmp
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

//ms
#include <MS5611.h>
MS5611 ms(0x77);

//calibration datasets
calData calib = { 0 };
AccelData accelData;
GyroData gyroData;

void setup() {
  //I2C, serial, housekeaping stuff
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);

  //IMU
  int imuErr = IMU.init(calib, IMU_ADDRESS);

  if (!DEBUG){
    Serial.println("FastIMU calibration & data example");
    Serial.println("Keep IMU level.");
    delay(5000);
    IMU.calibrateAccelGyro(&calib);
    Serial.println("Calibration done!");
    Serial.println("Accel biases X/Y/Z: ");
    Serial.print(calib.accelBias[0]);
    Serial.print(", ");
    Serial.print(calib.accelBias[1]);
    Serial.print(", ");
    Serial.println(calib.accelBias[2]);
    Serial.println("Gyro biases X/Y/Z: ");
    Serial.print(calib.gyroBias[0]);
    Serial.print(", ");
    Serial.print(calib.gyroBias[1]);
    Serial.print(", ");
    Serial.println(calib.gyroBias[2]);
    delay(5000);
    IMU.init(calib, IMU_ADDRESS);
    };

  //bmp
  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
              Adafruit_BMP280::SAMPLING_X2,
              Adafruit_BMP280::SAMPLING_X16,
              Adafruit_BMP280::FILTER_X2,
              Adafruit_BMP280::STANDBY_MS_500);
  
  //ms
  ms.begin();
}

void loop() {
  IMU.update();
  IMU.getAccel(&accelData);
  Serial.print(">accelX:");
  Serial.println(accelData.accelX);
  Serial.print(">accelY:");
  Serial.println(accelData.accelY);
  Serial.print(">accelZ:");
  Serial.println(accelData.accelZ);
  IMU.getGyro(&gyroData);
  Serial.print(">gyroX:");
  Serial.println(gyroData.gyroX);
  Serial.print(">gyroY:");
  Serial.println(gyroData.gyroY);
  Serial.print(">gyroZ:");
  Serial.println(gyroData.gyroZ);
  if (IMU.hasTemperature()) {
	  Serial.print(">imuTemp:");
	  Serial.println(IMU.getTemp());
  }
  Serial.print(">bmpTemp:");
  Serial.println(bmp.readTemperature());

  Serial.print(">bmpPressure:");
  Serial.println(bmp.readPressure()/100);

  Serial.print(">bmpAltitude:");
  Serial.println(bmp.readAltitude(1013.25));
  delay(50);

  ms.read();
  Serial.print(">msTemp:");
  Serial.println(ms.getTemperature());
  Serial.print(">msPressure:");
  Serial.println(ms.getPressure());
  Serial.print(">msAltitude:");
  Serial.println(ms.getAltitude(1013.25));

  delay(50);

}
