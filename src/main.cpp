#include <Arduino.h>
#include <Wire.h>
#include <vl53l5cx_class.h>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <cassert>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

VL53L5CX sensors[] = {
  (VL53L5CX(&Wire,PB4)),
  (VL53L5CX(&Wire,PA11))
};

uint16_t sensors_addr[] = {0x56, 0x58};

void setup()
{

  //Initialize serial for output.
  Serial.begin(115200);
  Serial.println("Initialize... Please wait, it may take few seconds...");

  //Initialize I2C bus.
  Wire.setSCL(PA8);
  Wire.setSDA(PB5);
  Wire.begin();

  //Shutdown every i2c connection
  for (auto sensor: sensors) {
    sensor.begin();
    sensor.vl53l5cx_off();
  }

  delay(1000);

  // Initialize every vl53l5cx according to config
  uint8_t sensor_number = 0;
  for (auto sensor: sensors) {
    printf("Starting VL53L5CX %d for address : 0x%02x \n", sensor_number, sensors_addr[sensor_number]);

    //For each sensor activate the i2c connection and change the i2c address
    sensor.vl53l5cx_on();
    uint8_t status = sensor.init_sensor(sensors_addr[sensor_number]);

    if (status != VL53L5CX_STATUS_OK) {
      printf(COLOR_RED "Failed to initialize VL53L5CX %d, status : %d\n" COLOR_RESET, sensor_number, status);
    }else{
      printf(COLOR_GREEN "VL53L5CX %d initialized successfully !\n" COLOR_RESET, sensor_number);
    }
    sensor_number++;
  }
}

void loop()
{
}