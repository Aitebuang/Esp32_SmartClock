#include "commandSelect.h"
#include "getTime.h"
#include "getWeather.h"
#include "baiduASR.h"
#include "baiduAsrRemind.h"
#include "get3daysWeather.h"
#include "WiFiUdp.h"
#include "esp_task_wdt.h"
#include "gui.h"

/* 天气switch选择 */
extern int Url_choose;
//显示相关
int UI = 1;
extern lv_obj_t * scr1;
extern lv_obj_t * scr2;
extern lv_obj_t * scr3;
extern lv_obj_t * label_image;
extern lv_obj_t * label_remind;  //提醒事项
//wifi Udp通讯
extern WiFiUDP Udp;             //建立udp对象
extern unsigned int UdpPort;  // 设置本地端口
extern char incomingPacket[10];  // 接受的信息放置在这个字符数组里面
char  replyPacekt_onn[] = "onn";  //回复客户端的信息，可以任意修改。该值为“onn”则开灯，为“off”则关掉灯。
char  replyPacekt_off[] = "off";  //回复客户端的信息，可以任意修改。该值为“onn”则开灯，为“off”则关掉灯。
//提醒事项
char remindEvent[30];
String timeRemind = "timeRemind";      //记录提醒事项的时间
String birthdayRemind = "birthdayRemind";      //记录生日提醒的时间
String timeHandle = "timeHandle";      //格式处理后的时间，可以与提醒事项时间匹配
/* 天气位置 */
const char* weatherLocation = "DG松山湖";
//队列
int reccommandNumber =0;
extern QueueHandle_t myqueue;  

// void commandSelect(void *pvParameters)
// {
void commandSelect()
{

    // (void)pvParameters;  
    

    // while(1) //任务执行while(1)里的内容
    // {     
        // BaseType_t rc = xQueueReceive(myqueue, &reccommandNumber, portMAX_DELAY);  //reccommandNumber
        // if(rc == pdTRUE)
        // {
                switch(commandNumber){
                    case 1: //显示主界面
                            Serial.println("显示主界面。"); 
                            lv_scr_load(scr1); 
                            commandNumber = 0; //指令选择复位
                        break; 
                    case 2: //设置提醒事项
                            /* 函数baiduASR()和baiduAsrRemind()功能几乎是一模一样的，但是后者
                            是不需要按键触发的，这是针对“提醒事项”功能作的修改，因此这里baiduASR不能代替baiduAsrRemind。*/

                            Serial.println("设置提醒事项。"); 
                            delay(1500);        //这里delay用来与前一句语音识别拉开时间距离
                            baiduAsrRemind();   
                            timeRemind = asr_command;    //提醒事项的时间
                            Serial.println(timeRemind);
                            
                            delay(1500);

                            baiduAsrRemind(); 
                            // remindEvent = asr_command;   //提醒事项的内容
                            strcpy(remindEvent,asr_command);
                            Serial.println(remindEvent);	
                            commandNumber = 0;  //指令选择复位

                        break;
                    case 3: //执行提醒事项
                            Serial.println("执行提醒事项。"); 
                            
                            lv_scr_load(scr2); 
                            commandNumber = 0; //指令选择复位
                        break; 
                    case 4: //打开台灯
                            Serial.println("正在打开台灯。"); 
                            if (Udp.parsePacket())//如果有返回值
                            {
                                Udp.read(incomingPacket, 10);  //将读取的数据存放在incomingPacket数组，这一步不能省略，否则会因数据堵塞导致通信失败
                                Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                                Udp.print(replyPacekt_onn);
                                Udp.endPacket();
                                Serial.println("replyPacekt_onn"); 
                                // light = 0;
                            } 
                            commandNumber = 0; 
                        break;
                    case 5: //关掉台灯
                            if (Udp.parsePacket())//如果有返回值
                            {
                                Udp.read(incomingPacket, 255);  //将读取的数据存放在incomingPacket数组，这一步不能省略，否则会因数据堵塞导致通信失败
                                Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                                Udp.print(replyPacekt_off);
                                Udp.endPacket();
                                Serial.println("replyPacekt_off"); 
                            } 
                            commandNumber = 0;
                        break;
                    case 6: //切换壁纸
                            Serial.println("更换壁纸。");  //
                            //-----------------------嵌套switch-------------------------------
                            switch(UI){
                                case 1:
                                    LV_IMG_DECLARE(image11_true_color);    //白色月亮
                                    lv_img_set_src(label_image, &image11_true_color);
                                    UI = UI+1;
                                    break;
                                case 2:
                                    LV_IMG_DECLARE(image2_true_color);   //苹果图标
                                    lv_img_set_src(label_image, &image2_true_color);
                                    UI = UI+1;
                                    break;
                                case 3:
                                    LV_IMG_DECLARE(image10_true_color);   //微信图标
                                    lv_img_set_src(label_image, &image10_true_color);
                                    UI = UI+1;
                                    break;
                                case 4:
                                    LV_IMG_DECLARE(image1_true_color);   //小熊头像
                                    lv_img_set_src(label_image, &image1_true_color);
                                    UI = UI+1;
                                    break;
                                case 5:
                                    LV_IMG_DECLARE(image3_true_color);    //太空人
                                    lv_img_set_src(label_image, &image3_true_color);
                                    UI = 1;
                                    break;
                                // case 6:
                                //     LV_IMG_DECLARE(Birthday_true_color);    //生日图
                                //     lv_img_set_src(label_image, &Birthday_true_color);
                                //     break;
                                // case 7:
                                    // LV_IMG_DECLARE(image3_true_color);    //太空人
                                    // lv_img_set_src(label_image, &image3_true_color);
                                //     UI = UI+1;
                                //     break;
                                // case 8:
                                //     LV_IMG_DECLARE(image8_true_color);    
                                //     lv_img_set_src(label_image, &image8_true_color);
                                //     UI = UI+1;
                                //     break;
                                // case 9:
                                //     LV_IMG_DECLARE(image9_true_color);    
                                //     lv_img_set_src(label_image, &image9_true_color);
                                //     UI = 1;
                                //     break;
                                default:
                                    break;
                            }
                            lv_scr_load(scr3); 
                            commandNumber = 0; //指令选择复位
                        break;  
                        //-------------------------嵌套switch结束------------------------------
                    case 7: //刷新天气
                            Serial.println("刷新。"); 
                            // getTime();
                            get3daysWeather();
                            commandNumber = 0; //指令选择复位
                        break;      
                    case 8: //获取生日时间

                            Serial.println("生日提醒。"); 
                            delay(1500);        //这里delay用来与前一句语音识别拉开时间距离
                            baiduAsrRemind();   
                            birthdayRemind = asr_command;    //提醒事项的时间
                            Serial.println(birthdayRemind);
                            
                            commandNumber = 0; //指令选择复位
                        break;      
                    case 9: //生日快乐
                            Serial.println("生日快乐。"); 
                            LV_IMG_DECLARE(Birthday_true_color);    
                            lv_img_set_src(label_image, &Birthday_true_color);
                            lv_scr_load(scr3); 
                            commandNumber = 0; //指令选择复位
                        break;  
                    case 10: //定位广州
                            Serial.println("定位广州"); 
                            Url_choose = 1;    
                            weatherLocation = "广州";
                            get3daysWeather();
                            commandNumber = 0; //指令选择复位
                        break;  
                    case 11: //定位深圳
                            Serial.println("定位深圳"); 
                            Url_choose = 2;   
                            weatherLocation = "SZ南山";
                            get3daysWeather();
                            commandNumber = 0; //指令选择复位
                        break;  
                    case 12: //定位东莞
                            Serial.println("定位东莞"); 
                            Url_choose = 3;   
                            weatherLocation = "DG松山湖";
                            get3daysWeather();
                            commandNumber = 0; //指令选择复位
                        break;  
                    case 13: //定位佛山
                            Serial.println("定位佛山"); 
                            Url_choose = 4;   
                            weatherLocation = "佛山";
                            get3daysWeather();
                            commandNumber = 0; //指令选择复位
                        break;  
                           
                    default: 
                        break;
                        // Serial.println("无特殊指令"); 
                }
            // }    
            // vTaskDelay(10);
            // esp_task_wdt_reset();       

    // }  
}   



