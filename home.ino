#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>
#include "pin_define.h"
int servoStatus = 0;
int fanStatus = 0;
int temp = 0;
int humi = 0;
const unsigned long redLightDuration = 10000;
unsigned long redLightStart = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_NeoPixel strip(NUMPIXELS, WS2812_PIN, NEO_GRB + NEO_KHZ800);
Servo ser;
void setup() {
  pinMode(soundSensorPin, INPUT); // 声音传感器
  pinMode(waterSenserPin, INPUT); // 雨滴传感器
  pinMode(lightSensorPin, INPUT); // 光敏电阻
  pinMode(motionSensorPin, INPUT_PULLUP); // ！必须上拉！
  pinMode(redLightPin, OUTPUT); // 红色的LED灯，模拟房屋前大灯
  pinMode(buzzerPin, OUTPUT); // 蜂鸣器
  pinMode(relayPin, OUTPUT); // 继电器触发端口，低电平有效
  Serial.begin(9600);
  ser.attach(servoControlPin);
  dht.begin();
  strip.begin();   // 初始化
  strip.show();    // 更新显示
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED 初始化失败"));
    while (true);
  }
}
void loop() {
  read_temp();
  controll_fan();
  controll_motion();
  controll_redled();
  controll_servo();
  delay(500);
  display.display();
}

void read_temp(){
  temp = dht.readTemperature(); //温度数据
  humi = dht.readHumidity();    //湿度数据
  display.clearDisplay();  // 清空显示
  display.setTextSize(1);  // 设置文本大小
  display.setTextColor(SSD1306_WHITE);  // 设置文本颜色
  display.setCursor(13, 10);  // 设置光标位置
  display.print(" temp:");
  display.println(temp);
  display.setCursor(70, 10);
  display.print(" humi:");
  display.println(humi);
  display.drawRect(6, 6, 121, 57, SSD1306_WHITE); // 画一个白色正方形边框
  display.drawLine(6, 23, 126, 23, SSD1306_WHITE); // 画一条横线
}
// 全局变量
bool lightOn = false;
bool motionAlert = false;

// 红灯控制（只改逻辑，不改显示区）
void controll_redled() {
  if (digitalRead(lightSensorPin) && digitalRead(soundSensorPin)) {
    lightOn = true;
    redLightStart = millis();
  }
  // 超时自动关灯
  if (lightOn && millis() - redLightStart > redLightDuration) {
    lightOn = false;
  }
  digitalWrite(redLightPin, lightOn ? HIGH : LOW);

  // OLED显示，位置不变
  display.setCursor(10, 28);
  display.print(String("Light:") + String(lightOn ? "On" : "Off"));
}

// 运动检测报警（只改逻辑，不改显示区）
void controll_motion() {
  if (!digitalRead(motionSensorPin)) {
    motionAlert = true;
    digitalWrite(buzzerPin, HIGH);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(30, 0, 0));
    }
    strip.show();
    // 红灯报警状态也亮
    lightOn = true;
    redLightStart = millis(); // 运动触发也更新亮灯时间
    Serial.println("\xb7\xc7\xb7\xa8\xc8\xeb\xc7\xd6");
    delay(2500);
  } else {
    motionAlert = false;
    digitalWrite(buzzerPin, LOW);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  }
  // OLED显示，位置不变
  display.setCursor(10, 28);
  display.println(String("Light:") + String(lightOn ? "On" : "Off"));
  display.setCursor(75, 28);
  display.println(String("Alarm:") + String(motionAlert ? "T" : "F"));
}
void controll_servo(){
  if (digitalRead(waterSenserPin) && servoStatus != 0) {
    servoStatus = 0;
    ser.write(90); // 设置角度为90
  } else if (!digitalRead(waterSenserPin) && servoStatus == 0){
    servoStatus = 1;
    ser.write(0);  // 设置角度为0                    
    Serial.print("\xc1\xc0\xd2\xc2\xbc\xdc\xca\xd5\xbb\xd8");
  }
  display.setCursor(10, 43);
  display.println(String("Servo:") + String(servoStatus == 0 ? "On" : "Off"));
}
void controll_fan(){
  if (temp >= 29) {
    digitalWrite(relayPin, LOW);  // 低电平触发继电器工作，风扇转动
    if (fanStatus == 0) {
      Serial.print("\xb7\xe7\xc9\xc8\xd2\xd1\xb4\xf2\xbf\xaa");
      fanStatus = 1;
    }
  } else {
    digitalWrite(relayPin, HIGH); // 高电平不处触发继电器工作，风扇不转动
    if (fanStatus == 1) {
      Serial.print("\xb7\xe7\xc9\xc8\xd2\xd1\xb9\xd8\xb1\xd5");
      fanStatus = 0;
    }
  }
  display.setCursor(75, 43);
  display.println(String("Fan:") + String(fanStatus ? "On" : "Off"));
}
