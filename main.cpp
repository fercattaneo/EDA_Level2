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
	InitWindow(500, 200, "EDA PARK");
	SetTargetFPS(10);

	//Inicializacion del cliente
	MQTTClient client("controller");
	client.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez");
	if (!client.isConnected()) {
		std::cout << "Error de conexión" << std::endl;
		return 1;
	}
	std::cout << "El cliente esta conectado" << std::endl;

	//Suscripciones a lo que se desea mostrar en pantalla
	client.subscribe("robot1/power/batteryLevel");
	client.subscribe("robot1/motor1/voltage");
	client.subscribe("robot1/motor1/temperature");

	//Se encienden los ojos
	client.setEyes();

	//Actualiza el robot de acuerdo a las teclas presionadas
	//Tambien se actualiza el display con la informacion que se muestra
	while (!WindowShouldClose()) { 
		BeginDrawing();
		ClearBackground(BLANK);
		auto messages = client.getMessages();
		if(!(messages.empty())){
			for(auto &i : messages){
				if (i.topic == "robot1/power/batteryLevel"){
					DrawText(TextFormat("Nivel de bateria: %f", client.getString(i.payload)), 10, 50, 20, WHITE);
				}
				else if (i.topic == "robot1/motor1/voltage"){
					DrawText(TextFormat("Voltaje del motor 1: %f", client.getString(i.payload)), 10, 100, 20, WHITE);
				}
				else if (i.topic == "robot1/motor1/temperature"){
					DrawText(TextFormat("Temperatura del motor 1: %f", client.getString(i.payload)), 10, 150, 20, WHITE);
				}
			}
		}
		client.moveMotors();
		client.setKickerChipper();
		client.setDribbler();
		EndDrawing();
	}
	//Aviso de fin de programa
	std::cout << "Controlador Finalizado, Gracias por utilizar el EDABot" << std::endl; 
	CloseWindow();
	return 0;
}
