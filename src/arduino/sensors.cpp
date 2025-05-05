#include "sensors.h"

#define CRITICAL_THRESHOLD 2000
#define HIGH_THRESHOLD 800
#define NORMAL_THRESHOLD 0

typedef struct {
    MQ135 mq135_sensor;
    // MQ2 mq2_sensor     // Methan Sensor
} sensor_devices_t;

typedef struct {
    float co2;
    float ch4;
} sensor_values_t;

typedef struct {
    sensor_devices_t devices;
    sensor_values_t values;
} sensors_t;

sensors_t sensors[NUMBER_OF_SENSORS] = {
    {
        .devices = sensor_devices_t{MQ135(PIN_MQ135_0)},
        .values  = sensor_values_t{.co2 = 0.0f, .ch4 = 0.0f}
    }, 
    {
        .devices = sensor_devices_t{MQ135(PIN_MQ135_1)},
        .values  = sensor_values_t{.co2 = 0.0f, .ch4 = 0.0f}
    }
};

const float temperature = 21.0; // Assume current temperature. Recommended to measure with DHT22
const float humidity = 25.0; // Assume current humidity. Recommended to measure with DHT22

void pollSensors(int sensor_index){
  float co2_ppm = sensors[sensor_index].devices.mq135_sensor.getCorrectedPPM(temperature, humidity);
  sensors[sensor_index].values.co2 = co2_ppm;
}

void pollAllSensors(){
  for (int i = 0; i < NUMBER_OF_SENSORS; i++){
    pollSensors(i);
  }
}

float getCo2Value(int sensor_index){
  return sensors[sensor_index].values.co2;
}

int getCo2Level(int sensor_index){
  if (sensors[sensor_index].values.co2 >= CRITICAL_THRESHOLD) {
    return 2;
  }
  if (sensors[sensor_index].values.co2 >= HIGH_THRESHOLD) {
    return 1;
  }
  if (sensors[sensor_index].values.co2 >= NORMAL_THRESHOLD) {
    return 0;
  }
  return -1;
}

void sendSensorValues(int sensor_index){
  Serial.print(sensor_index);
  Serial.print(F(";"));
  Serial.print(sensors[sensor_index].values.co2, 2);
  Serial.print(F(";"));
  Serial.println(sensors[sensor_index].values.ch4, 2);
}

void sendAllSensorValues(){
  for (int i = 0; i < NUMBER_OF_SENSORS; i++){
    sendSensorValues(i);
  }
}
