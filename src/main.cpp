//General
#include <Wire.h>
int DEBUG = 1;

//webserial
#include "webserial.h"
WebSerialClass WebSerial;

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

//kalman
#include <AutoKalman.h>
AutoKalman2D kalman2D(0.5, 3.0, AutoKalman2D::Vector2D(0.0, 0.0), AutoKalman2D::Vector2D(0.0, 0.0));
int prevTimeKalman = 0;


//calibration datasets
calData calib = { 0 };
AccelData accelData;
GyroData gyroData;

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  WebSerial.begin("Z");

  int imuErr = IMU.init(calib, IMU_ADDRESS);

  if (!DEBUG){
    WebSerial.println("FastIMU calibration & data example");
    WebSerial.println("Keep IMU level.");
    delay(5000);
    IMU.calibrateAccelGyro(&calib);
    WebSerial.println("Calibration done!");
    WebSerial.println("Accel biases X/Y/Z: ");
    WebSerial.print(calib.accelBias[0]);
    WebSerial.print(", ");
    WebSerial.print(calib.accelBias[1]);
    WebSerial.print(", ");
    WebSerial.println(calib.accelBias[2]);
    WebSerial.println("Gyro biases X/Y/Z: ");
    WebSerial.print(calib.gyroBias[0]);
    WebSerial.print(", ");
    WebSerial.print(calib.gyroBias[1]);
    WebSerial.print(", ");
    WebSerial.println(calib.gyroBias[2]);
    delay(5000);
    IMU.init(calib, IMU_ADDRESS);
    };

  bmp.begin(0x76);
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_NONE,
                    Adafruit_BMP280::SAMPLING_NONE,
                    Adafruit_BMP280::FILTER_OFF,
                    Adafruit_BMP280::STANDBY_MS_1);
  
  ms.begin();
}

void loop() {
  WebSerial.loop();
  IMU.update();
  IMU.getAccel(&accelData);
  WebSerial.print(">accelX:");
  WebSerial.println(accelData.accelX);
  WebSerial.print(">accelY:");
  WebSerial.println(accelData.accelY);
  WebSerial.print(">accelZ:");
  WebSerial.println(accelData.accelZ);
  IMU.getGyro(&gyroData);
  WebSerial.print(">gyroX:");
  WebSerial.println(gyroData.gyroX);
  WebSerial.print(">gyroY:");
  WebSerial.println(gyroData.gyroY);
  WebSerial.print(">gyroZ:");
  WebSerial.println(gyroData.gyroZ);
  if (IMU.hasTemperature()) {
	  WebSerial.print(">imuTemp:");
	  WebSerial.println(IMU.getTemp());
  }
  WebSerial.print(">bmpTemp:");
  WebSerial.println(bmp.readTemperature());

  WebSerial.print(">bmpPressure:");
  WebSerial.println(bmp.readPressure()/100);

  WebSerial.print(">bmpAltitude:");
  WebSerial.println(bmp.readAltitude(1013.25));

  ms.read();
  WebSerial.print(">msTemp:");
  WebSerial.println(ms.getTemperature());
  WebSerial.print(">msPressure:");
  WebSerial.println(ms.getPressure());
  WebSerial.print(">msAltitude:");
  WebSerial.println(ms.getAltitude(1013.25));


  //kalman
  float accelY = (accelData.accelY - 1)*9.80665;
  float Alt = ms.getAltitude(1013.25);
  float dt = (micros() - prevTimeKalman) / 1000000.0;
  prevTimeKalman = micros();
  AutoKalman2D::Vector2D measurement(Alt, 0.0);
  AutoKalman2D::Vector2D result = kalman2D.filterPosition(measurement, dt);
  Alt = result.x;


  WebSerial.print(">KalmanAlt:");
  WebSerial.println(Alt);
  WebSerial.print(">KalmanVel:");
  WebSerial.println(result.y);

  WebSerial.println();
  delay(100);
}