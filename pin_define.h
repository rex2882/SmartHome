#include <Servo.h>
#define WS2812_PIN     4
#define NUMPIXELS      8   // LED 数量
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define DHT_TYPE DHT11
#define DHT_PIN 2
#define soundSensorPin 6      // 数字声音传感器
#define lightSensorPin 10     // 数字光线传感器
#define motionSensorPin 7     // 红外传感器(必须上拉)
#define redLightPin 3         // 红色LED灯(前大灯)
#define buzzerPin 5           // 蜂鸣器
#define relayPin 11           // 继电器控制引脚(低电平有效)
#define servoControlPin 8     // 伺服电机控制引脚
#define waterSenserPin 9      // 雨滴传感器