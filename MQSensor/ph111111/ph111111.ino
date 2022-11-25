// mq-135 (nitrogen oxides, alcohol, smoke)

// the setup routine runs once when you press reset

//mq-7 (carbon monoxide, CO)

float RS_gas = 0;
float ratio = 0;
float sensorValue = 0;
float sensor_volt = 0;
float R0 = 7200.0;
 

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0, for MQ135:
  int mq135Value = analogRead(A0);

  // read the input for MQ7:
  int mq7Value = analogRead(A1); 
  sensor_volt = sensorValue/1024*5.0;
  RS_gas = (5.0-sensor_volt)/sensor_volt;
  ratio = RS_gas/R0; //Replace R0 with the value found using the sketch above
  float x = 1538.46 * ratio;
  float ppm = pow(x,-1.709);

  // output (mq135, mq7)
  Serial.print(mq135Value);
  Serial.print(",");
  Serial.println(mq7Value);
  delay(500);
}

/*
void loop() {
   sensorValue = analogRead(A1);
   
   sensor_volt = sensorValue/1024*5.0;
   RS_gas = (5.0-sensor_volt)/sensor_volt;
   ratio = RS_gas/R0; //Replace R0 with the value found using the sketch above
   float x = 1538.46 * ratio;
   float ppm = pow(x,-1.709);
   Serial.print("PPM: ");
   Serial.println(ppm);
   delay(1000);
}
*/

// sds-011
//#include "SDS011.h"
//
//float p10, p25;
//int error;
//SDS011 my_sds;
//
//void setup() {
//  my_sds.begin(2, 3);
//  Serial.begin(9600);
//      }
//
//void loop() {
//  Serial.println(millis());
//  error = my_sds.read(&p25, &p10);
//  if (!error) {
//    Serial.println("P2.5: " + String(p25));
//    Serial.println("P10:  " + String(p10));
//      
//  }
//  delay(1000);
//  }
