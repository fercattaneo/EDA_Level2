
#include "MQTTClient.h"
#include <iostream>
#include <vector>
#include "raylib-cpp.hpp"

int main (void){

    //Inicializacion
    InitWindow(300, 300, "EDA PARK");

    MQTTClient cliente("controller");

    cliente.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez");
    int var1 = cliente.isConnected();
    if(var1!=0){
        std::cout << "El cliente esta conectado" << std::endl;
    }
    SetTargetFPS(60);

    cliente.subscribe("robot1/display/leftEye/set");
    cliente.subscribe("robot1/display/rightEye/set");
    cliente.subscribe("robot1/motor1/current/set");
    cliente.subscribe("robot1/motor3/current/set");

    //Trabajo con los motores.
    class MQTTMessage mensaje;
    class MQTTMessage mensaje2;
    
    mensaje.topic = "robot1/display/leftEye/set";
    mensaje.payload = {120, 0, 0};
    cliente.publish(mensaje.topic, mensaje.payload);
    mensaje2.topic = "robot1/display/rightEye/set";
    mensaje2.payload = {120, 0, 0};
    cliente.publish(mensaje2.topic, mensaje2.payload);

     while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK); 
        cliente.doFuntions();
        EndDrawing();
    }
    std::cout << "Termino" << std::endl;
    CloseWindow();
    return 0;
}
