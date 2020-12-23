#include <iostream>
#include "libs/EdubotLib.hpp"

//Velocidad
#define SPEED 0.5

//Timeout cuando va en la direccion correcta
#define TIMEOUT 20

//Timeout de giro
#define TURNTIMEOUT 1600

//Timeout de avance post-giro (para avanzar un casillero completo)
#define POSTTURNTIMEOUT 4120

//Distancia minima a la pared
#define DISTANCE 0.3

using namespace std;

void advance(int sonar, double speed, double distance, double timeout, EdubotLib*& edubotLib,bool hallway){
	edubotLib->move(speed);
	while(edubotLib->getSonar(sonar)<=distance){
		}
	//Caso 1: está avanzando hacia la dirección
	if(sonar==3){
		//edubotLib->stop();
	}
	//Caso 2: despejar pasillo
	else if(hallway==true){
		//Caso 2.1: pasillo vertical
		if(edubotLib->getTheta() == 90 || edubotLib->getTheta() == 270){
			double startY = edubotLib->getY();
			edubotLib->move(speed);
			while(abs(startY-edubotLib->getY()) <= 0.13){
			}
			edubotLib->stop();
		}
		//Caso 2.2: pasillo horizontal
		else{
			double startX = edubotLib->getX();
			edubotLib->move(SPEED);
			while(abs(startX-edubotLib->getX()) <= 0.13){
			}
			edubotLib->stop();
		}
	}
	//Caso 3: Despejar una esquina vertical
	else if(edubotLib->getTheta() == 90 || edubotLib->getTheta() == 270){
		double startY = edubotLib->getY();
		edubotLib->move(speed);
		while(abs(startY-edubotLib->getY()) <= 0.35){
		}
		edubotLib->stop();
	}
	//Caso 5: Despejar esquina horizontal
	else{
		double startX = edubotLib->getX();
		edubotLib->move(SPEED);
		while(abs(startX-edubotLib->getX()) <= 0.35){
		}
		edubotLib->stop();
	}
	edubotLib->sleepMilliseconds(timeout);
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
				if(edubotLib->getSonar(3)>0.1){
					advance(3,SPEED,0.1,TIMEOUT,edubotLib,false);
				}
			//Priorizar derecha
				else if(edubotLib->getSonar(6)>0.3){
						edubotLib->rotate(90);
						edubotLib->sleepMilliseconds(TURNTIMEOUT);
						turnCounter += 1;
						advance(6,SPEED,DISTANCE,TIMEOUT,edubotLib,false);
					}
			}
			//Si va en otra direccion
			else{
				//Priorizar derecha
				if(edubotLib->getSonar(6)>0.3){
					edubotLib->rotate(90);
					edubotLib->sleepMilliseconds(TURNTIMEOUT);
					turnCounter += 1;
					advance(6,SPEED,DISTANCE,TIMEOUT,edubotLib,false);
				}
				//Si la derecha esta bloqueada, probar izquierda
				else if(edubotLib->getSonar(0)>0.3){
					edubotLib->rotate(-90);
					edubotLib->sleepMilliseconds(TURNTIMEOUT);
					turnCounter -= 1;
					advance(0,SPEED,DISTANCE,TIMEOUT,edubotLib,false);
				}
				//Si no hay alternativa (pasillo), avanzar
				else{
					advance(6,SPEED,DISTANCE,TIMEOUT,edubotLib,true);
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
