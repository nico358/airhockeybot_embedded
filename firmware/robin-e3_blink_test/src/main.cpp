#include "Arduino.h"

int pin_state = HIGH;


/*
 * files defining pins
 * - Marlin/src/pins/stm32f1/pins_MKS_ROBIN_E3_common.h <- endstop pins found here
 * - Marlin/src/pins/stm32f1/pins_MKS_ROBIN.h <- LED pin found here
 *
 * #define LED_PIN                             PB2
 * X_STOP_PIN PA12
 * X_STOP_PIN PA12
 * Y_STOP_PIN PA11
 * Z_MIN_PIN PC6
 * Z_MAX_PIN PB1
 */

int pin = PB2;

void setup() {
	pinMode(pin, OUTPUT);
}

void loop() {
	digitalWrite(pin, LOW);
	delay(1000);
	digitalWrite(pin, HIGH);
	delay(1000);
}
