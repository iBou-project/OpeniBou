#define BLYNK_PRINT Serial

#include <WiFi.h>
#include "my-blynk-info.h"
#include "image.h"

#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <M5StickCPlus.h>

void setup()
{
  char auth[] = BLYNK_AUTH_TOKEN;
  char ssid[] = WIFI_SSID;
  char pass[] = WIFI_PASS;

  M5.begin();
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  M5.Axp.ScreenBreath(10);
  M5.Lcd.setRotation(0);
  /*
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setCursor(5, 10);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("iBou by Komorebi");
  */
  M5.Lcd.pushImage(0, 0, IMG_WIDTH, IMG_HEIGHT, IMG_DATA[1]);

  pinMode(33, OUTPUT);
}

void loop()
{
  M5.update();
  Blynk.run();
}

BLYNK_WRITE(V1)
{
  Serial.printf("Button V1 = %d\r\n", param[0].asStr());

  /*
  M5.Lcd.setCursor(0, 120);
  M5.Lcd.fillRect(0, 120, 320, 20, BLACK);
  M5.Lcd.printf("V1:%d", param[0]);
  */

  if ( param[0] == 1 ) {
    digitalWrite(33, HIGH);
    M5.Lcd.pushImage(0, 0, IMG_WIDTH, IMG_HEIGHT, IMG_DATA[1]);
  } else {
    digitalWrite(33, LOW);
    M5.Lcd.pushImage(0, 0, IMG_WIDTH, IMG_HEIGHT, IMG_DATA[0]);
  }
}
