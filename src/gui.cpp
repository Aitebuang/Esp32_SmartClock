#include "gui.h"
#include "getTime.h"
#include "getWeather.h"
#include "get3daysWeather.h"

// extern const char* remindEvent;
extern char remindEvent[30];  //提醒事项内容
extern char timeHour_display[10];
extern char timeMinute_display[10];
extern char timeDate_display[20];
extern char weather_text_display1[20];
extern char sunrise_time[10];  //日出时间
extern char sunset_time[10];  //日落时间
extern long weather_tem_day1;
extern long humidity;  //湿度

/* 天气位置 */
extern const char* weatherLocation;
/* 屏幕部件 */
lv_obj_t * label_Hour;
lv_obj_t * label_Minute;
lv_obj_t * label_Date;
lv_obj_t * label_weather; 
lv_obj_t * label_humidity;
lv_obj_t * label_sunrise_set;

lv_obj_t * label_face;
// lv_obj_t * label_image1;

lv_obj_t * label_image;
lv_obj_t * label_remind;
//创建3个屏幕
lv_obj_t * scr1;
lv_obj_t * scr2;
lv_obj_t * scr3;



void gui_setup()
{   
    scr1 = lv_obj_create(NULL);
    scr2 = lv_obj_create(NULL);
    scr3 = lv_obj_create(NULL);

    //首先获取一遍时间和天气，也可以在这个函数前调用
    // getTime();
    get3daysWeather();

/****************************************************************************************************************************/
    /*屏幕1*/

//-------------------------------------------------------------------------------------------------------------
    /* 创建label部件(对象) */
    label_Hour = lv_label_create(scr1); //小时
    lv_obj_set_pos(label_Hour, 45, 58);          //文本位置
    // lv_label_set_long_mode(label_Hour, LV_LABEL_LONG_SCROLL);                //滚动模式
    // lv_obj_set_size(label_Hour, 180, 60);                                    //标签框大小  
    // lv_obj_set_style_text_color(label_Hour, lv_color_hex(0xFA0000), 0);  //文本颜色：红色
    LV_FONT_DECLARE(lv_font_math_heavy_Size70); //设置字体    
    lv_obj_set_style_text_font(label_Hour, &lv_font_math_heavy_Size70, 0);
    lv_label_set_recolor(label_Hour, true);
    // lv_label_set_text_fmt(label_Hour, "#0xFF8C00 %s# #0x000000 %s#", timeHour_display); 
    lv_label_set_text_fmt(label_Hour, "#000000 %s#", timeHour_display); 

    // lv_label_set_recolor(label_Hour, true);
    // lv_label_set_text(label_Hour, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label");
//-------------------------------------------------------------------------------------------------------------                                    
    label_Minute = lv_label_create(scr1);   //分钟
    lv_obj_set_pos(label_Minute, 135, 58);          //文本位置
    lv_obj_set_style_text_color(label_Minute, lv_color_hex(0xFF8C00), 0);  //文本颜色:橘色
    LV_FONT_DECLARE(lv_font_math_heavy_Size70); //设置字体    
    lv_obj_set_style_text_font(label_Minute, &lv_font_math_heavy_Size70, 0);
    lv_label_set_recolor(label_Hour, true);
    lv_label_set_text_fmt(label_Minute, "%s", timeMinute_display); 
//-------------------------------------------------------------------------------------------------------------
    label_weather = lv_label_create(scr1);   //位置和天气
    // lv_obj_add_style(label_weather,&style,0);           //将样式添加到文字对象中
    // lv_obj_set_pos(label_weather, 22, 14);                     //显示位置
    // lv_label_set_long_mode(label_weather, LV_LABEL_LONG_SCROLL);                //滚动模式
    // lv_obj_set_size(label_weather, 200, 40);                                    //标签框大小        
    // lv_obj_set_style_text_color(label_weather, lv_color_hex(0x000000), 0);  //文本颜色  
    lv_obj_align(label_weather, LV_ALIGN_CENTER, 0, -90);      //设置中心点并对齐
    LV_FONT_DECLARE(lv_font_SYRHDKB_Heavy_size25);              //设置字体
    lv_obj_set_style_text_font(label_weather, &lv_font_SYRHDKB_Heavy_size25, 0); 
    lv_label_set_recolor(label_weather, true);    
    lv_label_set_text_fmt(label_weather, "%s-#FF8C00 %s#",weatherLocation,weather_text_display1);
//-------------------------------------------------------------------------------------------------------------
    label_Date = lv_label_create(scr1);           //日期
    lv_obj_set_pos(label_Date, 78, 130);                                      //文本位置
    // lv_obj_set_style_text_color(label_Date, lv_color_hex(0x000000), 0);
    LV_FONT_DECLARE(lv_font_montserrat_14);                             //设置字体
    lv_obj_set_style_text_font(label_Date, &lv_font_montserrat_14, 0);    
    lv_label_set_recolor(label_Date, true);                                 //使能颜色修改
    lv_label_set_text_fmt(label_Date, "#000000 %s#", timeDate_display); 
//-------------------------------------------------------------------------------------------------------------
    label_humidity = lv_label_create(scr1);   //温湿度
    lv_obj_set_pos(label_humidity, 10, 160);                     //显示位置 
    // lv_obj_set_style_text_color(label_weather, lv_color_hex(0x000000), 0);  //文本颜色    
    LV_FONT_DECLARE(lv_font_SYRHDKB_Heavy_size20);              //设置字体
    lv_label_set_recolor(label_humidity, true);   
    lv_obj_set_style_text_font(label_humidity, &lv_font_SYRHDKB_Heavy_size20, 0); 
    lv_label_set_text_fmt(label_humidity, "#000000 温度T:%ld#\n\n#000000 湿度H:%ld#", weather_tem_day1, humidity);    
//-------------------------------------------------------------------------------------------------------------
    label_sunrise_set = lv_label_create(scr1);   //日出日落时间
    lv_obj_set_pos(label_sunrise_set, 120, 160);                     //显示位置 
    // lv_obj_set_style_text_color(label_sunrise_set, lv_color_hex(0x000000), 0);  //文本颜色    
    LV_FONT_DECLARE(lv_font_SYRHDKB_Heavy_size20);              //设置字体
    lv_label_set_recolor(label_sunrise_set, true);   
    lv_obj_set_style_text_font(label_sunrise_set, &lv_font_SYRHDKB_Heavy_size20, 0); 
    lv_label_set_text_fmt(label_sunrise_set, "#FF8C00 日出R:%s#\n\n#FF8C00 日落S:%s#", sunrise_time, sunset_time);       
//-------------------------------------------------------------------------------------------------------------



//jpg壁纸------------------------------------------------------------------------------------------------------
//模板：image1_true_color （将数字改为1-9分别对应九张壁纸）
    // label_image1 = lv_img_create(lv_scr_act());
    

    // lv_img_set_src(label_image1, &image4_true_color);


    /*
    片段1：
    刷新label_Minute示例：这里的片段1+下面的片段2函数即可完成刷新的注册，在外部调用lv_event_send(label_Minute,LV_EVENT_REFRESH,NULL);即可刷新
    更多事件参考http://lvgl.100ask.org/8.2/overview/event.html
    此处片段1为注册label_Minute刷新事件回调函数
    */

    lv_obj_add_event_cb(label_Minute, label_Minute_event_cb, LV_EVENT_REFRESH, NULL);
    // lv_obj_add_event_cb(label_remind, label_remind_event_cb, LV_EVENT_REFRESH, NULL);        

/****************************************************************************************************************************/
                                    /*屏幕2：提醒事项*/
    label_remind = lv_label_create(scr2);
    lv_obj_align(label_remind, LV_ALIGN_CENTER, 0, 0);      //设置中心点并对齐
    LV_FONT_DECLARE(lv_font_SYRHDKB_Heavy_size25);              //设置字体
    lv_obj_set_style_text_font(label_remind, &lv_font_SYRHDKB_Heavy_size25, 0); 
    lv_label_set_text_fmt(label_remind, "提醒:\n\n%s",remindEvent);

/****************************************************************************************************************************/
                                    /*屏幕3：壁纸*/
    label_image = lv_img_create(scr3);
    // LV_IMG_DECLARE(image1_true_color);    
    // LV_IMG_DECLARE(image2_true_color);    
    // LV_IMG_DECLARE(image3_true_color);    
    // LV_IMG_DECLARE(image4_true_color);    
    // LV_IMG_DECLARE(image5_true_color);    
    // LV_IMG_DECLARE(image6_true_color);    
    // LV_IMG_DECLARE(image7_true_color);    
    // lv_img_set_src(label_image, &image4_true_color);

/****************************************************************************************************************************/
    lv_scr_load(scr1);    //初始化后第一次先进入屏幕1
}
  
/*
片段2：
刷新label_Minute示例：这里的片段1+下面的片段2函数即可完成刷新的注册，在外部调用lv_event_send(label_Minute,LV_EVENT_REFRESH,NULL);即可刷新
此处片段2为回调函数
*/
void label_Minute_event_cb(lv_event_t * e)
{
    lv_label_set_text_fmt(label_Hour, "#000000 %s#", timeHour_display); 
    lv_label_set_text_fmt(label_Minute, "%s", timeMinute_display); 
    lv_label_set_text_fmt(label_Date, "#000000 %s#", timeDate_display); 
    lv_label_set_text_fmt(label_weather, "%s-#FF8C00 %s#",weatherLocation,weather_text_display1);
    // lv_label_set_text_fmt(label_humidity, "#FF8C00 温度T:%ld#\n\n#007ACC 湿度H:%ld#", weather_tem_day1, humidity);  
    lv_label_set_text_fmt(label_humidity, "#000000 温度T:%ld#\n\n#000000 湿度H:%ld#", weather_tem_day1, humidity);   
    // lv_label_set_text_fmt(label_sunrise_set, "日出R:%s\n\n日落S:%s", sunrise_time, sunset_time);      
    lv_label_set_text_fmt(label_sunrise_set, "#FF8C00 日出R:%s#\n\n#FF8C00 日落S:%s#", sunrise_time, sunset_time);    
    lv_label_set_text_fmt(label_remind, "提醒:\n\n%s",remindEvent);
}

// void label_remind_event_cb(lv_event_t * a)
// {
//     lv_label_set_text_fmt(label_remind, "提醒事项:\n\n%s",remindEvent); 
// }
