#include <Arduino.h>
#include "circular_buffer.h"
#include <Wire.h>
#include "Adafruit_LTR329_LTR303.h"

#define CBUFFER_LENGTH  50

struct LTR303_t {
  Adafruit_LTR303 ltr303;
  uint16_t visible_ir_lux;
  uint16_t ir_lux;
  uint32_t buffer[CBUFFER_LENGTH];
};

uint32_t buffer[10] = {0};
circular_buffer_t* light_sensor_cbuf;

uint32_t ch0;

LTR303_t light_sensor;

bool light_sensor_isInitialized = false;

void setup() {


  // put your setup code here, to run once:
  Serial.begin(115200);

  //Set I2C pins according to schematic 
  Wire.setPins(20, 21);

  light_sensor_cbuf = init_circular_buffer(buffer, 10);

  circular_buffer_set_n_avg(light_sensor_cbuf, 5);

  Serial.println("Initialization of LTR303");
  light_sensor_isInitialized = light_sensor.ltr303.begin(&Wire);
  //Set gain, integration time and measurement rate
  light_sensor.ltr303.setGain(LTR3XX_GAIN_8);
  light_sensor.ltr303.setIntegrationTime(LTR3XX_INTEGTIME_200);
  light_sensor.ltr303.setMeasurementRate(LTR3XX_MEASRATE_1000);

  Serial.print("LTR303 Initialization: ");
  Serial.println(light_sensor_isInitialized);
  
}

void loop() {
  static int test = 0;
  light_sensor.ltr303.readBothChannels(light_sensor.visible_ir_lux, light_sensor.ir_lux);

  Serial.print("Channel0: ");
  Serial.println(light_sensor.visible_ir_lux);

  Serial.println(light_sensor_cbuf->head);
  circular_buffer_add(light_sensor_cbuf, light_sensor.visible_ir_lux);
  Serial.println(light_sensor_cbuf->buffer[light_sensor_cbuf->head]);
  Serial.println(light_sensor_cbuf->sum);
  Serial.println(light_sensor_cbuf->average);

  delay(1000);
  test++;
}

