
#include "ZUNO_DS18B20.h"

word temp_in_c;

// Never sleep
ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_ALWAYS_AWAKE);

ZUNO_SETUP_CHANNELS(
        ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_TEMPERATURE, SENSOR_MULTILEVEL_SCALE_CELSIUS, SENSOR_MULTILEVEL_SIZE_TWO_BYTES, SENSOR_MULTILEVEL_PRECISION_ONE_DECIMAL, getterTemp)
);

float f_to_c(float f)
{
  return (f - 32.0) * (5.0/9.0);
}

void setup() {

  // Start up the serial port
  Serial.begin(9600);

  delay(5000);
  Serial.println("Hello!");

}

void loop() {

  while (Serial.available() > 0) {
  
    temp_in_c = f_to_c(Serial.parseFloat()) * 100;

    if (Serial.read() == '\n')
    {
      Serial.println(temp_in_c);
      zunoSendReport(1);
    }
  }

}

word getterTemp()
{
  return temp_in_c;
}
