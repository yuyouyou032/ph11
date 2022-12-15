// Adafruit interface
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "AdafruitIO_Feed.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "._."
#define WLAN_PASS       "qwertyuiop1234567890/"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "ph11"
#define AIO_KEY         "aio_OFIF11KCcKksOmVHGG2otQykMez4"

#define MQ135  33
#define MQ7    32


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
AdafruitIO_Feed *MQ135digital = io.feed("mq135");
AdafruitIO_Feed *MQ7digital = io.feed("mq7");

void setup() {
  Serial.begin(115200);
  pinMode(MQ135, INPUT);
  pinMode(MQ7, INPUT);
  
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
    return;
  }

  Serial.print("Connecting to MQTT… ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds…");
    mqtt.disconnect();

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
  MQTT_connect();

  // all code (processing) after this point
  // dummy variables to aggregate
  int a = 0, b = 0; 
  
  for (int i=0; i<1000; i++) {
    int MQ135reading = analogRead(MQ135);
    int MQ7reading = analogRead(MQ7);
    a = a + MQ135reading;
    b = b + MQ7reading;
  }

  // output the results
  int averageMQ135 = a/1000;
  int averageMQ7 = b/1000;
  Serial.print("MQ135 reading (hazardous gases): ");
  Serial.println(averageMQ135);
  MQ135digital->save(averageMQ135);
  
  Serial.print("MQ7 reading (CO): ");
  Serial.println(averageMQ7);
  MQ7digital->save(averageMQ7);

  delay(5000);
}
  
