/*
 * Simple MQTT Client
 * Editado por: Fernanda Cattaneo y Franco Grippino
 * EDA Level 2
 * Copyright (C) 2022 Marc S. Ressl
 *
 * libmosquitto documentation:
 * https://mosquitto.org/api/files/mosquitto-h.html
 */

#include <cstring>
#include <iostream>
#include <assert.h>
#include "raylib-cpp.hpp"
#include "raymath.h"

// Definicion de Constantes:
//
// LIMIT_CURRENT: Corriente maxima dada a los motores,
// evita la sobreexigencia de los motores q llevan a quemarlos
//
// VECTOR_MOV_(i): Vector columna de la matriz de movimiento para motores
// KICKER_CHARGEVOLTAGE & KICKER_KICK & KICKER_CHIP: Kicker constants
// 
// DRIBBLER_CURRECT: Dribbler constant

#define LIMIT_CURRENT 5.0F

#define VECTOR_MOV_1 \
	{                \
		-1.0F, 1.0F  \
	}
#define VECTOR_MOV_2 \
	{                \
		-1.0F, -1.0F \
	}
#define VECTOR_MOV_3 \
	{                \
		1.0F, -1.0F  \
	}
#define VECTOR_MOV_4 \
	{                \
		1.0F, 1.0F   \
	}

#define KICKER_CHARGEVOLTAGE 100.0F
#define KICKER_KICK 0.5F
#define KICKER_CHIP 0.5F

#define DRIBBLER_CURRECT 0.5F

static void onMQTTMessage(struct mosquitto *mosquittoClient,
						  void *context,
						  const struct mosquitto_message *message);

#include "MQTTClient.h"

using namespace std;

// Awful but necessary global variable:
static bool isMosquittoInitialized = false;

// MQTT message callback.
static void onMQTTMessage(struct mosquitto *mosquittoClient,
						  void *context,
						  const struct mosquitto_message *message)
{
	MQTTClient *mqttClient = (MQTTClient *)context;

	MQTTMessage mqttMessage;
	mqttMessage.topic = message->topic;
	mqttMessage.payload.resize(message->payloadlen);
	memcpy(mqttMessage.payload.data(), message->payload, message->payloadlen);

	mqttClient->lastMessages.push_back(mqttMessage);
}

/*
 * MQTTClient::connect
 *
 * Constructs an MQTT client object.
 *
 * Parameters:
 *  clientId -   MQTT client identifier
 */
MQTTClient::MQTTClient(string clientId)
{
	if (!isMosquittoInitialized)
	{
		mosquitto_lib_init();

		isMosquittoInitialized = true;
	}

	const bool cleanSession = true;

	mosquittoInstance = mosquitto_new(clientId.c_str(), cleanSession, this);

	mosquitto_message_callback_set(mosquittoInstance, onMQTTMessage);

	connected = false;
}

MQTTClient::~MQTTClient()
{
	mosquitto_destroy(mosquittoInstance);
}

/*
 * MQTTClient::connect
 *
 * Connects to an MQTT server without encryption.
 *
 * Parameters:
 *  host -       Host to connect to (IP address or domain name)
 *  username -   MQTT username
 *  password -   MQTT password
 */
bool MQTTClient::connect(string host, int port, string username, string password)
{
	int errorCode;

	mosquitto_username_pw_set(mosquittoInstance,
							  username.c_str(),
							  password.c_str());

	const int keepalive = 60;

	errorCode = mosquitto_connect(mosquittoInstance,
								  host.c_str(),
								  port,
								  keepalive);

	if (errorCode == MOSQ_ERR_SUCCESS)
		connected = true;

	return connected;
}

/*
 * MQTTClient::isConnected
 *
 * Tells whether the connection is up.
 */
bool MQTTClient::isConnected()
{
	return connected;
}

/*
 * MQTTClient::disconnect
 *
 * Disconnects from the MQTT server.
 */
void MQTTClient::disconnect()
{
	mosquitto_disconnect(mosquittoInstance);

	connected = false;
}

/*
 * MQTTClient::publish
 *
 * Publishes an MQTT message to the server.
 *
 * Parameters:
 *  topic -     The MQTT topic
 *  payload -   The data to be sent
 *
 * Returns: call succeeded
 */
bool MQTTClient::publish(string topic, vector<char> &payload)
{
	const int qos = 0;
	const bool retain = false;

	int errorCode = mosquitto_publish(mosquittoInstance,
									  NULL,
									  topic.c_str(),
									  (int)payload.size(),
									  payload.data(),
									  qos,
									  retain);

	if (errorCode == MOSQ_ERR_NO_CONN)
		connected = false;

	return (errorCode == MOSQ_ERR_SUCCESS);
}

/*
 * MQTTClient::subscribe
 *
 * Sends an MQTT subscription request.
 * Topic may be straight or contain wildcards ('+' for any topic, '#' for anything below in tree).
 *
 * Parameters:
 *  topic -     The MQTT topic
 *
 * Returns: call succeeded
 */
bool MQTTClient::subscribe(string topic)
{
	const int qos = 0;

	int errorCode = mosquitto_subscribe(mosquittoInstance,
										NULL,
										topic.c_str(),
										qos);

	if (errorCode == MOSQ_ERR_NO_CONN)
		connected = false;

	return (errorCode == MOSQ_ERR_SUCCESS);
}

/*
 * MQTTClient::unsubscribe
 *
 * Sends an MQTT unsubscription request. Should match a previous subscription request.
 *
 * Parameters:
 *  topic -     The MQTT topic
 *
 * Returns: call succeeded
 */
bool MQTTClient::unsubscribe(string topic)
{
	int errorCode = mosquitto_unsubscribe(mosquittoInstance,
										  NULL,
										  topic.c_str());

	if (errorCode == MOSQ_ERR_NO_CONN)
		connected = false;

	return (errorCode == MOSQ_ERR_SUCCESS);
}

/*
 * MQTTClient::getMessages
 *
 * Retrieve latest messages. Should be called quite frequently so messages do not accumulate.
 *
 * Returns: the MQTT messages
 */
vector<MQTTMessage> MQTTClient::getMessages()
{
	vector<MQTTMessage> messages;

	while (true)
	{
		const int timeout = 0;
		const int maxPackets = 1;

		int errorCode = mosquitto_loop(mosquittoInstance, timeout, maxPackets);

		if ((errorCode == MOSQ_ERR_NO_CONN) ||
			(errorCode == MOSQ_ERR_CONN_LOST))
			connected = false;

		if (!lastMessages.size())
			break;

		// Moves lastMessages to messages
		messages.insert(messages.end(), lastMessages.begin(), lastMessages.end());
		lastMessages.clear();
	}

	return messages;
}

// Funcion para trasnformar un float a vector de char.
std::vector<char> MQTTClient::getArrayFromFloat(float payload)
{
	std::vector<char> data(sizeof(float));

	// Extraído de: https://www.cplusplus.com/reference/cstring/memcpy/
	memcpy(data.data(), &payload, sizeof(float));

	return data;
}

// Funcion para convertir un vector de char a un float
float MQTTClient::getFloat(std::vector<char> vec)
{
	float value = 0.0;

	// Extraído de: https://www.cplusplus.com/reference/cassert/assert/
	assert(vec.size() == sizeof(value));

	memcpy(&value, &vec[0], std::min(vec.size(), sizeof(float)));
	return value;
}

/*Función de movimiento del robot, a travez de los 4 motores
 *
 * Esta función envia los valores de corriente a cada motor,
 * para obtenerlos se utiliza una transformación lineal
 * sobre la dirección deseada.
 * Ademas utiliza un coeficiente de rotación que permite
 * modificar la matriz de transformación y asi lograr rotar al robot.
 *
 * Normalizando el vector resultante y luego escalandolo
 * por el valor limite permite mantener limitado a los motores
 * y evitar que se quemen
 */
void MQTTClient::moveMotors()
{
	// Calculo de corriente sobre los motores
	raylib::Vector2 direction(IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT),
							  IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN));

	int rotationCoef = IsKeyDown(KEY_SPACE);

	raylib::Vector4 motor(direction.DotProduct(VECTOR_MOV_1),
						  direction.DotProduct(VECTOR_MOV_2) * (1 - rotationCoef),
						  direction.DotProduct(VECTOR_MOV_3),
						  direction.DotProduct(VECTOR_MOV_4) * (1 + rotationCoef));

	// Creación y Publicación de los mensajes
	MQTTMessage msj1, msj2, msj3, msj4;
	msj1.topic = "robot1/motor1/current/set";
	msj2.topic = "robot1/motor2/current/set";
	msj3.topic = "robot1/motor3/current/set";
	msj4.topic = "robot1/motor4/current/set";

	msj1.payload = getArrayFromFloat(LIMIT_CURRENT * motor.Normalize().x);
	msj2.payload = getArrayFromFloat(LIMIT_CURRENT * motor.Normalize().y);
	msj3.payload = getArrayFromFloat(LIMIT_CURRENT * motor.Normalize().z);
	msj4.payload = getArrayFromFloat(LIMIT_CURRENT * motor.Normalize().w);

	publish(msj1.topic, msj1.payload);
	publish(msj2.topic, msj2.payload);
	publish(msj3.topic, msj3.payload);
	publish(msj4.topic, msj4.payload);
}

// Funcion que enciende los ojos del robot
void MQTTClient::setEyes()
{
	class MQTTMessage msj1, msj2;
	msj1.topic = "robot1/display/leftEye/set";
	msj1.payload = {120, 0, 0};
	msj2.topic = "robot1/display/rightEye/set";
	msj2.payload = {120, 0, 0};
	publish(msj1.topic, msj1.payload);
	publish(msj2.topic, msj2.payload);
}

// Función para hacer funcionar el Kricker y el Chipper.
// Estos se activan con la tecla ENTER
void MQTTClient::setKickerChipper()
{
	class MQTTMessage msj1, msj2, msj3;
	if (IsKeyDown(KEY_ENTER))
	{
		msj1.topic = "robot1/kicker/chargeVoltage/set";
		msj1.payload = getArrayFromFloat(KICKER_CHARGEVOLTAGE);
		publish(msj1.topic, msj1.payload);
		msj2.topic = "robot1/kicker/kick/cmd";
		msj2.payload = getArrayFromFloat(KICKER_KICK);
		publish(msj2.topic, msj2.payload);
		msj3.topic = "robot1/kicker/chip/cmd";
		msj3.payload = getArrayFromFloat(KICKER_CHIP);
		publish(msj3.topic, msj3.payload);
	}
}

// Función para encender el Dribbler. Se enciende con la tecla
// Blockspace y se apaga con la tecla M
void MQTTClient::setDribbler()
{
	class MQTTMessage msj1;
	if (IsKeyDown(KEY_BACKSPACE))
	{
		msj1.topic = "robot1/dribbler/current/set";
		msj1.payload = getArrayFromFloat(DRIBBLER_CURRECT);
		publish(msj1.topic, msj1.payload);
	}
	if (IsKeyDown(KEY_M))
	{
		msj1.topic = "robot1/dribbler/voltage/set";
		msj1.payload = getArrayFromFloat(0.0F);
		publish(msj1.topic, msj1.payload);
	}
}