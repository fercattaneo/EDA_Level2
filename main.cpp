/*
* Hecho por Fernanda Cattaneo y Franco Grippino
* EDA Level 2
* Main Module
*/

#include "MQTTClient.h"
#include <iostream>
#include <vector>
#include "raylib-cpp.hpp"

int main(void) {

	//Inicializacion Ventana
	InitWindow(500, 500, "EDA PARK");
	SetTargetFPS(60);

	//Inicializacion del cliente
	MQTTClient cliente("controller");
	cliente.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez");
	if (!cliente.isConnected())
		std::cout << "El cliente esta conectado" << std::endl;

	//Se encienden los ojos
	cliente.setEyes();

	while (!WindowShouldClose()) { //Actualiza los motores de acuerdo a las teclas presionadas
		BeginDrawing();
		ClearBackground(BLACK);
		cliente.moveMotors();
		cliente.setKickerChipper();
		cliente.setDribbler();
		EndDrawing();
	}
	std::cout << "Termino" << std::endl; //Imprime cuando se cerró la ventana de raylib
	CloseWindow();
	return 0;
}
