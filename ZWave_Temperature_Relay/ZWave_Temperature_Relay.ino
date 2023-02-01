
#include "ZUNO_DS18B20.h"

#define BUFFER_SIZE 64

#define VERBOSE_LOGGING 0
#define USE_UART_0_FOR_DEBUG 0

word temp_in_c;

// Generate a new product ID
ZUNO_SETUP_PRODUCT_AUTO();

// Never sleep
ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_ALWAYS_AWAKE);

ZUNO_SETUP_CHANNELS(
    ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_TEMPERATURE, SENSOR_MULTILEVEL_SCALE_CELSIUS, SENSOR_MULTILEVEL_SIZE_TWO_BYTES, SENSOR_MULTILEVEL_PRECISION_ONE_DECIMAL, getterTemp));

float f_to_c(float f)
{
    float c;
    c = (f - 32) / 1.8;
    return c;
}
void setup()
{

    // Start up the serial port
    Serial.begin(9600);
    Serial1.begin(9600);

    delay(5000);
    Serial.println("Hello! I'm alive! <3");

#ifdef USE_UART_0_FOR_DEBUG
    Serial0.begin(9600);
    Serial0.println("Hello! I'm alive! <3");
#endif
}

byte buffer[BUFFER_SIZE];
int currentByte = 0;

void loop()
{

    while (Serial1.available() > 0)
    {

        byte b = Serial1.read();

        if (currentByte < BUFFER_SIZE)
        {
            if (b != 10)
            {
                buffer[currentByte] = b;

#ifdef VERBOSE_LOGGING
                Serial.print(b);
#endif
#ifdef USE_UART_0_FOR_DEBUG
                Serial0.print(b);
#endif

                currentByte++;
            }
            else
            {
                // Convert to a string
                char str[currentByte + 1];
                memcpy(str, buffer, currentByte);
                str[sizeof currentByte] = 0; // Null termination.

                char *ending;
                float value = strtof(str, &ending);
                if (*ending != 0)
                {
                    Serial.println("ERR: Uh oh, didn't convert to float. Doing nothing.");
                    Serial0.println("ERR: Uh oh, didn't convert to float. Doing nothing.");
                }
                else
                {
                    temp_in_c = f_to_c(value) * 100;
                    Serial.print("Data Read: ");
                    Serial.print(value);
                    Serial.print(" (");
                    Serial.print(temp_in_c);
                    Serial.println(")");

#ifdef USE_UART_0_FOR_DEBUG
                    Serial0.print("Data Read: ");
                    Serial0.print(value);
                    Serial0.print(" (");
                    Serial0.print(temp_in_c);
                    Serial0.println(")");
#endif

                    zunoSendReport(1);
                }

                currentByte = 0;
            }
        }
        else
        {
            Serial.println("Buffer is full without a CR. Starting over.");
            currentByte = 0;
        }
    }
}

word getterTemp()
{
    return temp_in_c;
}
