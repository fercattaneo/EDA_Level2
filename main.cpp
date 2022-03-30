
#include "MQTTClient.h"
#include <iostream>
#include <vector>
#include "raylib-cpp.hpp"

int main(void) {

	//Inicializacion Ventana
	InitWindow(300, 300, "EDA PARK");
	SetTargetFPS(60);

	//Inicializacion del cliente
	MQTTClient cliente("controller");
	cliente.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez");
	if (!cliente.isConnected())
		std::cout << "El cliente esta conectado" << std::endl;

	//Se encienden los ojos
	cliente.setEyes();

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		cliente.moveMotors();
		EndDrawing();
	}
	std::cout << "Termino" << std::endl;
	CloseWindow();
	return 0;
}
