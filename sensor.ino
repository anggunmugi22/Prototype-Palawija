#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//definisi pin tcs warna
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

SoftwareSerial nodemcu(2, 3); //2=Rx, 3=Tx

//variable lm35 suhu
int val;
int tempPin = 1;

//variable lm393 moisture
int analogInPin = A0;
int sensorValue = 0;
int outputValue = 0;

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

void setup()
{
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(9600);

  nodemcu.begin(9600);
  delay(1000);

  Serial.println("Program started");
}
void loop()
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  //sensor lm35 suhu
  val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000;
  float cel = mv/10;
  Serial.print("TEMPRATURE = ");
  Serial.print(cel);
  Serial.print("*C");
  Serial.println();
  delay(1000);


  delay(2000);
  //sensor lm393 Moisture
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 100);
  Serial.print("Soil Moisture Level = ");
  Serial.print(outputValue);
  Serial.println(" %");
  delay(1000);

  //sensor tcs warna
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
 
  redFrequency = pulseIn(sensorOut, LOW);

  Serial.print("R= ");
  Serial.print(redFrequency);
  Serial.print("  ");
  delay(100);
  
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  greenFrequency = pulseIn(sensorOut, LOW);
 
  Serial.print("G= ");
  Serial.print(greenFrequency);
  Serial.print("  ");
  delay(100);
  
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
 
  blueFrequency = pulseIn(sensorOut, LOW);
  
  Serial.print("B= ");
  Serial.print(blueFrequency);
  Serial.println("  ");
  delay(100);

  data["temperature"] = cel;
  data["moisture"] = outputValue;
  data["red"] = redFrequency;
  data["green"] = greenFrequency;
  data["blue"] = blueFrequency;

  data.printTo(nodemcu);
  jsonBuffer.clear();


}
