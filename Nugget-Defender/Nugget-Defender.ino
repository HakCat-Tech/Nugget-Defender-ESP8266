#include "fonts.h"
#include "Hardware.h"

#include "ESP8266WiFi.h"

#include "SH1106Wire.h"
SH1106Wire display(0x3C, SDA, SCL); // use builtin i2C

uint8_t numNetworks;
uint8_t currentNetwork= 0;
uint8_t cursorPos = 0;

int prevPress=0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(upBtn, INPUT_PULLUP);
  pinMode(dnBtn, INPUT_PULLUP);
  pinMode(ltBtn, INPUT_PULLUP);
  pinMode(rtBtn, INPUT_PULLUP);

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(DejaVu_Sans_Mono_10);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100); 
  display.drawString(5,28,"Nugget Defender 1.0");
  display.display();
  scan();
  
  
  display.clear();
  draw_AP_menu();
  update_AP_list();
  display.display();
}

  String ssid;
  uint8_t encryptionType;
  int32_t RSSI;
  uint8_t* BSSID;
  int32_t channel;
  bool isHidden;
  
void scan() {
  Serial.print("Scan start ... ");
  numNetworks = WiFi.scanNetworks(false,true);
  
  
  for (int i = 0; i < numNetworks; i++)
  {
    WiFi.getNetworkInfo(i, ssid, encryptionType, RSSI, BSSID, channel, isHidden);
    Serial.printf("%d: %s, Ch:%d (%ddBm) %s %s\n", i + 1, ssid.c_str(), channel, RSSI, encryptionType == ENC_TYPE_NONE ? "open" : "", isHidden ? "hidden" : "");
  }

}

void update_AP_list() {
  for (uint8_t i=(cursorPos/5)*5; i<((cursorPos/5)*5)+5; i++) {
    if (i>=numNetworks) break;   
    WiFi.getNetworkInfo(i, ssid, encryptionType, RSSI, BSSID, channel, isHidden);
    display.drawString(3,(i%5)*10,isHidden ? "*hidden network*" : ssid.c_str());
  }
}

void draw_AP_menu() {
  display.drawLine(0,54, 127,54); display.drawLine(0,53, 127,53);
  display.drawLine(37,54,37,63);  display.drawLine(38,54,38,63);
  display.drawLine(78,54,78,63);  display.drawLine(77,54,77,63);
  display.fillTriangle(1, 60, 4, 57, 4, 63);
  display.fillTriangle(126, 60, 123, 57, 123, 63);
  display.drawString(9,54,"Back"); display.drawString(83,54,"Select");
  
  display.drawString(43,54,(cursorPos<10 ? "0"+(String) (cursorPos+1):(String) (cursorPos+1)) + "/" + (numNetworks<10 ? "0"+(String) numNetworks:(String) numNetworks));
  display.drawLine(0,(cursorPos%5*10)+2, 0,(cursorPos%5*10)+10);
  
}

void select_AP() {
  uint8_t press = buttonsPressed();
  if (press==2 && cursorPos<numNetworks-1 && prevPress!=press) { cursorPos++; }
  else if (press==1 && cursorPos>0 && prevPress!=press) { cursorPos--; }
  
  if (press>0) {
    display.clear();
    draw_AP_menu();
    update_AP_list();
    display.display();
    prevPress=press;
  }
  else {
    prevPress=0;
  }
}

void loop() {
  select_AP(); // choose an access point to log
}
