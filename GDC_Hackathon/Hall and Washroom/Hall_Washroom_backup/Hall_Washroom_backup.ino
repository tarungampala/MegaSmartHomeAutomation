#define BLYNK_TEMPLATE_ID "TMPL33Nb57NMh"
#define BLYNK_TEMPLATE_NAME "Room and Washroom"
#define BLYNK_AUTH_TOKEN "pBN3x0NTpvYSRo6ElsSG4Sh3JEkqGteo"

#define BLYNK_PRINT Serial
#include <Servo.h>
Servo servo1;
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
int position = 0;  
char auth[] = BLYNK_AUTH_TOKEN;
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 3 
DHT dht(DHTPIN, DHTTYPE);
char ssid[] = "Phone";
char pass[] = "12345678";
 int IR = D0 ;  //IR
const int trigPin = D5;   
 const int echoPin = D6;   
 long duration;  
 int distance;
 int tanker_level = 0;
BlynkTimer timer;
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V2){
  int value = param.asInt();
  if(value == 1){
    digitalWrite(D1, HIGH);
  }
  else{
    digitalWrite(D1, LOW);
  }
  Serial.println(value);
  }

BLYNK_WRITE(V5){
   int value = param.asInt();
   if(value == 1){
  for (position = 0; position <= 180; position++) { 
    servo1.write(position);  // Set position of Servo motor 1
    delay(10);                     
    }
   }
   else{
    for (position = 180; position >= 0; position--) { 
    servo1.write(position);  // Set position of Servo motor 1
    delay(15);               // Short delay to control the speed
    }
   }
   
  }

  BLYNK_WRITE(V7){
  int value = param.asInt();
  if(value == 1){
    digitalWrite(D4, HIGH);
  }
  else{
    digitalWrite(D4, LOW);
  }
  Serial.println(value);
  }

void tankerLevel(){
  digitalWrite(trigPin, LOW);  
 delayMicroseconds(2);  
 // Sets the trigPin on HIGH state for 10 micro seconds  
 digitalWrite(trigPin, HIGH);  
 delayMicroseconds(10);  
 digitalWrite(trigPin, LOW);  
 // Reads the echoPin, returns the sound wave travel time in microseconds  
 duration = pulseIn(echoPin, HIGH);  
 // Calculating the distance  
 distance= duration*0.034/2;  
 // Prints the distance on the Serial Monitor  
 //Serial.print("Distance: ");  
Serial.println(distance);  
 tanker_level = 100 - ((distance*100)/8);
 //Serial.println(tanker_level);
 Blynk.virtualWrite(V6, tanker_level); 
}

void tankerPump(){
  if(tanker_level < 20){
    digitalWrite(D4, HIGH);
    Blynk.virtualWrite(V7, 1); 
  }
  else if(tanker_level > 70){
    digitalWrite(D4, LOW);
    Blynk.virtualWrite(V7, 0);
  }
  //Serial.println(tanker_level);
}

void bathtemp(){
  float Humidity = dht.readHumidity();
  float Temperature = dht.readTemperature();
  //Serial.println(Temperature);
  //Serial.println(Humidity);
  Blynk.virtualWrite(V1, Humidity);
  Blynk.virtualWrite(V0, Temperature);
}
int prev = 1;
void ir(){
  int IRState = digitalRead(IR);
  Serial.println(IRState);
  if(IRState != prev){
    prev = IRState;
    if(IRState == 0){
   Blynk.virtualWrite(V5, 1); 
  for (position = 0; position <= 180; position++) { 
    servo1.write(position);  // Set position of Servo motor 1
    delay(10);                     
    }
   }
   else{
    Blynk.virtualWrite(V5, 0); 
    for (position = 180; position >= 0; position--) { 
    servo1.write(position);  // Set position of Servo motor 1
    delay(15);               // Short delay to control the speed
    }
   }
  }
}

void setup(){
  pinMode(D1, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D3, INPUT);
  pinMode(IR, INPUT);
  Serial.begin(115200);
  dht.begin(); 
  Blynk.begin(auth, ssid, pass);
  servo1.attach(D8);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output  
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input  
  timer.setInterval(1000L, tankerLevel);
  timer.setInterval(1000L, tankerPump);
  timer.setInterval(1000L, bathtemp);
  
}



void loop(){
  Blynk.run();
  timer.run();
  ir();

 
}
