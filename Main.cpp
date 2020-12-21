#include <iostream>
#include "libs/EdubotLib.hpp"

//Velocidad
#define SPEED 0.1

//Timeout cuando va en la direccion correcta
#define TIMEOUT 10

//Timeout de giro
#define TURNTIMEOUT 1600

//Timeout de avance post-giro (para avanzar un casillero completo)
#define POSTTURNTIMEOUT 4120


using namespace std;


int main(){
	EdubotLib *edubotLib = new EdubotLib();

	//Variables
	int turnCounter = 0;
	int targetDir = 2;

	//Conectar robot
	if(edubotLib->connect()){
		while(edubotLib->isConnected()){
			//Yendo en la direccion correcta
			if(turnCounter==targetDir){
				//Si no hay obstaculos
				if(edubotLib->getSonar(3)>0.05){
					edubotLib->move(SPEED);
					edubotLib->sleepMilliseconds(TIMEOUT);
					edubotLib->stop();
				}
			//Priorizar derecha
				else if(edubotLib->getSonar(6)>0.3){
						edubotLib->rotate(90);
						edubotLib->sleepMilliseconds(TURNTIMEOUT);
						turnCounter += 1;
						edubotLib->move(SPEED);
						edubotLib->sleepMilliseconds(POSTTURNTIMEOUT);
						edubotLib->stop();
					}
			}
			//Si va en otra direccion
			else{
				//Priorizar derecha
				if(edubotLib->getSonar(6)>0.3){
					edubotLib->rotate(90);
					edubotLib->sleepMilliseconds(TURNTIMEOUT);
					turnCounter += 1;
					edubotLib->move(SPEED);
					edubotLib->sleepMilliseconds(POSTTURNTIMEOUT);
					edubotLib->stop();
				}
				//Si la derecha esta bloqueada, probar izquierda
				else if(edubotLib->getSonar(0)>0.3){
					edubotLib->rotate(-90);
					edubotLib->sleepMilliseconds(TURNTIMEOUT);
					turnCounter -= 1;
					edubotLib->move(SPEED);
					edubotLib->sleepMilliseconds(POSTTURNTIMEOUT);
					edubotLib->stop();
				}
				//Si no hay alternativa (pasillo), avanzar
				else{
					edubotLib->move(SPEED);
					edubotLib->sleepMilliseconds(POSTTURNTIMEOUT);
					edubotLib->stop();
				}
			}

			std::cout << "Contador de giro: " << turnCounter <<std::endl;
			}

		
	}
	else{
		std::cout << "No se pudo conectar al robot!" <<std::endl;
	}
	
	return 0;
}
