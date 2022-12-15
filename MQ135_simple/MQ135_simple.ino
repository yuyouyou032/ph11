//Include the library
#include <WiFi.h>
#include <MQUnifiedsensor.h>

// ESP32 Hostname
String hostname = "ESP32.MQ135";

// Network Credentials
const char* ssid = "yuyouyou";
const char* pass = "02031344";

//Definitions
#define placa "ESP-32"

// Voltage 5v
#define Voltage_Resolution 5

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

void setup()
{
  Serial.begin(115200);
  if(Serial) Serial.println("Serial is open");

  WiFi.begin(ssid, pass);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  //Set math model to calculate the PPM concentration and the value of constants
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b

  //Init Sensor
  MQ135.init();
 
  // In this routine the sensor will measure the resistance of the sensor supposing before was pre-heated
  // and now is on clean air (Calibration conditions), and it will setup R0 value.
  // We recomend execute this routine only on setup or on the laboratory and save on the eeprom of your arduino
  // This routine not need to execute to every restart, you can load your R0 if you know the value
  // Verbose PreHeat
  Serial.print("Calibrating ...");

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
  Serial.print("Calibration Done.");
  
  if(isinf(calcR0)) {
    Serial.println("Warning: Conection Issue Founded, R0 is infite : Open Circuit Detected"); while(1);
    }
  if(calcR0 == 0){
    Serial.println("Warning: Conection Issue Founded, R0 is zero : Analog Pin Short Circuit Ground"); while(1);
    }

  // Debug Mode
  //MQ135.serialDebug(false);
}

void loop()
{
  // Update data, the esp will be read the voltage on the analog pin
  MQ135.update();

  // Configurate PPM ecuation CO concentration
  MQ135.setA(605.18); MQ135.setB(-3.937);
  float CO = MQ135.readSensor();
  
  // Configurate PPM ecuation CO2 concentration
  MQ135.setA(110.47); MQ135.setB(-2.862);
  float CO2 = MQ135.readSensor();
  
  // Configurate PPM ecuation Alcohol concentration
  MQ135.setA(77.255); MQ135.setB(-3.18);
  float Alcohol = MQ135.readSensor();
  
  // Configurate PPM ecuation Toluene concentration
  MQ135.setA(44.947); MQ135.setB(-3.445);
  float Toluene = MQ135.readSensor();
  
  // Configurate PPM ecuation NH4 concentration
  MQ135.setA(102.2); MQ135.setB(-2.473);
  float NH4 = MQ135.readSensor();

  // Configurate PPM ecuation Acetone concentration
  MQ135.setA(34.668); MQ135.setB(-3.369);
  float Acetone = MQ135.readSensor();

  Serial.print("CO: ");
  Serial.println(CO);
  
  Serial.print("CO2: ");
  Serial.println(CO2);
  
  Serial.print("NH4: ");
  Serial.println(NH4);
  
  Serial.print("Alcohol: ");
  Serial.println(Alcohol);
  
  Serial.print("Toluene: ");
  Serial.println(Toluene);
  
  Serial.print("Acetone: ");
  Serial.println(Acetone);

  //Sampling frequency 1000
  delay(1000); 
}
