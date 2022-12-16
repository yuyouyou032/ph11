// Adafruit interface
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <SDS011.h>
#include "Adafruit_MQTT.h"
#include "AdafruitIO_Feed.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "._."
#define WLAN_PASS       "qwertyuiop1234567890/"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "ph11"
#define AIO_KEY         "aio_LTyK211T4xYR7nq50Xz509HTI4r5"

#define MQ135  35
#define MQ7    34

SDS011 my_sds;
#ifdef ESP32
HardwareSerial port(2);
#endif

// setup WiFi credentials to enable I/O
#include "AdafruitIO_WiFi.h"
#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) || defined(ADAFRUIT_PYPORTAL)
// Configure the pins used for the ESP32 connection
#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
// Don't change the names of these #define's! they match the variant ones
#define SPIWIFI SPI
#define SPIWIFI_SS 10 // Chip select pin
#define NINA_ACK 9    // a.k.a BUSY or READY pin
#define NINA_RESETN 6 // Reset pin
#define NINA_GPIO0 -1 // Not connected
#endif
AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WLAN_SSID, WLAN_PASS, SPIWIFI_SS, NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
#else
AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WLAN_SSID, WLAN_PASS);
#endif


WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish gases = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mq135");


// Input to live feed on Adafruit
AdafruitIO_Feed *MQ135digital = io.feed("mq-air-quality-1.mq135-1");
AdafruitIO_Feed *MQ7digital = io.feed("mq-air-quality-1.mq7-1");
AdafruitIO_Feed *PM25digital = io.feed("mq-air-quality-1.pm25-1"); // pm25 = pm2.5
AdafruitIO_Feed *PM10digital = io.feed("mq-air-quality-1.pm10-1");

//AdafruitIO_Feed *wifiOn = io.feed("wifiActive");
//wifiOn->save(0);

void setup() {
  my_sds.begin(&port);
  Serial.begin(115200);
  pinMode(MQ135, INPUT);
  pinMode(MQ7, INPUT);
  Serial.println("sensors initialised");
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());


  // Feed I/O set-up
  while(! Serial); // wait for serial monitor to open

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // connected to live feed
  Serial.println();
  Serial.println(io.statusText());
}


// for the function in loop - checks if server is active or not
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
//    wifiOn->save(1);
    return;
  }

  Serial.print("Connecting to MQTT… ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds…");
    mqtt.disconnect();

//    wifiOn->save(0);

    delay(5000); // wait 5 seconds

    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}


void loop() {
  io.run();
//  wifiOn->save(0);
  MQTT_connect();

  // all code (processing) after this point
  // dummy variables to aggregate
  int a = 0, b = 0;
  float c = 0, d = 0;
  int err = 0;
  float p10, p25;
  float RS_gas = 0;
  float ratio = 0;
  float sensorValue = 0;
  float sensor_volt = 0;
  float R0 = 720;
  
  for (int i=0; i<1000; i++) {
    int MQ135reading = analogRead(MQ135);
    int MQ7reading = analogRead(MQ7);
    a = a + MQ135reading;
    b = b + MQ7reading;
  }

  
  int mqR = 22000; //pull-down resistor value
  long rO = 41763; //rO sensor value
  float minRsRo = 0.358; //min value for Rs/Ro
  float maxRsRo = 2.428; //max value for Rs/Ro
  float a7 = 116.6020682; //sensor a coefficient value
  float b7 = -2.769034857; //sensor b coefficient value

  // tidying up the values with exponential regression function
  int averageMQ135 = a/1000;
  int averageMQ7 = b/1000;

  
  int adcRaw = averageMQ7;
  long rS = ((1024.0 * mqR) / adcRaw) - mqR;
//  Serial.print("Rs: ");
//  Serial.println(rS);
  float rSrO = (float)rS / (float)rO;
//  Serial.print("Rs/Ro: ");
//  Serial.println(rSrO);

  float ppm = a7 * pow((float)rS / (float)rO, b7);


  err = my_sds.read(&p25, &p10);
  c = p25;
  d = p10;

  float averagePM25 = c;
  float averagePM10 = d;

  // output the results
  Serial.print("MQ135 reading (hazardous gases): ");
  Serial.println(averageMQ135);
  MQ135digital->save(averageMQ135);
  
  Serial.print("MQ7 reading (CO): ");
  Serial.println(ppm);
  MQ7digital->save(ppm);

  Serial.print("PM2.5 reading: ");
  Serial.println(String(averagePM25));
  PM25digital->save(averagePM25);

  Serial.print("PM10 reading: ");
  Serial.println(String(p10));
  PM10digital->save(averagePM10);


  delay(10000);
}
  
