#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#define WS2812_PIN     4
#define NUMPIXELS      8   // LED 数量
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define DHT_TYPE DHT11
#define DHT_PIN 2
Servo ser;
int servoStatus = 0;
int temp;
int humi;
// 使用 I2C 地址 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_NeoPixel strip(NUMPIXELS, WS2812_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(6, INPUT); // 声音传感器
  pinMode(10, INPUT); // 光敏电阻
  pinMode(7, INPUT_PULLUP); // ！必须上拉！
  pinMode(3, OUTPUT); // 红色的LED灯，模拟房屋前大灯
  pinMode(5, OUTPUT); // 蜂鸣器
  pinMode(11, OUTPUT); // 继电器触发端口，低电平有效
  Serial.begin(9600);
  ser.attach(8);
  dht.begin();
  strip.begin();   // 初始化
  strip.show();    // 更新显示
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED 初始化失败"));
    while (true);
  }
  
}

void loop() {
  temp = dht.readTemperature(); //温度数据
  humi = dht.readHumidity();    //湿度数据
  display.clearDisplay();  // 清空显示
  display.setTextSize(2);  // 设置文本大小
  display.setTextColor(SSD1306_WHITE);  // 设置文本颜色
  display.setCursor(12, 12);  // 设置光标位置
  display.print(" temp:");
  display.println(temp);
  display.setCursor(12, 36);
  display.print(" humi:");
  display.println(humi);
  display.drawRect(6, 6, 121, 57, SSD1306_WHITE); // 画一个白色正方形边框
  display.display();  // 更新屏幕显示
  if (digitalRead(10) && digitalRead(6)) {  // 6 -> 声音传感器, 10 -> 光敏电阻 模拟天黑后 房屋前红色LED大灯亮
    digitalWrite(3, HIGH); 
    delay(5000); // 不然红色大灯会闪烁
  } else {
    digitalWrite(3, LOW); // 红色LED大灯关闭
  } 
  if (!digitalRead(7)) { // 7 -> 红外对射传感器，无遮挡输出1,有遮挡输出0。（可能受外部光线影响，给发射头和接收头加了一个“帽子”）
    digitalWrite(5, HIGH); // 5 -> 打开蜂鸣器
    for (int i = 0; i < strip.numPixels(); i++) {
      // 将每个 LED 设置为红色
      strip.setPixelColor(i, strip.Color(30, 0, 0));  
      strip.show();
    }
    digitalWrite(3, HIGH); 
    delay(5000); // 不然灯会闪烁
  } else {
    digitalWrite(5, LOW); // 关闭 -> 蜂鸣器
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  
      strip.show();
    }
    digitalWrite(3, LOW); 
  }
  if (digitalRead(9) && servoStatus != 0) {
    servoStatus = 0;
    ser.write(90); // 设置角度为90
  } else if (!digitalRead(9) && servoStatus == 0){
    servoStatus = 1;
    ser.write(0);  // 设置角度为0                        
  }
  if (temp >= 31) {
    digitalWrite(11, LOW);  // 低电平触发继电器工作，风扇转动
  } else {
    digitalWrite(11, HIGH); // 高电平不处触发继电器工作，风扇不转动哟
  }
  delay(500);
}

