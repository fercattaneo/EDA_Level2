
#include "MQTTClient.h"
#include <iostream>
#include <vector>
#include <raylib.h>

int main (void){

    //Inicializacion
    MQTTClient cliente("controller");

    cliente.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez");
    int var1 = cliente.isConnected();
    if(var1!=0){
        std::cout << "El cliente esta conectado" << std::endl;
    }
    cliente.subscribe("robot1/display/leftEye/set");
    cliente.subscribe("robot1/display/rightEye/set");
    cliente.subscribe("robot1/motor1/current/set");
    cliente.subscribe("robot1/motor2/current/set");

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
        if(IsKeyDown(KEY_UP)){
            class MQTTMessage mensaje1, mensaje2;
            mensaje1.topic = "robot1/motor1/current/set";
            mensaje1.payload = cliente.getArrayFromFloat(1.0F);
            cliente.publish(mensaje1.topic, mensaje1.payload);
            mensaje2.topic = "robot1/motor3/current/set";
            mensaje2.payload = cliente.getArrayFromFloat(1.0F);
            cliente.publish(mensaje2.topic, mensaje2.payload);
        }
        if(IsKeyDown(KEY_DOWN)){
            class MQTTMessage mensaje1, mensaje2;
            mensaje1.topic = "robot1/motor1/current/set";
            mensaje1.payload = cliente.getArrayFromFloat(-1.0F);
            cliente.publish(mensaje1.topic, mensaje1.payload);
            mensaje2.topic = "robot1/motor3/current/set";
            mensaje2.payload = cliente.getArrayFromFloat(-1.0F);
            cliente.publish(mensaje2.topic, mensaje2.payload);
        }
        if(IsKeyDown(KEY_RIGHT)){
            class MQTTMessage mensaje1, mensaje2;
            mensaje1.topic = "robot1/motor1/current/set";
            mensaje1.payload = cliente.getArrayFromFloat(1.0F);
            mensaje2.topic = "robot1/motor3/current/set";
            mensaje2.payload = cliente.getArrayFromFloat(-1.0F);
            cliente.publish(mensaje2.topic, mensaje2.payload);
        }
        if(IsKeyDown(KEY_LEFT)){
            class MQTTMessage mensaje1, mensaje2;
            mensaje1.topic = "robot1/motor1/current/set";
            mensaje1.payload = cliente.getArrayFromFloat(-1.0F);
            mensaje2.topic = "robot1/motor3/current/set";
            mensaje2.payload = cliente.getArrayFromFloat(1.0F);
            cliente.publish(mensaje2.topic, mensaje2.payload);
        }
    }
    return 0;
}
