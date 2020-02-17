#include <i2c_driver_wire.h>
//#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!
#include <ssd1351.h>

typedef ssd1351::LowColor Color;

// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
float accXangle;
float accYangle;

// display
auto display = ssd1351::SSD1351<Color, ssd1351::SingleBuffer, 128, 128>();
char data[100];

float mag_max[]={0,0,0};
float mag_min[]={0,0,0};

void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}


void setup() 
{
  Serial.begin(115200);

  while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }

  display.begin();
  Serial.println("LSM9DS1 data read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  setupSensor();
}

void loop() 
{
  lsm.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;

  lsm.getEvent(&a, &m, &g, &temp); 

  //Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2");
  //Serial.print("\tY: "); Serial.print(a.acceleration.y);     Serial.print(" m/s^2 ");
  //Serial.print("\tZ: "); Serial.print(a.acceleration.z);     Serial.println(" m/s^2 ");

  //Serial.print("Mag X: "); Serial.print(m.magnetic.x);   Serial.print(" uT");
  //Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" uT");
  //Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.println(" uT");

  //Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" rad/s");
  //Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" rad/s");
  //Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.println(" rad/s");

  if (m.magnetic.x < mag_min[0]) {
    mag_min[0] = m.magnetic.x;
  }else if (m.magnetic.x > mag_max[0]) {
    mag_max[0] = m.magnetic.x;
  }
  if (m.magnetic.y < mag_min[1]) {
    mag_min[1] = m.magnetic.y;
  }else if (m.magnetic.y > mag_max[1]) {
    mag_max[1] = m.magnetic.y;
  }
  if (m.magnetic.z < mag_min[2]) {
    mag_min[2] = m.magnetic.z;
  }else if (m.magnetic.z > mag_max[2]) {
    mag_max[2] = m.magnetic.z;
  }
  float heading = atan2(m.magnetic.y, m.magnetic.x) * 180 / M_PI;


  //Apply compass calibration
  m.magnetic.x -= (mag_min[0] + mag_max[0]) /2.0;
  m.magnetic.y -= (mag_min[1] + mag_max[1]) /2.0;
  m.magnetic.z -= (mag_min[2] + mag_max[2]) /2.0;

  // Convert Accelerometer values to degrees
  accXangle =  (atan2(a.acceleration.y,a.acceleration.z)*RAD_TO_DEG);
  accYangle =  (atan2(a.acceleration.z,a.acceleration.x)+M_PI)*RAD_TO_DEG;

  // Convert the values to -180 and +180
  if (accYangle > 90) {
    accYangle -= 270.0;
  } else {
    accYangle += 90.0;
  }

  display.fillScreen(ssd1351::RGB());
  display.setFont(FreeMono9pt7b);
  display.setTextSize(1);
  display.setTextColor(ssd1351::RGB(255, 255, 255));
  display.setCursor(0,10);
  //sprintf(data, "x:%.2f\ny:%.2f\nz:%.2f\n",a.acceleration.x, a.acceleration.y, a.acceleration.z); 
  //display.print(data);
  sprintf(data, "a:%.1f,%.1f\n", accXangle, accYangle); 
  display.print(data);
  sprintf(data, "m%.1f,%.1f\n", m.magnetic.x, m.magnetic.y); 
  display.print(data);
  sprintf(data, "h:%.2f\n", heading); 
  display.print(data);  display.updateScreen();

  delay(2);
}
