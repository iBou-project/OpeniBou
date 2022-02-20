#define BLYNK_PRINT Serial

#include <WiFi.h>
#include "blynk-test.h"

#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <M5StickCPlus.h>

WidgetTerminal terminal(V0);

void setup()
{
  char auth[] = BLYNK_AUTH_TOKEN;
  char ssid[] = WIFI_SSID;
  char pass[] = WIFI_PASS;

  M5.begin();
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  M5.Axp.ScreenBreath(10);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setCursor(5, 10);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("Hello World!");

  pinMode(33, OUTPUT);
}

void loop()
{
  M5.update();
  Blynk.run();

  if (M5.BtnA.wasPressed())
  {
    Blynk.notify("OK!!");
    Serial.printf("OK");
    terminal.println("OK!!");
    terminal.flush();

    M5.Lcd.fillScreen(WHITE);
    M5.Lcd.printf("OK!!");
  }
    
  if (M5.BtnB.wasPressed())
  {
    Blynk.notify("NG");
    Serial.printf("NG");
    terminal.println("NG");
    terminal.flush();

    M5.Lcd.fillScreen(RED);
    M5.Lcd.printf("NG!!");
  }
}

int f = 0;

BLYNK_WRITE(V1)
{
  Serial.printf("Blynk_read = %s\r\n", param[0].asStr());
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.printf("here\n");

  if ( f ) {
    digitalWrite(33, HIGH);
    delay(1000);
  } else {
    digitalWrite(33, LOW);
    delay(1000);
  }
  f = 1 - f;
}
