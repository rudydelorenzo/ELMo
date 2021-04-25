/*
ELMo - v1.1.2
WARNING - NEEDS WiFi.h Library

TODO: ADD DESCRIPTION
TODO: MAKE CLIENT PRIVATE
TODO: THROW EXCEPTIONS
TODO: REMOVE/BETTER COMMENTS

*/
#ifndef ELMo_h
#define ELMo_h
#include <Arduino.h>
#include <WiFi.h>

class ELMo {
private:
	bool _initialized;
	String _sendUninitialized(String message);
	bool _debug;
	int _baudRate;
	int _responseTimeout;

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
	int getTimeout();
	int setTimeout(int seconds);

	WiFiClient client;
	IPAddress ip;
	int port;
};

#endif