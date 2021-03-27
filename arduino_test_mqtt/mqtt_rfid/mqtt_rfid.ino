/* -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

 // server 192.168.0.105:5000/check?rfid=uid
 // other server 192.168.0.218:5000/check?rfid=uid

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "IKBSP"
#define STAPSK  ""
#define SERVER_IP "SERVER_IP"
#endif

#define SS_PIN D0
#define RST_PIN D4

const char* ssid     = STASSID;
const char* password = STAPSK;
/*IPAddress ip(192,168,1,220);
IPAddress gateway(192,168,1,254);
IPAddress subnet(255,255,254,0); */

MFRC522 mfrc522(SS_PIN, RST_PIN);
uint64_t uidDec, uidDecTemp;
String code;

void setup() 
{
  Serial.begin(115200); 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  SPI.begin();
  mfrc522.PCD_Init();
}

void print_uint64_t(uint64_t num);

void loop()  
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  uidDec = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }
  print_uint64_t(uidDec);
  if ((WiFi.status() == WL_CONNECTED)) 
  {
    StaticJsonDocument<64> doc;
    doc["rfid"] = code;
    code = "";
    String json;
    serializeJson(doc, json);
    WiFiClient client;
    HTTPClient http;
    http.begin(client, "http://" SERVER_IP);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json);
    if (httpCode > 0 )
    {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      delay(2000);
    } else {
       Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
   }
}

void print_uint64_t(uint64_t num) {

  char rev[128]; 
  char *p = rev+1;

  while (num > 0) {
    *p++ = '0' + ( num % 10);
    num/= 10;
  }
  p--;
  /*Print the number which is now in reverse*/
  while (p > rev) {
    code += *p--;
  }
}
