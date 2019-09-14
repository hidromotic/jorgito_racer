#include "led_test.h"

void LedTest(void)
	{
	static unsigned long ms_ant=0;
	static bool estado=0;
	static unsigned int espera=TPO_LED_OFF;

	if(millis()-ms_ant < espera) return;
	ms_ant=millis();

	estado=!estado;

	if(estado)  
		{
		ENCENDER_LED_TEST;
		espera=TPO_LED_ON;
		}
	else 
		{
		APAGAR_LED_TEST;
		espera=TPO_LED_OFF;
		}	
	}