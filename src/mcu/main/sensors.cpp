#include "pin_layout.h"
#include <MQ135.h>
#include <MQ2_LPG.h>

// --- CO2 Level Thresholds ---
#define CRITICAL_CO2_THRESHOLD 2000
#define HIGH_CO2_THRESHOLD 800
#define NORMAL_CO2_THRESHOLD 0
// --- CO2 Level Thresholds end ---

// --- MQ2 Sensor - calibration data ---
#define R_LOAD 100            // resistance load setting (100K ohm)
#define R_OSMOSIS 6.02        // reverse osmosis setting
#define SUPPLY_VOLTAGE 5.0    // VCC
#define ADC_RESOLUTION 1023.0 // development board adc resolution
#define TEMPERATURE 21.0      // Assume temperature and humidity
#define HUMIDITY 25.0
// --- MQ2 Sensor - calibration data end ---

// --- custom types and global variables ---
typedef struct
{
  MQ135 mq135_sensor;
  MQ2Sensor mq2_sensor;
} sensor_devices_t;

typedef struct
{
  float co2;
  float ch4;
} sensor_values_t;

typedef struct
{
  sensor_devices_t devices;
  sensor_values_t values;
} sensors_t;

sensors_t g_sensors[NUMBER_OF_SENSOR_GROUPS] = {
    {.devices = sensor_devices_t{MQ135(PIN_MQ135_0), MQ2Sensor(PIN_MQ2_0)},
     .values = sensor_values_t{.co2 = 0.0f, .ch4 = 0.0f}},
    {.devices = sensor_devices_t{MQ135(PIN_MQ135_1), MQ2Sensor(PIN_MQ2_1)},
     .values = sensor_values_t{.co2 = 0.0f, .ch4 = 0.0f}}};
// --- custom types and global variables end ---

static void init(int sensorIndex)
{
  g_sensors[sensorIndex].devices.mq2_sensor.RL(R_LOAD);
  g_sensors[sensorIndex].devices.mq2_sensor.Ro(R_OSMOSIS);
  g_sensors[sensorIndex].devices.mq2_sensor.Volt(SUPPLY_VOLTAGE);
  g_sensors[sensorIndex].devices.mq2_sensor.BitADC(ADC_RESOLUTION);
}

void initAllSensors()
{
  for (int i = 0; i < NUMBER_OF_SENSOR_GROUPS; i++)
  {
    init(i);
  }
}

static void pollSensors(int sensorIndex)
{
  g_sensors[sensorIndex].values.co2 = 
    g_sensors[sensorIndex].devices.mq135_sensor.getCorrectedPPM(TEMPERATURE, HUMIDITY);
  g_sensors[sensorIndex].values.ch4 = g_sensors[sensorIndex].devices.mq2_sensor.readGas();
}

void pollAllSensors()
{
  for (int i = 0; i < NUMBER_OF_SENSOR_GROUPS; i++)
  {
    pollSensors(i);
  }
}

float getCo2Value(int sensorIndex)
{
  return g_sensors[sensorIndex].values.co2;
}

int getCo2Level(int sensorIndex)
{
  if (g_sensors[sensorIndex].values.co2 >= CRITICAL_CO2_THRESHOLD)
  {
    return 2;
  }
  if (g_sensors[sensorIndex].values.co2 >= HIGH_CO2_THRESHOLD)
  {
    return 1;
  }
  if (g_sensors[sensorIndex].values.co2 >= NORMAL_CO2_THRESHOLD)
  {
    return 0;
  }
  return -1;
}

static void sendSensorValues(int sensorIndex)
{
  Serial.print(sensorIndex);
  Serial.print(F(";"));
  Serial.print(g_sensors[sensorIndex].values.co2, 2);
  Serial.print(F(";"));
  Serial.println(g_sensors[sensorIndex].values.ch4, 2);
}

void sendAllSensorValues()
{
  for (int i = 0; i < NUMBER_OF_SENSOR_GROUPS; i++)
  {
    sendSensorValues(i);
  }
}
