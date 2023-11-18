#include <DHT.h>
#include <DHT_U.h>

#define BLYNK_TEMPLATE_ID "TMPL3DE60e5ON"
#define BLYNK_TEMPLATE_NAME "Kitchen and Lawn"
#define BLYNK_AUTH_TOKEN "0hoGS7_uEw1yCQ5GLxYp0jUSQx1dIM5H"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h> 
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Phone";
char pass[] = "12345678";
BlynkTimer timer;

//Soil moisture
int sense_Pin = 0; // sensor input at Analog pin A0
int value = 0;


BLYNK_WRITE(V7){
  int value = param.asInt();
  if(value == 1){
    digitalWrite(D4, HIGH);
  }
  else{
    digitalWrite(D4, LOW);
  }
  //Serial.println(value);
  }

  
void soilMoisture(){
  value= analogRead(sense_Pin);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V2, value);
}


void plantWater(){
  if(value < 30){
    digitalWrite(D4, HIGH);
    Blynk.virtualWrite(V7, 1); 
  }
  else if(value >  30) {
    digitalWrite(D4, LOW);
    Blynk.virtualWrite(V7, 0);
  }
  Serial.println(value);
}

void setup(){
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, soilMoisture);
}



void loop(){
  Blynk.run();
  timer.run();
  plantWater();
}
