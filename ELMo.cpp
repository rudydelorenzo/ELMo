#include <Arduino.h>
#include <ELMo.h>

#define TIMEOUT "ELM TIMEOUT"
#define NO_DATA "NO DATA"
#define NOT_INITIALIZED "NOT INITIALIZED"

ELMo::ELMo() {
	ip = IPAddress(192, 168, 0, 10);
	port = 35000;
	_debug = false;
	_responseTimeout = 60;
	_initialized = false;
	_baudRate = 115200;
}

String ELMo::send(String message) {
	if (!_initialized) return NOT_INITIALIZED;

	client.println(message);
  
	String received = "";
	unsigned long beginTime = millis();
	while (millis() - beginTime < (_responseTimeout*1000)) {
	    if (client.available()) {
	        char c = client.read();
	        if (c == '>') {
		        // done receiving
		        if (_debug) {
		            Serial.print("ELMo: Received: ");
		            Serial.println(received);
		        }
		        // check that response is adequate
		        if (message.indexOf("AT") == -1) {
		            // message is not AT
		            if (received.indexOf(NO_DATA) > -1) return NO_DATA;
	        	}
	        	return received;
	        }
	        received += c;
	    }
	}
	
	// timed out
	return TIMEOUT;
}

// private, to be used only to initialize
String ELMo::_sendUninitialized(String message) {
	client.println(message);
  
	String received = "";
	unsigned long beginTime = millis();
	while (millis() - beginTime < (_responseTimeout*1000)) {
	    if (client.available()) {
	        char c = client.read();
	        if (c == '>') {
		        // done receiving
		        if (_debug) {
		            Serial.print("ELMo: Received: ");
		            Serial.println(received);
		        }
		        // check that response is adequate
		        if (message.indexOf("AT") == -1) {
		            // message is not AT
		            if (received.indexOf(NO_DATA) > -1) return NO_DATA;
	        	}
	        	return received;
	        }
	        received += c;
	    }
	}
	
	// timed out
	return TIMEOUT;
}

bool ELMo::initialize() {
	_initialized = false;
	if (client.connect(ip, port)) {
	    if (_debug) Serial.println("ELMo: Connected to ELM327");
	    _sendUninitialized("AT Z");
	    _sendUninitialized("AT E0"); // disables echo
	    _sendUninitialized("AT H0"); // disables headers
	    if (!_sendUninitialized("AT SP 0").substring(0, 2).equals("OK")) return false;
	    if (_sendUninitialized("0100").indexOf("UNABLE TO CONNECT") > -1) return false; // queries for supported PIDs and prepares ELM

	    if (_debug) Serial.println("ELMo: Initialized ELM327");
	}
	else {
	    if (_debug) Serial.println("ELMo: Connection to ELM failed");
	    return false;
	}
	_initialized = true;
	return true;
}

bool ELMo::getDebug() {
	return _debug;
}

bool ELMo::setDebug(bool value) {
	if (value == false) {
		_debug = false;
		return _debug;
	} else {
		if (!Serial) {
			Serial.begin(_baudRate);
			unsigned long startTime = millis();

		  	while (!Serial && ((millis() - startTime) < 10000));

		  	if (!Serial) {
		  		_debug = false;
		  		return _debug;
		  	} else {
		  		_debug = true;
		  		return _debug;
		  	}
		} else {
			_debug = true;
			return _debug;
		}
		
	}
	
}

int ELMo::getBaud() {
	return _baudRate;
}

int ELMo::setBaud(int rate) {
	if (!Serial) {
		_baudRate = rate;
		return _baudRate;
	} else {
		Serial.end();

		_baudRate = rate;
		Serial.begin(_baudRate);
		unsigned long startTime = millis();

	  	while (!Serial && ((millis() - startTime) < 10000));

	  	if (!Serial) {
	  		return 0;
	  	} else {
	  		return _baudRate;
	  	}
	}
}

bool ELMo::connected() {
	bool conn = client.connected();
	if (!conn) {
		_initialized = false;
		if (_debug) Serial.println("ELMo: CONNECTION LOST, ELM UNINITIALIZED");
	}
	return conn;
}

bool ELMo::stop() {
	send("AT WS");
	client.stop();
	return true;
}

int ELMo::getTimeout() {
	return _responseTimeout;
}

int ELMo::setTimeout(int seconds) {
	_responseTimeout = seconds;
	return _responseTimeout;
}