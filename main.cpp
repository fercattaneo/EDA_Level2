
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

	//Cambio de Ojos
	class MQTTMessage msj1;
	class MQTTMessage msj2;
	msj1.topic = "robot1/display/leftEye/set";
	msj1.payload = { 120, 0, 0 };
	msj2.topic = "robot1/display/rightEye/set";
	msj2.payload = { 120, 0, 0 };
	cliente.publish(msj1.topic, msj1.payload);
	cliente.publish(msj2.topic, msj2.payload);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		cliente.doFuntions();
		EndDrawing();
	}
	std::cout << "Termino" << std::endl;
	CloseWindow();
	return 0;
}
