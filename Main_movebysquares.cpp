#include <iostream>
#include "libs/EdubotLib.hpp"
#include <cmath> 

//Velocidad
#define SPEED 0.5

//Timeout cuando va en la direccion correcta
#define TIMEOUT 100

//Timeout de giro
#define TURNTIMEOUT 2000

//Largo de cada casillero
#define LENGTH 0.365


using namespace std;

void advanceSquare(double length, EdubotLib*& edubotLib, double speed){
	double startX=edubotLib->getX();
	double startY=edubotLib->getY();
	double startTheta=edubotLib->getTheta();

	//Caso 1: movimiento vertical
	if(startTheta==90 || startTheta==270){
		edubotLib->move(SPEED);
		while(abs(startY-edubotLib->getY()) <= length){
		}
		edubotLib->stop();
		edubotLib->sleepMilliseconds(50);
	}
	else{
		edubotLib->move(SPEED);
		while(abs(startX-edubotLib->getX()) <= length){
		}
		edubotLib->stop();
		edubotLib->sleepMilliseconds(50);
	}
	
	
}


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
				if(edubotLib->getSonar(3)>0.4){
					advanceSquare(LENGTH,edubotLib,SPEED);
					edubotLib->sleepMilliseconds(TIMEOUT);
				}
			//Priorizar derecha
				else if(edubotLib->getSonar(6)>0.4){
						edubotLib->rotate(90);
						edubotLib->sleepMilliseconds(TURNTIMEOUT);
						turnCounter += 1;
						advanceSquare(LENGTH,edubotLib,SPEED);
						edubotLib->sleepMilliseconds(TIMEOUT);
					}
			}
			//Si va en otra direccion
			else{
				//Priorizar derecha
				if(edubotLib->getSonar(6)>0.4){
					edubotLib->rotate(90);
					edubotLib->sleepMilliseconds(TURNTIMEOUT);
					turnCounter += 1;
					advanceSquare(LENGTH,edubotLib,SPEED);
					edubotLib->sleepMilliseconds(TIMEOUT);
				}
				//Si la derecha esta bloqueada, probar izquierda
				else if(edubotLib->getSonar(0)>0.4){
					edubotLib->rotate(-90);
					edubotLib->sleepMilliseconds(TURNTIMEOUT);
					turnCounter -= 1;
					advanceSquare(LENGTH,edubotLib,SPEED);
					edubotLib->sleepMilliseconds(TIMEOUT);
				}
				//Si no hay alternativa (pasillo), avanzar
				else{
					advanceSquare(LENGTH,edubotLib,SPEED);
					edubotLib->sleepMilliseconds(TIMEOUT);
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

