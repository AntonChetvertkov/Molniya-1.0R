//General
#include <Wire.h>
int DEBUG = 1;

//imu defines
#include "FastIMU.h"
#define IMU_ADDRESS 0x68
#define PERFORM_CALIBRATION
MPU6500 IMU;

//bmp defines
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

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
    delay(5000);
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
              Adafruit_BMP280::FILTER_X16,
              Adafruit_BMP280::STANDBY_MS_500);
}

void loop() {
  IMU.update();
  IMU.getAccel(&accelData);
  Serial.print(accelData.accelX);
  Serial.print("\t");
  Serial.print(accelData.accelY);
  Serial.print("\t");
  Serial.print(accelData.accelZ);
  Serial.print("\t");
  IMU.getGyro(&gyroData);
  Serial.print(gyroData.gyroX);
  Serial.print("\t");
  Serial.print(gyroData.gyroY);
  Serial.print("\t");
  Serial.print(gyroData.gyroZ);
  if (IMU.hasTemperature()) {
	  Serial.print("\t");
	  Serial.println(IMU.getTemp());
  }
  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1013.25));
  Serial.println(" m");
  delay(50);
}
