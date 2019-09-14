#ifndef LED_TEST

#define LED_TEST
#include "Arduino.h"

//http://blog.theinventorhouse.org/usando-las-entradas-analogicas-de-arduino-como-digitales/
//A0 a A5 --> 14 a 19

//LED DE TESTEO
#define PIN_LED_TEST 13

#define CONFIGURAR_LED_TEST pinMode(PIN_LED_TEST, OUTPUT)
#define ENCENDER_LED_TEST digitalWrite(PIN_LED_TEST, HIGH)
#define APAGAR_LED_TEST digitalWrite(PIN_LED_TEST, LOW)

#define TPO_LED_ON 100
#define TPO_LED_OFF 1000

void LedTest(void);

#endif