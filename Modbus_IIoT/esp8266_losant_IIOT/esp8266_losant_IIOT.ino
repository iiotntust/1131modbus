/**
 * Example that connects an ESP8266 based board to the Losant
 * IoT platform. This example reports state to Losant whenever a button is
 * pressed. It also listens for the "toggle" command to turn the LED on and off.
 *
 * This example assumes the following connections:
 * Button connected to pin 14.
 * LED connected to pin 12.
 *
 * Copyright (c) 2020 Losant. All rights reserved.
 * http://losant.com
 */




#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <Losant.h>



/*
  ModbusRTU ESP8266/ESP32
  Read multiple coils from slave device example

  (c)2019 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266

  modified 13 May 2020
  by brainelectronics

  This code is licensed under the BSD New License. See LICENSE.txt for more info.
*/



#include <ModbusRTU.h>
#if defined(ESP8266)
 #include <SoftwareSerial.h>
 // SoftwareSerial S(D1, D2, false, 256);

 // receivePin, transmitPin, inverse_logic, bufSize, isrBufSize
 // connect RX to D2 (GPIO4, Arduino pin 4), TX to D1 (GPIO5, Arduino pin 4)
 SoftwareSerial S(D4, D3);
#endif

ModbusRTU mb;




// WiFi credentials.

const char* WIFI_SSID = "Factory2_2.4G";
const char* WIFI_PASS = "118factory2";

// Losant credentials.
const char* LOSANT_DEVICE_ID = "651cd01df3525308b76039e7";
const char* LOSANT_ACCESS_KEY = "2169ec8e-e08b-4e08-8d58-d7f24a9a68e3";
const char* LOSANT_ACCESS_SECRET = "64dac31d03ac2cd8d24d2235a3d33f58b31e4cdefb24e48425cde97640bd70e5";

const int BUTTON_PIN = 14;
const int LED_PIN = 12;

bool ledState = false;
bool coils[20];
uint16_t Iregs[7];
uint16_t Hregs[7];
int buttonState = 0;






// Cert taken from 
// https://github.com/Losant/losant-mqtt-ruby/blob/master/lib/losant_mqtt/RootCA.crt
static const char digicert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)EOF";

BearSSL::WiFiClientSecure wifiClient;

LosantDevice device(LOSANT_DEVICE_ID);
bool ModbusIsGood = false;

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
#ifdef ESP8266
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  //if(event=0){
  //    ModbusIsGood = true;
  //    else{
  //    ModbusIsGood = false;
 //       }
 // }
  
#elif ESP32
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
    ModbusIsGood = true;
#else
  Serial.print("Request result: 0x");
  Serial.print(event, HEX);

  
#endif
  return true;
}





// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    now = time(nullptr);
  }
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
}

void toggle() {
  Serial.println("Toggling LED.");
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

// Called whenever the device receives a command from the Losant platform.
void handleCommand(LosantCommand *command) {
  Serial.print("Command received: ");
  Serial.println(command->name);
  // Optional command payload. May not be present on all commands.
  //JsonObject payload = *command->payload;
  
  // Perform action specific to the command received.
  if(strcmp(command->name, "toggle") == 0) {
    toggle();
    /**
    * In Losant, including a payload along with your
    * command is optional. This is an example of how
    * to parse a JSON payload from Losant and print
    * the value of a key called "temperature".
    */
    // int temperature = payload["temperature"];
    // Serial.println(temperature);
  }
}




void connect() {

  // Connect to Wifi.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Validating  the SSL for for a secure connection you must
  // set trust anchor, as well as set the time.
  BearSSL::X509List cert(digicert);
  wifiClient.setTrustAnchors(&cert);
  setClock();
  

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to Losant.
  Serial.println();
  Serial.print("Connecting to Losant...");

  device.connectSecure(wifiClient, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);

  while(!device.connected()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { }
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Register the command handler to be called when a command is received
  // from the Losant platform.
  device.onCommand(&handleCommand);

  connect();

 #if defined(ESP8266)
  S.begin(9600, SWSERIAL_8N1);
  mb.begin(&S,D7,true);
 #elif defined(ESP32)
  Serial1.begin(9600, SERIAL_8N1);
  mb.begin(&Serial1);
 #else
  Serial1.begin(9600, SERIAL_8N1);
  mb.begin(&Serial1);
  mb.setBaudrate(9600);
 #endif
  mb.master();  
}

float scale_value = 100;

void send_Modbus_reading(){
  float convertedTemp = 0;
  // Losant uses a JSON protocol. Construct the simple state object.
  // { "button" : true }

  convertedTemp = scale_value*(Iregs[0]*pow(2,-16)-0.5)+Iregs[1]*(pow(2,-16))-0.5;
  //convertedTemp = pow(2,-16)*scale_value+(pow(2,-16));

  
  //Serial.println("Converted Temp: ");
  Serial.printf_P("converted Tep:: %05f\n",convertedTemp);

  StaticJsonDocument<200> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["ModbusSlave_1_Temp"] = convertedTemp;
  // Send the state to Losant.
  device.sendState(root);
}




void send_ModbusStatus() {
  // Losant uses a JSON protocol. Construct the simple state object.
  // { "button" : true }
  StaticJsonDocument<200> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["ModbusIsGood"] = ModbusIsGood;
  // Send the state to Losant.
  device.sendState(root);
}



uint32_t interval =  1000;
uint64_t previousMillis = 0 ;
void loop() {
  uint64_t currentMillis = millis();
  
  
  

  bool toReconnect = false;

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from WiFi");
    toReconnect = true;
  }

  if(!device.connected()) {
    Serial.println("Disconnected from Losant");
    toReconnect = true;
  }

  if(toReconnect) {
    connect();
  }

  device.loop();

  int currentRead = digitalRead(BUTTON_PIN);

   if (!mb.slave()) {
    //mb.readCoil(1, 1, coils, 20, cbWrite);
    mb.readIreg( 1, 30001, Iregs, 3, cbWrite);
    Serial.printf_P("Modbus Iregs result: 0x%02X\n",Iregs[1]);
  }

  if (currentMillis - previousMillis >= interval) {
    // save the last time the function ran
  previousMillis = currentMillis;
  send_Modbus_reading();
  send_ModbusStatus();
  }

  mb.task();
  yield();

  delay(100);//<-----possible issue
}
