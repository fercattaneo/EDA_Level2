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
	if (!cliente.isConnected()) {
		std::cout << "Error de conexión" << std::endl;
		return 1;
	}
	std::cout << "El cliente esta conectado" << std::endl;

	//Se encienden los ojos
	cliente.setEyes();

	//Actualiza el robot de acuerdo a las teclas presionadas
	while (!WindowShouldClose()) { 
		BeginDrawing();
		ClearBackground(BLANK);
		cliente.moveMotors();
		cliente.setKickerChipper();
		cliente.setDribbler();
		EndDrawing();
	}
	//Aviso de fin de programa
	std::cout << "Controlador Finalizado, Gracias por utilizar el EDABot" << std::endl; 
	CloseWindow();
	return 0;
}
