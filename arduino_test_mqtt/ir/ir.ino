#include <IRremoteESP8266.h>
#include <IRsend.h>

IRsend irsend(4);

void setup(){
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  delay(10);
  pinMode(4, OUTPUT);
  digitalWrite(4,0);
}
void loop(){
  int ir_code;
  ir_code = 0xC1AA09F6;
  Serial.println("NEC: ");
  Serial.println(ir_code, HEX);
  irsend.sendNEC(ir_code, 32);
  delay(10000);
}
