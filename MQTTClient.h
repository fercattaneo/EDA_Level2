/*
 * Simple MQTT Client
 * Editado por: Cattaneo Fernanda y Franco Grippino
 * EDA Level 2
 * Copyright (C) 2022 Marc S. Ressl
 */

#include <string>
#include <vector>
#include <mosquitto.h>

class MQTTMessage
{
public:
	std::string topic;
	std::vector<char> payload;
};

class MQTTClient
{
public:
	MQTTClient(std::string clientId);
	~MQTTClient();

	bool connect(std::string host, int port, std::string username, std::string password);
	bool isConnected();
	void disconnect();

	bool publish(std::string topic, std::vector<char>& payload);
	bool subscribe(std::string topic);
	bool unsubscribe(std::string topic);

	void moveMotors();
	void setEyes();
	void setKickerChipper();
	void setDribbler();

	std::vector<MQTTMessage> getMessages();
	std::vector<char> getArrayFromFloat(float payload);
	float getString(std::vector<char> vector);

private:
	struct mosquitto* mosquittoInstance;
	bool connected;

	std::vector<MQTTMessage> lastMessages;

	friend void onMQTTMessage(struct mosquitto* mosquittoClient,
		void* context,
		const struct mosquitto_message* message);
};
