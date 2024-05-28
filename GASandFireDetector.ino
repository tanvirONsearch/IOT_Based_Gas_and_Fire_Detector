//ifttt
//https://maker.ifttt.com/trigger/{NodeMCU}/json/with/key/fQis6742chTClaM3s_ucUFeyFIg6NOX24EiS_inDk6L
#include<DNSServer.h>
#include<WiFiManager.h>
#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>

WiFiClient client;
HTTPClient http;
int code;
void connectTowifi(void);

int state=0;
int Fireop = D5;
int Gasop = D4;
int Fire,Gas;
String web="http://api.thingspeak.com/update?api_key=JDER6CE1W9W5JEZ7&field1=";

unsigned long ctym=millis();
unsigned long interval=16000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  connectTowifi();
  pinMode(Fireop, INPUT);
  pinMode(Gasop, INPUT);
  pinMode(D8, OUTPUT);
  pinMode(D1,OUTPUT);


}

void loop() {
   if(WiFi.status()==WL_CONNECTED)
      digitalWrite(D1,HIGH);
   else digitalWrite(D1,LOW);
  
  Fire = digitalRead(Fireop);
  Gas = digitalRead(Gasop);

  Serial.print("    Fire op = ");
  Serial.print(Fire);
  Serial.print("    Gas op = ");
  Serial.println(Gas);
  
  if (Fire == 0 || Gas==0) {
    state = 50;
    digitalWrite(D8,HIGH);
    delay(2000);
     digitalWrite(D8,LOW);
   
  }
  web = web+state;

  if(state==50 &&(millis()-ctym<=interval)){
    digitalWrite(D8,HIGH);
    delay(14000);
     digitalWrite(D8,LOW);
    http.begin(client,"http://api.thingspeak.com/update?api_key=JDER6CE1W9W5JEZ7&field1=50");
    code = http.GET();
    if (code > 0 )Serial.println("Fire detected");
    state=0;
    web="http://api.thingspeak.com/update?api_key=JDER6CE1W9W5JEZ7&field1=";
    ctym=millis();

  }

  if(millis()-ctym>=interval){
    state=0;
    web = web+state;
    http.begin(client,web);
    code = http.GET();
    if (code > 0 ){
      Serial.println("Data sent");
      ctym=millis();
      web="http://api.thingspeak.com/update?api_key=JDER6CE1W9W5JEZ7&field1=";
    }
  }
 
}

void connectTowifi()
{
  WiFiManager wifi;

  wifi.autoConnect("Gas_Fire_Detector");
  Serial.println(" connected ");

}
