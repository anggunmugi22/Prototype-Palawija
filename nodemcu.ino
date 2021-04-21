//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//NodeMCU code

//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "kualitas-tanah-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "OmTWJqLteyDqnaYM3Xmk2zXjrCqh6i9fy64QTmRM"

const char* ssid="minimouse";
const char* password = "yuhuy123";


//D3 = Rx & D2 = Tx
SoftwareSerial nodemcu(D3, D2);

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);
  while (!Serial) continue;

  //Konek ke WiFi
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");
  }
  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

  //Ke Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }

  Serial.println("JSON Object Recieved");
  Serial.print("Recieved Temperature:  ");
  float cel = data["temperature"];
  Serial.println(cel);
  Serial.print("Recieved Moisture:  ");
  int outputValue = data["moisture"];
  Serial.println(outputValue);
  Serial.print("Recieved Red Value:  ");
  int redFrequency = data["red"];
  Serial.println(redFrequency);
  Serial.print("Recieved Green Value:  ");
  int greenFrequency = data["green"];
  Serial.println(greenFrequency);
  Serial.print("Recieved Blue Value:  ");
  int blueFrequency = data["blue"];
  Serial.println(blueFrequency);
  Serial.println("-----------------------------------------");

      data["temperature"] = cel;
      data["moisture"] = outputValue;
      data["red"] = redFrequency;
      data["green"] = greenFrequency;
      data["blue"] = blueFrequency;
 
      // append a new value to /logDHT
      String name = Firebase.push("/sensor/tanah", data);


  
//  Firebase.setInt("temperature", cel);
//  if (Firebase.failed()) {
//    Serial.print("pushing /logs failed:");
//    Serial.println(Firebase.error());
//    return;
//  }
//
//  Firebase.setInt("moisture", outputValue);
//  if (Firebase.failed()) {
//    Serial.print("pushing /logs failed:");
//    Serial.println(Firebase.error());
//    return;
//  }
//  Firebase.setInt("red", redFrequency);
//  if (Firebase.failed()) {
//    Serial.print("pushing /logs failed:");
//    Serial.println(Firebase.error());
//    return;
//  }
//  Firebase.setInt("green", greenFrequency);
//  if (Firebase.failed()) {
//    Serial.print("pushing /logs failed:");
//    Serial.println(Firebase.error());
//    return;
//  }
//  Firebase.setInt("blue", blueFrequency);
//  if (Firebase.failed()) {
//    Serial.print("pushing /logs failed:");
//    Serial.println(Firebase.error());
//    return;
//  }

}
