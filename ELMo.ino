#include <ESP8266WiFi.h>

const char* ssid = "WiFi-OBDII";

WiFiClient client;

//IP Adress of the ELM327 Dongle
IPAddress server(192, 168, 0, 10);

int responseTimeout = 60; // seconds
int dsLength = 180; //seconds
bool clear = false;
bool debug = false;


void setup() {
  // start serial (for debugging)
  Serial.begin(115200);
  while (!Serial);
   
  // connect to wifi
  if (debug) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) delay(500);

  if (debug) {
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // connect to ELM
  if (client.connect(server, 35000)) {
    if (debug) Serial.println("Connected to ELM327");
    if (!initializeELM()) {
      if (debug) Serial.println("Coudln't Initialize ELM327, entering Deep Sleep");
      // in case of failed initialization, deep sleep; car may be off
      ESP.deepSleep(dsLength*1000000); 
    }
    if (debug) Serial.println("Initialized ELM327");
  }
  else {
    if (debug) Serial.println("Connection to ELM failed");
    delay(60000);
    reboot();
  }
}

void loop() {
  if (!client.connected()) {
    if (debug) Serial.println("CLIENT DISCONNECTED - RESET!");
    reboot();
  }

  clear = false;
  String status = sendAndWait("0101");
  if (status.substring(6,7).equals("8")) {
    //CEL is on
    if (0 < status.substring(7,8).toInt() < 3) {
      // there are two or less codes
      clear = true;
      String rawCodes = sendAndWait("03");
      String code1 = rawCodes.substring(3, 8);
      String code2 = rawCodes.substring(25, 30);
      if (!(code1.equals("00 00") || code1.equals("07 41") || code1.equals("15 25"))) clear = false;
      if (!(code2.equals("00 00") || code2.equals("07 41") || code2.equals("15 25"))) clear = false;
    }
  }

  if (clear) {
    sendAndWait("04");
    if (debug) Serial.println("CODES MUST BE CLEARED");
  }

  // sleep for 1 minute
  // TODO: use TaskScheduler
  if (debug) Serial.println("Sleeping...");
  delay(60000);
}

bool initializeELM() {
  bool success = true;
  
  sendAndWait("AT Z");
  sendAndWait("AT E0"); // disables echo
  sendAndWait("AT H0"); // disables headers
  if (!sendAndWait("AT SP 0").substring(0, 2).equals("OK")) success = false;
  if (sendAndWait("0100").indexOf("UNABLE TO CONNECT") > -1) success = false; // queries supported PIDs and prepares ELM

  return success;
}

String sendAndWait(String message) {
  client.println(message);
  
  String received = "";
  unsigned long beginTime = millis();
  while (millis() - beginTime < (responseTimeout*1000)) {
    if (client.available()) {
      char c = client.read();
      if (c == '>') {
        // done receiving
        if (debug) {
          Serial.print("Received: ");
          Serial.println(received);
        }
        // check that response is adequate
        if (message.indexOf("AT") == -1) {
          // message is not AT
          if (received.indexOf("NO DATA") > -1) reboot();
        }
        return received;
      }
      received += c;
    }
  }
  
  // timed out
  reboot();
}

void reboot() {
  if (debug) Serial.println("Rebooting...");
  client.stop();
  ESP.reset();
}
