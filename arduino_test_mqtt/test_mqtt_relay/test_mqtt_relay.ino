#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "IKBSP";
const char *pass = "ikbspikbsp";
const char *mqtt_server = "";
const int mqtt_port = 1883;
const char *mqtt_user = "MQTT_USER";
const char *mqtt_pass = "MQTT_PASS";

const int relePin = 3;

WiFiClient esp_client;
PubSubClient mqtt_client(esp_client);

void setup() {
  Serial.begin(115200);
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(callback);
 
  while (!mqtt_client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (mqtt_client.connect("ESP8266Client", mqtt_user, mqtt_pass )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(mqtt_client.state());
      delay(2000);
 
    }
  }
 
  mqtt_client.subscribe("socket/1");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  payload[length] = '\0';
  String str_topic = String(topic);
  String str_payload = String((char*)payload);
  Serial.println(str_payload);

  if(str_payload == "on"){
    digitalWrite(relePin, HIGH);
  }else if(str_payload == "off"){
    digitalWrite(relePin, LOW);
  }else{
    Serial.println("Error");
  }

 
 
}
 
void loop() {
  mqtt_client.loop();
}
