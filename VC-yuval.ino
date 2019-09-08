/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

/*
 * HTTP Client POST Request
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>



int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 2;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int red = D8;
int blue = D7;
int green = D6;

void sensor() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Serial.print("-------");
  //Serial.print(sensorValue);
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  // stop the program for for <sensorValue> milliseconds:
  delay(sensorValue);
}


void post_request(){
  HTTPClient http;    //Declare object of class HTTPClient

  String ADCData, station, postData;
  //int adcvalue=analogRead(A0);  //Read Analog value of LDR
  //ADCData = String(adcvalue);   //String to interger conversion
  station = "A";

  //Post Data
  postData = sensorValue ;
  
  http.begin("http://104.45.25.73:80/arduino");              //Specify request destination
  http.addHeader("Content-Type", "application/json");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
  Serial.print("-------");
  Serial.print(httpCode);   //Print HTTP return code
  Serial.print("-------");
  Serial.print(payload);    //Print request response payload
  http.begin("http://192.168.43.52:80/arduino");
  http.addHeader("Content-Type", "application/json");
  int httpCode2 = http.POST(postData);
  http.end();  //Close connection


}
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "UV-OP6"
#define STAPSK  "0543055564"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "104.45.25.73";
const uint16_t port = 80;
const char* local_host = "192.168.43.52";


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  digitalWrite(red,LOW);
  digitalWrite(blue,LOW);
  digitalWrite(green,LOW);
  
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  }

  Serial.print("");
  Serial.print("-------");
  Serial.print("WiFi connected");
  Serial.print("-------");
  Serial.print("IP address: ");
  Serial.print("-------");
  Serial.print(WiFi.localIP());
  Serial.println("");
}

void loop() {
  Serial.println("");
  sensor();
  digitalWrite(red,LOW);
  digitalWrite(blue,LOW);
  digitalWrite(green,LOW);
  if (sensorValue < 60){
    digitalWrite(green,HIGH);
  } else {
    digitalWrite(blue,HIGH);
  }
  Serial.print("Sensor Value Is: ");
  Serial.print(sensorValue);
  Serial.print("-------");
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.print(port);
  post_request();
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.print("-------");
    Serial.print("connection failed");
    delay(5000);
    return;
  }

  // This will send a string to the server
//  if (client.connected()) {
//    client.println(sensorValue);
//  }

//  // wait for data to be available
//  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 5000) {
//      Serial.println(">>> Client Timeout !");
//      client.stop();
//      delay(60000);
//      return;
//    }
//  }

  // Read all the lines of the reply from server and print them to Serial
  //Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  //Serial.println();
  //Serial.println("closing connection");
  client.stop();

  delay(1001); // 
}
