

#include <DHTesp.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial SerialEsp(4,0); //rx tx

char data;

 // Definimos el pin digital donde se conecta el sensor
//#define DHTPIN 2
// Dependiendo del tipo de sensor
//#define DHTTYPE DHT11
 
// Inicializamos el sensor DHT11
DHTesp dht;

const char* ssid = "frank";
const char* password = "11223344";

String apiKey="VRXV96TC40IQ1PQP";
const char* server = "api.thingspeak.com";

unsigned long previousMillis = 0;


void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(115200);

  SerialEsp.begin(9600);

  WiFi.begin(ssid, password);
  while( WiFi.status() != WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi conectado satisfactoriamente");
  Serial.println(WiFi.localIP());

  dht.setup(5, DHTesp::DHT11);
}

void loop() {
 unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 5000) { //envia la temperatura cada 5 segundos
    previousMillis = currentMillis;
    int analog = analogRead(17);
    float temp = analog*0.322265625;
     float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  
    enviarTempTS(temperature);
    Serial.println(temperature);
    
    enviarHumTS(humidity);
    Serial.println(humidity);

  }
  checkSerialCom();
  
}

void checkSerialCom(){
  if( SerialEsp.available() > 0){
    Serial.println("recibido:");
    while(SerialEsp.available()> 0){
      data = SerialEsp.read();
      Serial.println(data);
    }
  }
}
void enviarTempTS(float temp)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // usar 184.106.153.149 ó api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(temp);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }
 client.stop();
}


void enviarHumTS(float hum)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // usar 184.106.153.149 ó api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field2=";
   postStr += String(hum);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }
 client.stop();
}
