#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#include <DHT.h>;
#define DHTPIN 0
DHT dht(DHTPIN, DHT11);  
const unsigned char buzz =14;
const char* ssid = "Madaya"; 
const char* password = "1112223333";
int val;
int LDRpin = A0; //LDR Pin Connected at A0 Pin

WiFiClient client;

String apiKey = "197GD6ZDZT56AZM6"; 

unsigned long myChannelNumber = 1123344;
const char * myWriteAPIKey = "197GD6ZDZT56AZM6"; 
const char * readAPIKey="3QU083MYYDZXQR43";
unsigned int dataFieldOne=1;
const char* server = "api.thingspeak.com";
float aConst=2.25E-02;

void setup()
{
Serial.begin(9600);
delay(10);
dht.begin();
// Connect to WiFi network
WiFi.begin(ssid, password);
ThingSpeak.begin(client);
pinMode(buzz,OUTPUT);
}

void loop()
{
val = analogRead(LDRpin); 
Serial.print(val); 
ThingSpeak.writeField(myChannelNumber, 1,val, myWriteAPIKey); 
aConst=readTSData(myChannelNumber,dataFieldOne);
if(val<1024)
{
  digitalWrite(buzz,HIGH);
  delay(1000);
}
else
{
  digitalWrite(buzz,LOW);
  delay(1000);
}
float h = dht.readHumidity(); 
float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }

                         if (client.connect(server,80))   
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field2=";
                             postStr += String(t);
                             postStr +="&field3=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
delay(1000);
}

float readTSData(long TSChannel,unsigned int TSField){
  float data=ThingSpeak.readFloatField(TSChannel,TSField,readAPIKey);
  Serial.println("Data read from ThingSpeak:"+ String(data,9));
  return data;
}
