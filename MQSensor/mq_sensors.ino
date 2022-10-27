// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A1);
  
  // print out the value you read:
  Serial.println(sensorValue);
  delay(20);
}


//float RS_gas = 0;
//float ratio = 0;
//float sensorValue = 0;
//float sensor_volt = 0;
//float R0 = 7200.0;
// 
//void setup() {
// Serial.begin(9600);
//}
// 
//void loop() {
//   sensorValue = analogRead(A1);
//   
//   sensor_volt = sensorValue/1024*5.0;
//   RS_gas = (5.0-sensor_volt)/sensor_volt;
//   ratio = RS_gas/R0; //Replace R0 with the value found using the sketch above
//   float x = 1538.46 * ratio;
//   float ppm = pow(x,-1.709);
//   Serial.print("PPM: ");
//   Serial.println(ppm);
//   delay(1000);
//}
