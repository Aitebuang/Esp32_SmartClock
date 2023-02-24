#include "getDatas.h"
#include "getTime.h"
#include "getWeather.h"
#include "get3daysWeather.h"
#include "WiFiUdp.h"
#include "esp_task_wdt.h"


// int count_getTime = 500000;
// int count_getWeather = 100;
void getDatas(void *pvParameters)
{

    (void)pvParameters;

    while(1) //任务执行while(1)里的内容
    {     
        
        getTime();
        // get3daysWeather();
        // Serial.println("getDatas");
        // delay(1000);

        // getWeather();
        delay(5000);

        // if(count_getTime == 500000)
        // {
        //     if(count_getWeather == 100)  //每执行100次getTime，才执行1次getWeather
        //     {
        //         Serial.println("get3daysWeather");
                // get3daysWeather();
        //         count_getWeather = 0;
        //     }
        //     count_getWeather++;    
        //     count_getTime=0;    
        // }
        // count_getTime++;



        vTaskDelay(10);
        esp_task_wdt_reset();       

    }  
}   