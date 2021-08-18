#ifndef __CC3200R1M1RGC__ 
#include <SPI.h> 
#endif 
#include <WiFi.h> 
#include <Wire.h> 
#include "Adafruit_TMP006.h" 
#include <stdlib.h> 
char thingSpeakAddress[] = "api.thingspeak.com"; 
String writeAPIKey = "S5CEPJUL7KFTHSES"; 
const int updateThingSpeakInterval = 16 * 1000; // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interva 
char buffer[25]; 
char ssid[] = "R2C";
char password[] = "r2cudt@2021";
WiFiClient client;
unsigned long lastConnectionTime = 0; // last time you connected to the server, in milliseconds 
boolean lastConnected = false; // state of the connection last time through the main loop 
const unsigned long postingInterval = 10*1000; 
int failedCounter = 0; 
Adafruit_TMP006 tmp006(0x41); // start with a diferent i2c address! 
void setup() {
 Serial.begin(115200);
 pinMode(23,INPUT);//ldr
 pinMode(2,OUTPUT);//led1
 pinMode(4,OUTPUT);//led2
 pinMode(6,OUTPUT);//led3
 pinMode(8,OUTPUT);//led4
 pinMode(10,OUTPUT);//buzzer
Serial.print("Attempting to connect to Network named: ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while ( WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(300); 
} 
if (! tmp006.begin()) { 
Serial.println("No sensor found"); 
while (1); 
}
Serial.println("\nYou're connected to the network");
Serial.println("Waiting for an ip address"); 
while (WiFi.localIP() == INADDR_NONE) { 
Serial.print("."); 
delay(300); 
} 
Serial.println("\nIP Address obtained"); 
printWifiStatus(); 
}
void loop() { 
while (client.available()) { 
char c = client.read(); 
Serial.print(c); 
} 
if (!client.connected() && lastConnected) { 
Serial.println(); 
Serial.println("disconnecting."); 
client.stop(); 
} 
if (!client.connected() && (millis() - lastConnectionTime > postingInterval)) { 
float objt = tmp006.readObjTempC();
int ldr= analogRead(23);
float voltage= ldr * (3.0/4096);
Serial.print("VOLTAGE: "); 
Serial.print(voltage);
String sobjt = dtostrf(objt,3,3,buffer);
String sldr = dtostrf(ldr,3,3,buffer); 
String svolt = dtostrf(voltage,3,3,buffer);
Serial.print(sobjt);
Serial.println("*C");
Serial.print(sldr);
Serial.print(svolt);
Serial.println("*C");

if(voltage > 3)
{
 Serial.println("Maximum Solar rays recieved");
 digitalWrite(2,HIGH);
 digitalWrite(4,HIGH);
 digitalWrite(6,HIGH);
 digitalWrite(8,HIGH);
 digitalWrite(10,LOW);//buzzer not needed as maximum solar rays recieved
}

if(voltage < 3 && voltage > 2)
{
 Serial.println("Moderate Solar rays recieved");
 digitalWrite(2,HIGH);
 digitalWrite(4,HIGH);
 digitalWrite(6,HIGH);
 digitalWrite(8,LOW);//only 3 leds glow as moderate solar rays recieved
 digitalWrite(10,LOW);//buzzer not needed as moderate amt of solar rays recieved
}

if(voltage < 2 && voltage > 1)
{
 Serial.println("Very less Solar rays recieved");
 digitalWrite(2,HIGH);
 digitalWrite(4,HIGH);
 digitalWrite(6,LOW);
 digitalWrite(8,LOW);
 digitalWrite(10,LOW);//buzzer not needed as still considerable solar rays recieved
}

if(voltage < 1)
{
 Serial.println("Warning !! Adequate amount of solar rays not available");
 digitalWrite(2,HIGH);
 digitalWrite(4,LOW);
 digitalWrite(6,LOW);
 digitalWrite(8,LOW);
 digitalWrite(10,HIGH);//buzzer rings as required amt of solar rays is not recieved
}
updateThingSpeak("field1=" + sobjt+"&field2=" + sldr+ "field3=" + svolt); 
} 
lastConnected = client.connected();
 
} 
void updateThingSpeak(String tsData) 
{ 
if (client.connect(thingSpeakAddress, 80)) 
{
client.print("POST /update HTTP/1.1\n"); 
client.print("Host: api.thingspeak.com\n"); 
client.print("Connection: close\n"); 
client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n"); 
client.print("Content-Type: application/x-www-form-urlencoded\n"); 
client.print("Content-Length: "); 
client.print(tsData.length()); 
Serial.println(">>TSDATALength=" + tsData.length()); 
client.print("\n\n");
client.print(tsData);
Serial.println(">>TSDATA=" + tsData);
lastConnectionTime = millis();
if (client.connected()) 
{ 
Serial.println("Connecting to ThingSpeak...");
Serial.println();
failedCounter = 0; 
}
else
{ 
failedCounter++;
Serial.println("Connection to ThingSpeak failed ("+String(failedCounter, DEC)+")");
Serial.println();
} 
} 
else 
{ 
failedCounter++; 
Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");
Serial.println(); 
lastConnectionTime = millis(); 
} 
}
int getLength(int someValue) { 
int digits = 1; 
int dividend = someValue / 10; 
while (dividend > 0) { 
dividend = dividend / 10; 
digits++; 
} 
return digits; 
} 
void printWifiStatus() {
Serial.print("SSID: ");
Serial.println(WiFi.SSID());
IPAddress ip = WiFi.localIP(); 
Serial.print("IP Address: "); 
Serial.println(ip); 
long rssi = WiFi.RSSI(); 
Serial.print("signal strength (RSSI):"); 
Serial.print(rssi); 
Serial.println(" dBm"); 
}
