
#include <AFMotor.h>
#include "led_test.h"

unsigned long cronometro = 0; // DEBUG
void debugVel(); void debugSens(); void debugErr(); void debugMap();

AF_DCMotor motorI(2); AF_DCMotor motorD(1); // Motores

int sI = 0; int sC = 0; int sD = 0; // Sensores
// float kP = 0.15; float kI = 0.004; float kD = 0.11; // PID
int err = 0; int errIx = 0; int errDx = 0; float senialErr = 0; int errAnt = 0; // Errores
int posicion = 0; unsigned long tiempo = 1;
int velI = 0; int velD = 0;
int setPoint = 0; int velMedia = 80; // Setear
int estadoAnt = 0;

int pulsador = 4;

float kP = 0.015; float kI = 0.0003; float kD = 0.2; // PID
#define PULSADOR_PRESIONADO (digitalRead(pulsador) == HIGH)

void setup() {
	//Led de Testeo
	Serial.begin(9600);
	motorI.setSpeed(50); motorD.setSpeed(50); motorI.run(FORWARD); motorD.run(FORWARD); //(FORWARD = ADELANTE) – (BACKWARD = ATRAS) – (RELEASE = DETENER)	
	pinMode(pulsador, INPUT);
	CONFIGURAR_LED_TEST;
	APAGAR_LED_TEST;
	motorI.setSpeed(0); motorD.setSpeed(0);
	}
	
/*
Utilizar el botón para cambiar de estado
Botón presionado por más de 2 segundos --> arranca al liberar ---> Destellar rápido indicando que está listo para arrancar
Un toque del botón --> Detiene el motor
*/
#define MODO_DETENIDO	0
#define MODO_LANZADOR	1
#define MODO_CARRERA	2

unsigned modo=MODO_DETENIDO;

void SupervisaBoton(void)
	{
	static bool pulsador_presionado_ant=0;
	static unsigned long ms_ini=0;
	//Debouncing (Anti-Rebote) ??? 

	//Si está en CARRERA y se presiona el botón --> pasar a modo DETENIDO
	//Si está en LANZADOR y se libera el botón --> pasar a modo CARRERA
	if(PULSADOR_PRESIONADO && !pulsador_presionado_ant)	//Se presionó el botón
		{
		ms_ini=millis(); 
		if(modo==MODO_CARRERA) modo=MODO_DETENIDO;
		}
	if(!PULSADOR_PRESIONADO && pulsador_presionado_ant)	//Se liberó el botón
		{
		if(millis()-ms_ini >2000) 	//Si pasaron más de 2 segundos
			{
			if(modo==MODO_LANZADOR)
				modo=MODO_CARRERA;
			}
		else
			modo=MODO_DETENIDO;
		}
	pulsador_presionado_ant=PULSADOR_PRESIONADO;
	}

void loop() {
	LedTest();
	// SupervisaBoton();
	// if(modo!=MODO_CARRERA) return;	//Queda a la espera del modo carrera

	if(PULSADOR_PRESIONADO){
		motorI.run(RELEASE); motorD.run(RELEASE);
		Serial.println("Apagado");
		return;
		}

	sI = analogRead(A0);
	sD = analogRead(A2);
	sI = constrain(map(sI, 160, 970, 0, 500), 0, 500);
  	sD = constrain(map(sD, 85, 750, 0, 500), 0, 500);
	
	posicion = sD - sI;
	err = setPoint - posicion;

	errIx = errIx + (err * tiempo);		//errIx + err
	errDx = (err - errAnt) / tiempo; 	//err
	senialErr = (kP*err) + (kI*errIx) + (kD*errDx);

	velI = (velMedia - senialErr);
	velD = (velMedia + senialErr);

	//Limito velocidad motor Izquierdo
	if( velI > 255)
		velI = 255;
	else if(velI < 0)
		velI = 0;
	//Limito velocidad motor Derecho
	if( velD > 255)
		velD = 255;
	else if(velD < 0)
		velD = 0;

	if(posicion < 0){ // Se va a la derecha
		estadoAnt = 1;
	}else if(posicion > 0){ // Se va a la izquierda
		estadoAnt = -1;
	}

	if(sI < 50 && sD < 50){ // Si se va a negro el auto, recuerdo por donde me sali
		if(estadoAnt > 0){
			motorI.setSpeed(20);		
			// motorD.setSpeed(60);
			motorD.setSpeed(velD);		
		}else if(estadoAnt < 0){
			// motorI.setSpeed(60);
			motorI.setSpeed(velI);
			motorD.setSpeed(20);
		}
	}else{
		motorI.setSpeed(velI);	motorD.setSpeed(velD);
	}


  	errAnt = err;
	// if(millis() - tiempo > 100)
	// 	tiempo = millis() - tiempo;

	//DEBUG
	return;
	if(millis() - cronometro < 500)
		return;

	cronometro = millis();
	debugMap();
	debugVel();
	debugErr();
	/* Serial.print("err: ");
	Serial.println(err);
	Serial.print("errIx: ");
	Serial.println(errIx);
	Serial.print("senialErr: ");
	Serial.println(senialErr);
	Serial.print("Pos: ");
	Serial.println(posicion);
	Serial.println("------"); */
}

void debugErr(){
	Serial.print("err: ");
	Serial.println(err);
	
	/* Serial.println();
	Serial.print("errIx: ");
	Serial.println(errIx); */

	/* Serial.println();
	Serial.print("errDx: ");
	Serial.println(errDx); */
	
	Serial.println();
	Serial.print("senialErr: ");
	Serial.println(senialErr);

}

void debugVel(){
	Serial.print("|| ");
	Serial.print(velI);
	Serial.print(" ||");
	
	Serial.print("------");

	Serial.print("|| ");
	Serial.print(velD);
	Serial.println(" ||");
	Serial.println();
}

void debugSens(){
	Serial.println("*******");
    Serial.print("sI: ");
    Serial.print(analogRead(A0));

    Serial.println("");
    Serial.print("sC: ");
    Serial.print(analogRead(A1));

    Serial.println("");
    Serial.print("sD: ");
    Serial.print(analogRead(A2));

	Serial.println("");
	Serial.print("Posicion: ");
	Serial.println(posicion);
}
void debugMap(){
	Serial.println("*******");
    Serial.print("sI: ");
    Serial.print(sI);

    /* Serial.println("");
    Serial.print("sC: ");
    Serial.print(analogRead(A1)); */

    Serial.print(" ---- ");
    Serial.print("sD: ");
    Serial.println(sD);

}

