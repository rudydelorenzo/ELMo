#include <ESP8266WiFi.h>

const char* ssid = "WiFi-OBDII";

//IP Adress of the ELM327 Dongle
IPAddress server(192, 168, 0, 10);

WiFiClient client;

int run = 0;

void setup() {
  // start serial (for debugging)
  Serial.begin(115200);
  while (!Serial);
   
  // connect to wifi
  Serial.println("Connecting to WiFi: WIFI-obd");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // connect to ELM
  if (client.connect(server, 35000)) {
    Serial.println("Connected to ELM327");
    initializeELM();
    Serial.println("Initialized ELM327");
  }
  else {
    Serial.println("Connection to ELM failed");
    ESP.reset();
  }
}

void loop() {
  if (!client.connected()) {
    Serial.println("CLIENT DISCONNECTED - RESET!");
    client.stop();
    ESP.reset();
  }
  
  //bool CELon = 
  String rawCodes = sendAndWait("03");
  Serial.println("Raw Codes:");
  Serial.println(rawCodes);

  //TODO: sleep for 1 minute
  delay(60000);
}

bool initializeELM() {
  sendAndWait("AT Z");
  sendAndWait("AT SP 0");
  sendAndWait("0100"); // queries supported PIDs and prepares ELM
}

String sendAndWait(String message) {
  client.println(message);
  
  String received = "";
  while (true) {
    if (client.available()) {
      char c = client.read();
      if (c == '>') {
        return received;
      }
      received += c;
    }
  }
}
