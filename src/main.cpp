#include <Arduino.h>
#include "esp32-hal-ledc.h"
#include "main.h"
#include "light.h"

#define LEDC_CHANNEL_0  0
#define LEDC_FREQUENCY_HZ 100000
#define LEDC_TIMER_2_BIT  2
#define LEDC_PIN  6

uint32_t soil_moisture;
LTR303_t light_sensor;
static float avg_light;

void setup() {

  //Initialize sensors (light, moisture, temperature)
  light_sensor = init_task_light_sensor(PIN_LTR303_SDA, PIN_LTR303_SCL);


  ledcSetup(LEDC_CHANNEL_0, LEDC_FREQUENCY_HZ, LEDC_TIMER_2_BIT);
  ledcAttachPin(LEDC_PIN, LEDC_CHANNEL_0);

  ledcWrite(LEDC_CHANNEL_0, 1);

  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  static int test = 0;
  static uint16_t ch0, ch1;
  //Read both channels (visible + ir & ir)
  // light_sensor.ltr303.readBothChannels(ch0, ch1);

  task_light_sensor(&light_sensor, &avg_light);

  // soil_moisture = analogRead(0);
  // Serial.print("Frequency: ");
  // Serial.print(ledcReadFreq(LEDC_CHANNEL_0));
  // Serial.print(" Duty: ");
  // Serial.print(ledcRead(LEDC_CHANNEL_0));
  // Serial.print(" Analog input: ");
  // Serial.println(soil_moisture);
  // Serial.print("CH0: ");
  // Serial.print(ch0);;
  // Serial.print("CH1: ");
  // Serial.println(ch1);

  delay(1000);
}

