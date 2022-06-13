#include <M5StickC.h>
#include <Wire.h>
#include "BluetoothSerial.h"
#include <Adafruit_LIS3MDL.h>
#include "Adafruit_Sensor.h"
#include <MadgwickAHRS.h>

Adafruit_LIS3MDL lis3mdl;

BluetoothSerial bluetooth;
Madgwick MadgwickFilter;

float acc[3];
float accOffset[3];
float gyro[3];
float gyroOffset[3];

float mag[3];
float magOffset[3];
float magmax[3];
float magmin[3];
uint8_t do_cal = 0;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;
int hz = 100;

long lastMs = 0;
int count = 0;
int mode = 0;

void calibrateMag()
{
  sensors_event_t event; 

  lis3mdl.getEvent(&event);

  magmin[0] = magmax[0] = event.magnetic.x;
  magmin[1] = magmax[1] = event.magnetic.x;
  magmin[2] = magmax[2] = event.magnetic.x;
  
  for( unsigned int i = 0 ; i < 128 ; i++ ) {
    lis3mdl.getEvent(&event);

    if ( event.magnetic.x < magmin[0] ) {
      magmin[0] = event.magnetic.x;
    }
    if ( magmax[0] < event.magnetic.x ) {
      magmax[0] = event.magnetic.x;
    }

    if ( event.magnetic.y < magmin[1] ) {
      magmin[1] = event.magnetic.y;
    }
    if ( magmax[1] < event.magnetic.y ) {
      magmax[1] = event.magnetic.y;
    }

    if ( event.magnetic.z < magmin[2] ) {
      magmin[2] = event.magnetic.z;
    }
    if ( magmax[2] < event.magnetic.z ) {
      magmax[2] = event.magnetic.z;
    }
    delay(128);
  }
  
  for( unsigned int i = 0 ; i < 3 ; i++ ) {
    magOffset[i] = (magmin[i] + magmax[i])/2;
    Serial.print(magOffset[i]); Serial.print(" ");Serial.print(magmin[i]); Serial.print(" "); Serial.println(magmax[i]);
  }
  if ( mode ) {
     M5.Lcd.fillScreen(BLACK);
  }
}

void calibrate6886(int counter=500, int ms=13)
{
  float gyroSum[3];
  float accSum[3];

  for(int i = 0; i < 3; i++){
    gyroSum[i] = 0.0;
    accSum[i] = 0.0;
  }
  
  for(int i = 0; i < counter; i++){
    M5.IMU.getGyroData(&gyro[0],&gyro[1],&gyro[2]);
    M5.IMU.getAccelData(&acc[0],&acc[1],&acc[2]);
    gyroSum[0] += gyro[0];
    gyroSum[1] += gyro[1];
    gyroSum[2] += gyro[2];
    accSum[0] += acc[0];
    accSum[1] += acc[1];
    accSum[2] += acc[2];
    delay(2+ms);
  }
  for(int i = 0; i < 3; i++){
    Serial.println(gyroSum[i]);
  }
  for(int i = 0; i < 3; i++){
    Serial.println(accSum[i]);
  }
  
  gyroOffset[0] = gyroSum[0]/counter;
  gyroOffset[1] = gyroSum[1]/counter;
  gyroOffset[2] = gyroSum[2]/counter;
  accOffset[0] = accSum[0]/counter;
  accOffset[1] = accSum[1]/counter;
  accOffset[2] = (accSum[2]/counter) - 1.0;//重力加速度1G、つまりM5ボタンが上向きで行う想定
}


void setup() {

  M5.begin();
  
  M5.IMU.Init();
  Serial.begin(115200);
  //bluetooth.begin("M5StickC");
  calibrate6886(100);
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("IMU TEST");
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.println("  X       Y       Z");
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.println("  Roll    Pitch   Yaw");

  static const char *a[] = {
    "UNknown", "SH200Q", "IMU_MP6886"
  };
  Serial.println(a[M5.IMU.imuType]);

  Wire.begin(0,26);
  if (! lis3mdl.begin_I2C(0x1C)) {
    Serial.println("Failed to find LIS3MDL chip");
    while (1) { delay(10); }
  }
  Serial.println("LIS3MDL Found!");
  lis3mdl.setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);
  Serial.print("Performance mode set to: ");
  switch (lis3mdl.getPerformanceMode()) {
    case LIS3MDL_LOWPOWERMODE: Serial.println("Low"); break;
    case LIS3MDL_MEDIUMMODE: Serial.println("Medium"); break;
    case LIS3MDL_HIGHMODE: Serial.println("High"); break;
    case LIS3MDL_ULTRAHIGHMODE: Serial.println("Ultra-High"); break;
  }

  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  Serial.print("Operation mode set to: ");
  // Single shot mode will complete conversion and go into power down
  switch (lis3mdl.getOperationMode()) {
    case LIS3MDL_CONTINUOUSMODE: Serial.println("Continuous"); break;
    case LIS3MDL_SINGLEMODE: Serial.println("Single mode"); break;
    case LIS3MDL_POWERDOWNMODE: Serial.println("Power-down"); break;
  }

  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin
  Serial.print("Data rate set to: ");
  switch (lis3mdl.getDataRate()) {
    case LIS3MDL_DATARATE_0_625_HZ: Serial.println("0.625 Hz"); break;
    case LIS3MDL_DATARATE_1_25_HZ: Serial.println("1.25 Hz"); break;
    case LIS3MDL_DATARATE_2_5_HZ: Serial.println("2.5 Hz"); break;
    case LIS3MDL_DATARATE_5_HZ: Serial.println("5 Hz"); break;
    case LIS3MDL_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3MDL_DATARATE_20_HZ: Serial.println("20 Hz"); break;
    case LIS3MDL_DATARATE_40_HZ: Serial.println("40 Hz"); break;
    case LIS3MDL_DATARATE_80_HZ: Serial.println("80 Hz"); break;
    case LIS3MDL_DATARATE_155_HZ: Serial.println("155 Hz"); break;
    case LIS3MDL_DATARATE_300_HZ: Serial.println("300 Hz"); break;
    case LIS3MDL_DATARATE_560_HZ: Serial.println("560 Hz"); break;
    case LIS3MDL_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }
  
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (lis3mdl.getRange()) {
    case LIS3MDL_RANGE_4_GAUSS: Serial.println("+-4 gauss"); break;
    case LIS3MDL_RANGE_8_GAUSS: Serial.println("+-8 gauss"); break;
    case LIS3MDL_RANGE_12_GAUSS: Serial.println("+-12 gauss"); break;
    case LIS3MDL_RANGE_16_GAUSS: Serial.println("+-16 gauss"); break;
  }

  /*
  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!
  */

  MadgwickFilter.begin(hz); //100Hz
}

void applycalibration()
{
  M5.IMU.getGyroData(&gyro[0],&gyro[1],&gyro[2]);
  M5.IMU.getAccelData(&acc[0],&acc[1],&acc[2]);
  gyro[0] -= gyroOffset[0];
  gyro[1] -= gyroOffset[1];
  gyro[2] -= gyroOffset[2];
  acc[0] -= accOffset[0];
  acc[1] -= accOffset[1];
  acc[2] -= accOffset[2];

  if ( count % 2 ) {  
    sensors_event_t event; 
    lis3mdl.getEvent(&event);

    mag[0] = event.magnetic.x - magOffset[0];
    mag[1] = event.magnetic.y - magOffset[1];
    mag[2] = event.magnetic.z - magOffset[2];
  } else {
    delay(2);
  }
}

void loop() {
  float dt = (micros() - lastMs) / 1000000.0;
  lastMs = micros();

  count++;
  if (( count % 256 ) == 0 ) {
    Serial.println(String(dt, 8));
  }
  applycalibration();

  MadgwickFilter.updateIMU(gyro[0], gyro[1], gyro[2], acc[0], acc[1], acc[2]);
  //MadgwickFilter.update(gyro[0], gyro[1], gyro[2], acc[0], acc[1], acc[2], event.magnetic.x, event.magnetic.y, event.magnetic.z);

  roll = MadgwickFilter.getRoll();
  pitch = MadgwickFilter.getPitch();
  yaw   = MadgwickFilter.getYaw();

  if ( mode == 0 ) {
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ", gyro[0], gyro[1], gyro[2]);//deg
    M5.Lcd.setCursor(140, 20);
    M5.Lcd.print("o/s");
  
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", acc[0], acc[1], acc[2]);
    M5.Lcd.setCursor(140, 30);
    M5.Lcd.print("G");
  
    float heading = atan2(-mag[1], -mag[0]); // 東西南北合わせる
    if(heading < 0)
      heading += 2*PI;
    if(heading > 2*PI)
      heading -= 2*PI;
  
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("headingDegrees: %2.1f", heading * 180/M_PI);  
  
    M5.Lcd.setCursor(0, 60);
    M5.Lcd.printf("%6.2f, %6.2f, %6.2f\r\n", roll, pitch, yaw);
  } else {
    if ( do_cal == 0 ) {
      static int last_x, last_y;

      M5.Lcd.drawLine(80, 40, 80+last_x, 40+last_y, BLACK);
 
      float heading = atan2(-mag[1], -mag[0]);
      last_x = int(cos(heading)*40);
      last_y = int(-sin(heading)*40);
      M5.Lcd.drawLine(80, 40, 80+last_x, 40+last_y, BLUE);
      delay(6);
    }
  }

  //bluetooth.printf("%6.2f, %6.2f, %6.2f\r\n", roll, pitch, yaw);
  delay(1);

  switch(do_cal) {
    default:
      break;
    case 1:
      do_cal = 0;
      calibrate6886();
      break;
    case 2:
      do_cal = 0;
      calibrateMag();
      break;
 }
   
 if(digitalRead(M5_BUTTON_HOME) == LOW){
  do_cal = 1;
  while(digitalRead(M5_BUTTON_HOME) == LOW);
  delay(1000);
  hz = int(1.0/dt);
  Serial.println("Hz:");
  Serial.println(hz);
  MadgwickFilter.begin(hz);
  delay(100);
 }

 if(digitalRead(BUTTON_B_PIN) == LOW){
  do_cal = 2;
  while(digitalRead(BUTTON_B_PIN) == LOW);
  if ( digitalRead(M5_BUTTON_HOME) == LOW ) {
    mode = 1;
  } else {
    mode = 0;
  }
  delay(1000);
 }

 M5.Lcd.setCursor(140, 70);
 if (do_cal) {
   M5.Lcd.printf("CAL");
 } else {
   M5.Lcd.printf("   ");
 }

}
