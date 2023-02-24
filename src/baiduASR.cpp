#include "baiduASR.h"
#include "esp_task_wdt.h"

int commandNumber=0;                          //该数字决定特殊指令的选择

/* 指令 */
const char* asr_text = "asr_text";           //记录普通的识别内容
const char* asr_command = "asr_command";     //记录提醒事项的识别内容

const char* commandMainDisplay = "主界面。";  
const char* commandRemind  = "提醒事项。"; 
const char* commandLightOn  = "开灯。"; 
const char* commandLightOff  = "关灯。"; 
const char* commandUI  = "更换壁纸。"; 
const char* commandWeather  = "刷新。"; 
const char* commandBirthday  = "生日时间。"; 
const char* commandLocation1  = "定位广州。"; 
const char* commandLocation2  = "定位深圳。"; 
const char* commandLocation3  = "定位东莞。"; 
const char* commandLocation4  = "定位佛山。"; 


hw_timer_t * timer = NULL;


/* ADC相关 */
static char state_adc=0;        //状态0为空闲，状态1为正在识别，状态2为完成识别，等待按键释放
uint16_t adc_data[data_len];    //16000个数据，8K采样率，即2秒钟，录音时间为2秒钟，想要实现更长时间的语音识别，就要改这个数组大小
                                //和下面data_json数组的大小，改大一些。(办：尝试延长失败，原因数组到达储存上限，
uint8_t adc_start_flag=0;       //开始标志                               可能需要将数组储存在sd卡中，才能进行更长
uint8_t adc_complete_flag=0;    //完成标志                                    时间的语音识别，该识别最长时间限制为1分钟。)

char data_json[45000];  //用于储存json格式的数据,大一点,JSON编码后数据字节数变成原来的4/3,所以得计算好,避免出现越界


extern QueueHandle_t myqueue;  //队列

void baiduASR()
{
// void baiduASR(void *pvParameters)
// {
//     (void) pvParameters;


//     //任务执行while(1)里的内容
//     while(1)
//     {
        // uint32_t time1,time2;  //测量打印花费时间

            HTTPClient http_client;
            DynamicJsonDocument doc_ASR(1024);

            if(digitalRead(KEY)==0) //按键按下
            {           
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


                        // // 打印调试
                        // Serial.printf("adc_data2:");
                        // for(int i=0; i<100; i++){
                        //     Serial.printf("%d", adc_data[i]);  //注意取地址符&
                        // }    


                    memset(data_json,'\0',strlen(data_json));   //将数组清空
                    strcat(data_json,"{");
                    strcat(data_json,"\"format\":\"pcm\",");
                    strcat(data_json,"\"rate\":8000,");         //采样率    如果采样率改变了，记得修改该值，只有16000、8000两个固定采样率
                    strcat(data_json,"\"dev_pid\":1537,");      //中文普通话
                    strcat(data_json,"\"channel\":1,");         //单声道
                    strcat(data_json,"\"cuid\":\"123456\",");   //识别码    随便打几个字符，但最好唯一
                    strcat(data_json,"\"token\":\"25.6eb9026f7003716afbb4f961ef994ac1.315360000.1966642242.282335-25853739\",");  //token	这里需要修改成自己申请到的token，详情见getToken.txt
                    strcat(data_json,"\"len\":32000,");         //数据长度  如果传输的数据长度改变了，记得修改该值，该值是ADC采集的数据字节数，不是base64编码后的长度
                    strcat(data_json,"\"speech\":\"");
                    strcat(data_json,base64::encode((uint8_t *)adc_data,sizeof(adc_data)).c_str());     //base64编码数据
                    strcat(data_json,"\"");
                    strcat(data_json,"}");



                    // 打印调试
                    // Serial.printf("data_json:"); 
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
                            Serial.println(asr_text);
                            // Serial.printf("\r\n"); 
                            asr_command = asr_text; //把识别内容复制一份给提醒事项功能

                                            /* ---特殊指令判断--- */
                                if(asr_command!=NULL) {         //需加入判断，若为空，执行以下语句将导致内核重启
                                    if(strcmp(asr_command, commandMainDisplay) == 0)
                                    {
                                        commandNumber = 1; //主界面    
                                    }    
                                    else if(strcmp(asr_command, commandRemind)== 0)
                                    {
                                        commandNumber = 2; //设置提醒事项
                                    }          
                                    /* commandNumber = 3的判断放在getTime函数里。 */
                                    else if(strcmp(asr_command, commandLightOn)== 0)
                                    {
                                        commandNumber = 4; //打开台灯
                                    }         
                                    else if(strcmp(asr_command, commandLightOff)== 0)
                                    {
                                        commandNumber = 5; //关掉台灯
                                    }        
                                    else if(strcmp(asr_command, commandUI)== 0)
                                    {
                                        commandNumber = 6; //切换壁纸    
                                    }              
                                    else if(strcmp(asr_command, commandWeather)== 0)
                                    {
                                        commandNumber = 7; //刷新  
                                    }         
                                    else if(strcmp(asr_command, commandBirthday)== 0)
                                    {
                                        commandNumber = 8; //生日时间  
                                    }      
                                    /* commandNumber = 9的判断放在getTime函数里。 */
                                    else if(strcmp(asr_command, commandLocation1)== 0)
                                    {
                                        commandNumber = 10; //定位广州
                                    }   
                                    else if(strcmp(asr_command, commandLocation2)== 0)
                                    {
                                        commandNumber = 11; //定位深圳  
                                    }   
                                    else if(strcmp(asr_command, commandLocation3)== 0)
                                    {
                                        commandNumber = 12; //定位东莞
                                    }   
                                    else if(strcmp(asr_command, commandLocation4)== 0)
                                    {
                                        commandNumber = 13; //定位佛山
                                    }   
                                }
                        }
                        else {
                            Serial.printf("[HTTP] GET... failed, error: %s\n", http_client.errorToString(httpCode).c_str());
                        }
                        http_client.end();    

                
                        while (!digitalRead(KEY));
                        Serial.printf("识别完成。\r\n\r\n");
                    }
        //     vTaskDelay(10);
        //     esp_task_wdt_reset();     //看门狗复位
        // } //注释线程时该括号也注释掉
    }
}


//涉及麦克风的ADC转换
uint32_t num=0;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer(){
  // Increment the counter and set the time of ISR
    portENTER_CRITICAL_ISR(&timerMux);    //进入临界区保护
    if(adc_start_flag==1)
    {
        //Serial.println("");
        adc_data[num]=analogRead(ADC);
        num++;
        if(num>=data_len)
        {
            adc_complete_flag=1;
            adc_start_flag=0;
            num=0;
            //Serial.println(Complete_flag);
        }
    }
    portEXIT_CRITICAL_ISR(&timerMux);    //出临界区保护
}
