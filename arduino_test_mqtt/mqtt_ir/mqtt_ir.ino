#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
const uint16_t kIrLed = 12;  // Выбор пина для ИК-светодиода
IRsend irsend(kIrLed);
const char *ssid = "SSID";
const char *pass = "PASS";
const char *mqtt_server = "IP";
const int mqtt_port = 1883;
const char *mqtt_user = "MQTT_USER";
const char *mqtt_pass = "MQTT_PASS";


WiFiClient esp_client;
PubSubClient mqtt_client(esp_client);

void setup() {
 Serial.begin(115200);
 WiFi.begin(ssid, pass);
 pinMode(12, OUTPUT);
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
 
 mqtt_client.subscribe("tv/1");
 mqtt_client.subscribe("projector/1");
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 if (String(topic) == "tv/1") {
 payload[length] = '\0';
 String str_topic = String(topic);
 String str_payload = String((char*)payload);
 Serial.println(str_payload);

 if(str_payload == "on"){
 irsend.sendCOOLIX(0x22DE8B05);
 }else if(str_payload == "off"){
 irsend.sendCOOLIX(0x22DE8B05);
 }else{
 Serial.println("Error");
 }
 }
 if (String(topic) == "projector/1") {
 payload[length] = '\0';
 String str_topic = String(topic);
 String str_payload = String((char*)payload);
 Serial.println(str_payload);

 if(str_payload == "on"){
 irsend.sendNEC(0xC1AA09F6);
 }else if(str_payload == "off"){
 irsend.sendNEC(0xC1AA09F6);
 }else{
 Serial.println("Error");
 }
 }
 
}
 
void loop() {
 mqtt_client.loop();
}
