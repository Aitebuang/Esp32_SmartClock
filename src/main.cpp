#include "include.h"       
#include "baiduASR.h"
#include "baiduAsrRemind.h"
#include "getTime.h"
#include "getWeather.h"
#include "get3daysWeather.h"
#include "commandSelect.h"
#include "lvgl.h"
#include "gui.h"
#include "display.h"
#include <WiFiUdp.h>
#include "getDatas.h"
#include "esp_task_wdt.h"

//多核---------------------------------------------------------------------------------------------------------
#define ARDUINO_RUNNING_CORE_0 0
#define ARDUINO_RUNNING_CORE_1 1
//Udp无线开灯相关-----------------------------------------------------------------------------------------------

WiFiUDP Udp;//建立udp对象
unsigned int UdpPort = 2222;  // 设置本地端口
char incomingPacket[10];  // 接受的信息放置在这个字符数组里面
extern char  replyPacekt_onn[];  //回复客户端的信息，可以任意修改。该值为“onn”则开灯，为“off”则关掉灯。
extern char  replyPacekt_off[];  //回复客户端的信息，可以任意修改。该值为“onn”则开灯，为“off”则关掉灯。
//------------------------------------------------------------------------------------------------------------- 

Display screen; //屏幕
extern hw_timer_t * timer;  //定时器 

//-------------------------------------------------------------------------------------------------------------
/* 显示文本 */
extern lv_obj_t * label_Hour;
extern lv_obj_t * label_Minute;
extern lv_obj_t * label_Date;
extern lv_obj_t * label_weather; //天气
extern lv_obj_t * label_remind;  //提醒事项
extern char timeHour_display[10];
extern char timeMinute_display[10];
extern char timeDate_display[20];
extern char weather_text_display1[10];
extern long weather_tem;            //温度
extern const char* weather_text;
//-------------------------------------------------------------------------------------------------------------
/* 队列相关 */
QueueHandle_t myqueue;
#define QUEUE_LENGTH 5
#define QUEUE_ITEM_SIZE sizeof( int )
//-------------------------------------------------------------------------------------------------------------

void setup() {

    /* 波特率和引脚初始化 */
    Serial.begin(115200);
    pinMode(ADC,ANALOG);     
    pinMode(KEY,INPUT_PULLUP);
    pinMode(LED,OUTPUT);      

    /* WIFI连接 */
    uint8_t count=0;
    // wifi切换：  003 2829zwz. XF xfbg8275088  Buang 8208820a Xiaomi wzn13632347614
    WiFi.mode(WIFI_MODE_APSTA);//设置WIFI模式为AP+STA模式
    WiFi.softAP("ESP", "Rangetest");//设置AP模式的WIFI账号和密码
    Udp.begin(UdpPort);//设置udp端口      
    WiFi.begin("003", "2829zwz.");//设置STA连接的WIFI名称和密码
    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.print(".");
        count++;
        if(count>=75){
            Serial.printf("\r\n-- wifi connect fail! --");
            break;
        }
        vTaskDelay(200);
    }
    Serial.printf("\r\n-- WIFI连接成功! --\r\n");
    /* Esp32时钟设置 */
    timer = timerBegin(0, 80, true);    //  80M的时钟 80分频 1M
    timerAlarmWrite(timer, 125, true);  //  1M  计125个数进中断  8K
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmEnable(timer);
    timerStop(timer);   //先暂停

    screen.init();  //屏幕包括lvgl初始化
    gui_setup();    //运行ui
    

    /* 创建普通队列 */
    myqueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);

    /* 创建线程任务 */
    // xTaskCreatePinnedToCore(
    // baiduASR        //百度语音识别
    // ,"TaskbaiduASR" //此任务名为"TaskbaiduASR"，可自定义
    // ,10240          // 任务堆栈大小，如果提示任务堆栈溢出，就改这个 This stack size can be checked & adjusted by reading the Stack Highwater
    // ,NULL
    // ,0              // 任务优先级：Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    // ,NULL
    // ,ARDUINO_RUNNING_CORE_0);

    // xTaskCreatePinnedToCore(
    // commandSelect
    // ,"TaskcommandSelect"
    // ,1024
    // ,NULL
    // ,1
    // ,NULL
    // ,ARDUINO_RUNNING_CORE_0);

    // xTaskCreatePinnedToCore(
    // getDatas
    // ,"TaskgetDatas"
    // ,10240
    // ,NULL
    // ,2
    // ,NULL
    // ,ARDUINO_RUNNING_CORE_1);

    xTaskCreatePinnedToCore(
    screen.routine
    ,"Taskscreenroutine"
    ,10240
    ,NULL
    ,3
    ,NULL
    ,ARDUINO_RUNNING_CORE_0);

}


void loop() 
{   
    baiduASR();
    commandSelect();
    getTime(); 

    lv_event_send(label_Minute,LV_EVENT_REFRESH,NULL); //屏幕参数刷新

    // Serial.printf("loop\r\n");

    // esp_task_wdt_reset();     //看门狗复位
    
}


   
/* 硬件连接
MAX9814正常供电，然后把MAX9814的OUT引脚接ESP32的GPIO39，GAIN和AR两个引脚悬空即可。
*/


