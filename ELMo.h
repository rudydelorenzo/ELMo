/*

WARNING - NEEDS ESP8266WiFi.h Library

TODO: ADD DESCRIPTION
TODO: MAKE CLIENT PRIVATE
TODO: THROW EXCEPTIONS
TODO: REMOVE/BETTER COMMENTS

*/
#ifndef ELMo_h
#define ELMo_h
#include <Arduino.h>
#include <ESP8266WiFi.h>

class ELMo {
private:
	bool _initialized;
	String _sendUninitialized(String message);
	bool _debug;
	int _baudRate;

public:
	ELMo();
	String send(String message);
	bool initialize();
	bool stop();
	bool connected();
	bool getDebug();
	bool setDebug(bool value);
	int getBaud();
	int setBaud(int rate);

	WiFiClient client;
	IPAddress ip;
	int port;
	int responseTimeout;
};

#endif