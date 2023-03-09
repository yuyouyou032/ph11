//Include the library
#include <FS.h>
#include "WiFi.h"
#include <WiFi.h>
#include "SPIFFS.h"
#include <SPIFFS.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include <MQUnifiedsensor.h>

// ESP32 Hostname
String hostname = "ESP32.MO135";

// Network Credentials
const char* ssid = "wifi-ssid";
const char* pass = "wifi-psk";

//Led OnBoard
#define ONBOARD_LED  2

//Definitions
#define placa "ESP-32"

// Voltage 3.3v
#define Voltage_Resolution 3.3

// Analog input Pin of your board
#define pin 34

//MQ135
#define type "MQ-135"

// For Arduino UNO/MEGA/NANO
#define ADC_Bit_Resolution 10

//RS / R0 = 3.6 ppm
#define RatioMQ135CleanAir 3.6

//Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Led Blink Function
void LED_Blink(int delayTime, int Replay){
  for (int i = 0; i <= Replay; i++) {
    delay(delayTime); digitalWrite(ONBOARD_LED,HIGH);
    delay(delayTime); digitalWrite(ONBOARD_LED,LOW);
  }
}

//Sensors Reading
String readMQ135CO() {
  // Sensor readings may also be up to 1 seconds
  // Read PPM CO concentration
  MQ135.setA(605.18); MQ135.setB(-3.937);
  float CO = MQ135.readSensor();
  // Check if any reads failed and exit early (to try again).
  if (isnan(CO)) {    
    Serial.println("CO PPP : Failed From Sensor!");
    return "--";
  }
  else {
    Serial.print("CO: ");
    Serial.println(CO);
    LED_Blink(100,0);
    return String(CO);
  }
}

String readMQ135CO2() {
  // Sensor readings may also be up to 1 seconds
  // Read PPM CO2 concentration
  MQ135.setA(110.47); MQ135.setB(-2.862);
  float CO2 = MQ135.readSensor();
  // Check if any reads failed and exit early (to try again).
  if (isnan(CO2)) {    
    Serial.println("CO2 PPP : Failed From Sensor!");
    return "--";
  }
  else {
    Serial.print("CO2: ");
    Serial.println(CO2);
    LED_Blink(100,0);
    return String(CO2);
  }
}

String readMQ135NH4() {
  // Sensor readings may also be up to 1 seconds
  // Read PPM NH4 concentration
  MQ135.setA(102.2); MQ135.setB(-2.473);
  float NH4 = MQ135.readSensor();
  // Check if any reads failed and exit early (to try again).
  if (isnan(NH4)) {    
    Serial.println("NH4 PPP : Failed From Sensor!");
    return "--";
  }
  else {
    Serial.print("NH4: ");
    Serial.println(NH4);
    LED_Blink(100,0);
    return String(NH4);
  }
}

String readMQ135ALC() {
  // Sensor readings may also be up to 1 seconds
  // Read PPM Alcohol concentration
  MQ135.setA(77.255); MQ135.setB(-3.18);
  float ALC = MQ135.readSensor();
  // Check if any reads failed and exit early (to try again).
  if (isnan(ALC)) {    
    Serial.println("Alcohol PPP : Failed From Sensor!");
    return "--";
  }
  else {
    Serial.print("Alcohol: ");
    Serial.println(ALC);
    LED_Blink(100,0);
    return String(ALC);
  }
}

String readMQ135TOL() {
  // Sensor readings may also be up to 1 seconds
  // Read PPM Toluene concentration
  MQ135.setA(44.947); MQ135.setB(-3.445);
  float TOL = MQ135.readSensor();
  // Check if any reads failed and exit early (to try again).
  if (isnan(TOL)) {    
    Serial.println("Toluene PPP : Failed From Sensor!");
    return "--";
  }
  else {
    Serial.print("Toluene: ");
    Serial.println(TOL);
    LED_Blink(100,0);
    return String(TOL);
  }
}

String readMQ135ACT() {
  // Sensor readings may also be up to 1 seconds
  // Read PPM Acetone concentration
  MQ135.setA(34.668); MQ135.setB(-3.369);
  float ACT = MQ135.readSensor();
  // Check if any reads failed and exit early (to try again).
  if (isnan(ACT)) {    
    Serial.println("Acetone PPP : Failed From Sensor!");
    return "--";
  }
  else {
    Serial.print("Acetone: ");
    Serial.println(ACT);
    LED_Blink(100,0);
    return String(ACT);
  }
}

// Replaces placeholder with MQ135 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "CO"){
    return readMQ135CO();
  }
  else if(var == "CO2"){
    return readMQ135CO2();
  }
  else if(var == "NH4"){
    return readMQ135NH4();
  }
  else if(var == "ACT"){
    return readMQ135ACT();
  }
  else if(var == "TOL"){
    return readMQ135TOL();
  }
  else if(var == "ALC"){
    return readMQ135ALC();
  }
  return String();
}

// Setup
void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Init Led
  pinMode(ONBOARD_LED,OUTPUT);

  // Booting
  Serial.println("Starting Device ...");
  delay(1000);

  // Led On
  digitalWrite(ONBOARD_LED, HIGH);

  // Define hostname
  WiFi.setHostname(hostname.c_str());

  // Print hostname
  Serial.print("Hostname: ");
  Serial.println(hostname);

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi ..");
  for (int i = 0; i <= 5; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
  }

  // Space Debug
  Serial.println();

  // Check Wi-Fi Connection
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.print("Connected To WiFi AP");
  }
  else {
    Serial.println("Connection To WiFi AP Failed");
    Serial.println("Restarting Device ...");
    LED_Blink(500,5);
    ESP.restart();
  }

  // Space Debug
  Serial.println();

  // Print ESP32 SSID Mac Address
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print ESP32 RRSI Mac Address
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  // Print ESP32 IP Address
  Serial.print("ADIP: ");
  Serial.println(WiFi.localIP());
  
  // Print ESP32 Mac Address
  Serial.print("MACS: ");
  Serial.println(WiFi.macAddress());

  // SPIFFS Initialization
  if(!SPIFFS.begin()){
    Serial.println("Error Mounting Up SPIFFS Volume !");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("SPIFFS Volume Mounted");

  // Set up mDNS responder:
  if (!MDNS.begin(hostname.c_str())) {
    Serial.println("Error Setting Up MDNS Responder !");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS Responder Started");

  //Set math model to calculate the PPM concentration and the value of constants
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b

  //Init Sensor
  MQ135.init();
 
  // In this routine the sensor will measure the resistance of the sensor supposing before was pre-heated
  // and now is on clean air (Calibration conditions), and it will setup R0 value.
  // We recomend execute this routine only on setup or on the laboratory and save on the eeprom of your arduino
  // This routine not need to execute to every restart, you can load your R0 if you know the value
  // Verbose PreHeat
  Serial.println("Calibrating ...");

  //PreHeat
  Serial.println("PreHeating ...");

  // LED Code
  digitalWrite(ONBOARD_LED,HIGH);

  //MQ CAlibration
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++){
    // Update data, the arduino will be read the voltage on the analog pin
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    MQ135.setR0(calcR0/10);
    // End Calibration
  }
  
  // Verobose Done
  Serial.println("Calibration Done.");

  // Wires Detection
  if(isinf(calcR0)) {
    Serial.println("Warning: Conection Issue Founded, R0 Infite : Open Circuit Detected");
    Serial.println("Restarting Device ...");
    delay(3000); LED_Blink(100,1);
    ESP.restart(); // Reboot Esp32
    }
  if(calcR0 == 0){
    Serial.println("Warning: Conection Issue Founded, R0 Zero : Analog Pin Short Circuit Ground");
    Serial.println("Restarting Device ...");
    delay(3000); LED_Blink(100,1);
    ESP.restart(); // Reboot Esp32
    }

  // Debug Mode
  //MQ135.serialDebug(false);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.css", "text/css");
  });
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.js", "text/js");
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico", "image/png");
  });
  server.on("/co", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMQ135CO().c_str());
  });
  server.on("/co2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMQ135CO2().c_str());
  });
  server.on("/nh4", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMQ135NH4().c_str());
  });
  server.on("/tol", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMQ135TOL().c_str());
  });
  server.on("/alc", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMQ135ALC().c_str());
  });
  server.on("/act", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMQ135ACT().c_str());
  });
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain","Rebooting ...");
    ESP.restart(); 
  });

  // Init Server
  Serial.println("Web Server Started.");
  
  // Start server
  server.begin();

  // Led Off
  digitalWrite(ONBOARD_LED,LOW);
}

// Loop
void loop()
{
  // if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED)) {
    Serial.println("Disconnected From WiFi AP"); 
    Serial.println("Restarting Device ...");
    delay(5000); LED_Blink(500,5);
    ESP.restart(); // Reboot Esp32
  }  

  // Update data, the esp will be read the voltage on the analog pin
  MQ135.update();
}
