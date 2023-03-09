/*
    Name:       startstop.ino
    Created:	2019-04-03 7:06:46
    Author:     Dirk O. Kaar <dok@dok-net.net>
*/

#include <SoftwareSerial.h>
#include <Sds011.h>

#define SDS_PIN_RX D7
#define SDS_PIN_TX D8

#ifdef ESP32
HardwareSerial& serialSDS(Serial2);
Sds011Async< HardwareSerial > sds011(serialSDS);
#else
SoftwareSerial serialSDS;
Sds011Async< SoftwareSerial > sds011(serialSDS);
#endif

bool is_SDS_running = true;

void start_SDS() {
    Serial.println("Start wakeup SDS011");

    if (sds011.set_sleep(false)) { is_SDS_running = true; }

    Serial.println("End wakeup SDS011");
}

void stop_SDS() {
    Serial.println("Start sleep SDS011");

    if (sds011.set_sleep(true)) { is_SDS_running = false; }

    Serial.println("End sleep SDS011");
}

// The setup() function runs once each time the micro-controller starts
void setup()
{
    Serial.begin(115200);

#ifdef ESP32
    serialSDS.begin(9600, SERIAL_8N1, SDS_PIN_RX, SDS_PIN_TX);
    delay(100);
#else
    serialSDS.begin(9600, SWSERIAL_8N1, SDS_PIN_RX, SDS_PIN_TX, false, 192);
#endif

    Serial.println("SDS011 start/stop and reporting sample");

    start_SDS();
    Serial.print("SDS011 is running = ");
    Serial.println(is_SDS_running);

    String firmware_version;
    uint16_t device_id;
    if (!sds011.device_info(firmware_version, device_id)) {
        Serial.println("Sds011::firmware_version() failed");
    }
    else
    {
        Serial.print("Sds011 firmware version: ");
        Serial.println(firmware_version);
        Serial.print("Sds011 device id: ");
        Serial.println(device_id, 16);
    }

    Sds011::Report_mode report_mode;
    if (!sds011.get_data_reporting_mode(report_mode)) {
        Serial.println("Sds011::get_data_reporting_mode() failed");
    }
    if (Sds011::REPORT_ACTIVE != report_mode) {
        Serial.println("Turning on Sds011::REPORT_ACTIVE reporting mode");
        if (!sds011.set_data_reporting_mode(Sds011::REPORT_ACTIVE)) {
            Serial.println("Sds011::set_data_reporting_mode(Sds011::REPORT_ACTIVE) failed");
        }
    }
}

// Add the main program code into the continuous loop() function
void loop()
{
    stop_SDS();
    Serial.print("SDS011 is stopped = ");
    Serial.println(!is_SDS_running);
    delay(10000);
    start_SDS();
    Serial.print("SDS011 is running = ");
    Serial.println(is_SDS_running);
    delay(10000);
}
