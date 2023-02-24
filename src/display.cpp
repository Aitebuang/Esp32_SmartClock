#include <display.h>
#include "esp_task_wdt.h"
#include "gui.h"
static void lv_port_disp_init();
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void IRAM_ATTR LV_TIMER_INT();

hw_timer_t * LV_timer;

/*
TFT引脚定义在/TFT_eSPI/User_Setups/Setup24_ST7789.h
TFT亮度引脚定义在display.h
*/
TFT_eSPI tft = TFT_eSPI();

//屏幕刷新指令任务
void Display::routine(void *pvParameters)
{
    (void) pvParameters;

    //任务执行while(1)里的内容
    while(1)
    {
    	lv_task_handler();
        vTaskDelay(10);
        esp_task_wdt_reset();
    }

}

void Display::init()
{
    //背光亮度
	ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
	ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL);
    setBackLight(0.5);

	tft.begin(); /* TFT init */
	tft.setRotation(4); /* mirror */

    /* 镜像显示 */
    tft.writecommand (0x36);
    tft.writedata(0x40);

	lv_init();              //LVGL内部初始化
	lv_port_disp_init();    //LVGL显示接口初始化

    //LVGL定时器设置
    LV_timer = timerBegin(1, 80, true);    //  80M的时钟 80分频 1M
    timerAlarmWrite(LV_timer, 1000, true);  //  1M  计1000个数进中断  8K
    timerAttachInterrupt(LV_timer, &LV_TIMER_INT, true);
    timerAlarmEnable(LV_timer);
}

//屏幕亮度调节函数
void Display::setBackLight(float duty)
{
	 duty = constrain(duty, 0, 1);
	 duty = 1 - duty;
	ledcWrite(LCD_BL_PWM_CHANNEL, (int)(duty * 255));
}

//内部函数：LVGL显示接口初始化函数
static void lv_port_disp_init()
{
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * BUFFERROWS];                          /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * BUFFERROWS);   /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

//内部函数：LVGL显示填充函数
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors(&color_p->full, w * h, true);
	tft.endWrite();
    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}


portMUX_TYPE timerMux_1 = portMUX_INITIALIZER_UNLOCKED;
//LV_Timer定时器中断函数：告诉lvgl时间
void IRAM_ATTR LV_TIMER_INT(){
    portENTER_CRITICAL_ISR(&timerMux_1);    //进入临界区保护
    lv_tick_inc(1);
    portEXIT_CRITICAL_ISR(&timerMux_1);    //出临界区保护
}
