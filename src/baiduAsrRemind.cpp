#include "baiduAsrRemind.h"

//baiduASR是需要按键触发的，而baiduAsrRemind只要被调用就触发语音识别

extern hw_timer_t * timer;
extern uint16_t adc_data[data_len];    //16000个数据，8K采样率，即2秒钟，录音时间为2秒钟，想要实现更长时间的语音识别，就要改这个数组大小
                                //和下面data_json数组的大小，改大一些。
extern uint8_t adc_start_flag;       //开始标志
extern uint8_t adc_complete_flag;    //完成标志 
extern char data_json[45000];  //用于储存json格式的数据,大一点,JSON编码后数据字节数变成原来的4/3,所以得计算好,避免出现越界

/* 涉及麦克风的ADC转换 */
extern uint32_t num;
extern portMUX_TYPE timerMux;
extern void IRAM_ATTR onTimer();

void baiduAsrRemind()
{

    // uint32_t time1,time2;  //测量打印花费时间

    HTTPClient http_client;
    DynamicJsonDocument doc_ASR(1024);

    // if(digitalRead(key)==0) //按键按下
    // {
        Serial.printf("开始识别···\r\n\r\n");
        digitalWrite(LED,HIGH);
        adc_start_flag=1;
        timerStart(timer);

        // time1=micros();
        while(!adc_complete_flag)  //等待采集完成
        {
            ets_delay_us(10);
        }
        // time2=micros()-time1;

        timerStop(timer);
        adc_complete_flag=0;        //清标志

        digitalWrite(LED,LOW);

        // Serial.printf("time:%d\r\n",time2);  //打印花费时间
        
        
        memset(data_json,'\0',strlen(data_json));   //将数组清空
        strcat(data_json,"{");
        strcat(data_json,"\"format\":\"pcm\",");
        strcat(data_json,"\"rate\":8000,");         //采样率    如果采样率改变了，记得修改该值，只有16000、8000两个固定采样率
        strcat(data_json,"\"dev_pid\":1537,");      //中文普通话
        strcat(data_json,"\"channel\":1,");         //单声道
        strcat(data_json,"\"cuid\":\"123456\",");   //识别码    随便打几个字符，但最好唯一
        strcat(data_json,"\"token\":\"25.6eb9026f7003716afbb4f961ef994ac1.315360000.1966642242.282335-25853739\",");  //token	这里需要修改成自己申请到的token
        strcat(data_json,"\"len\":32000,");         //数据长度  如果传输的数据长度改变了，记得修改该值，该值是ADC采集的数据字节数，不是base64编码后的长度
        strcat(data_json,"\"speech\":\"");
        strcat(data_json,base64::encode((uint8_t *)adc_data,sizeof(adc_data)).c_str());     //base64编码数据
        strcat(data_json,"\"");
        strcat(data_json,"}");
        // Serial.println(data_json);
        

        int httpCode;
        http_client.begin("http://vop.baidu.com/server_api");
        http_client.addHeader("Content-Type","application/json");
        httpCode = http_client.POST(data_json);

        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                String payload = http_client.getString();
                // Serial.println(payload);

                deserializeJson(doc_ASR, payload); //开始使用Json解析

                asr_text = doc_ASR["result"][0]; //获取识别内容
                // Serial.println(asr_text);
                // Serial.printf("识别内容: %s \r\n", asr_text);      
                
                asr_command = asr_text; //提醒事项内容由asr_command传递，
                                        //理论上来说由asr_text直接传递也是一样的，
                                        //但是如果asr_text经常被调来调去，程序容易出错。


            }
        }
        else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http_client.errorToString(httpCode).c_str());
        }
        http_client.end();    

        
        while (!digitalRead(KEY));
        Serial.printf("识别完成。\r\n");
    // }

}







