#include <M5StickCPlus.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("M5 Presenter");
bool blestate = false;
bool easy_mode = false;

float v = 1.0;
float max_v = 0.0;

void showxyz(float ax, float ay, float az)
{
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.fillRect(0, 80, 320, 20, BLACK);
    M5.Lcd.printf("%c%d %d %d", (easy_mode?'*':' '), (int)(ax*1000), (int)(ay*1000), (int)(az*1000));
}

void showacc(float v) {

    M5.Lcd.setCursor(0, 100);
    M5.Lcd.fillRect(0, 100, 320, 20, BLACK);
    M5.Lcd.printf("%4d %4d", (int)(v*1000),(int)(max_v*1000));
}

void showstate(char *txt) {
    M5.Lcd.setCursor(0, 120);
    M5.Lcd.fillRect(0, 120, 320, 20, BLACK);
    M5.Lcd.printf(txt);
}
 
void setup() {
    M5.begin();
    Serial.begin(115200);
 
    bleKeyboard.begin();

    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("Presenter");
    M5.Lcd.println("A: Page Down");
    M5.Lcd.println("B: Page Up");

    M5.IMU.Init();
    M5.IMU.SetAccelFsr(M5.IMU.AFS_4G);

    easy_mode = true;
    showstate("Disconnected");
}

float ax, ay, az;
#define SAMPLE_PERIOD 200

void loop() {
    M5.update();
 
    if (bleKeyboard.isConnected()) {
        if (!blestate) {
            blestate = true;
            showstate("Connected");
        }
 
        M5.IMU.getAccelData(&ax,&ay,&az);
        /*
        Serial.print((int)(ax*1000));
        Serial.print(" ");
        Serial.print((int)(ay*1000));
        Serial.print(" ");
        Serial.println((int)(az*1000));
        */

        if (M5.BtnA.wasPressed()) {
            v = (float)sqrt(ax*ax + ay*ay + az*az);
            if ( v > max_v ) {
              max_v = v;
            }
            showxyz(ax, ay, az);
            
            showacc(v);
            if (( v > 1.3 ) || easy_mode) {
              bleKeyboard.press(KEY_DOWN_ARROW);
              bleKeyboard.release(KEY_DOWN_ARROW);
            }

     
        } else if (M5.BtnB.wasPressed()) {
            bleKeyboard.press(KEY_UP_ARROW);
            bleKeyboard.release(KEY_UP_ARROW);
            if ( v > 1.3 ) {
                easy_mode = !easy_mode;
            }
            max_v = 0.0;
        }
    } else {
        if (blestate) {
            blestate = false;
            showstate("Disconnected");
        }
    }
}
