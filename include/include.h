#ifndef  __Include_H__
#define  __Include_H__

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
#include "../lib/ArduinoJson/ArduinoJson.h"
#include "base64.h"
#include "cJSON.h"

//c语言中，宏定义一般全大写，小写为变量，此处小写会与lvgl内部变量重复，报错
#define KEY 25       //端口25   按键引脚
#define ADC 39      //端口39  接麦克风out端口
#define LED 26       //端口26   led控制引脚





#define data_len 16000


extern int commandNumber;
extern const char* asr_text;
extern const char* asr_command;

extern const char* conmmandMainDisplay;  
extern const char* conmmandWeather;  
extern const char* conmmandRemind;


void select(void);
// void LightOn();
// void LightOff();
#endif  









