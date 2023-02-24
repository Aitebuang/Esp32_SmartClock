#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <lvgl.h>
#include "display.h"
#include <TFT_eSPI.h>

/*#define LCD_BL_PIN 4*/
#define LCD_BL_PIN 4		//ESP32S
#define LCD_BL_PWM_CHANNEL 4

#define MY_DISP_HOR_RES 240
#define MY_DISP_VER_RES 240

#define BUFFERROWS 2   //显示缓存大小(MY_DISP_HOR_RES*BUFFERROWS)

extern TFT_eSPI tft;

class Display
{
private:


public:
	static void routine(void *pvParameters);
	void init();
	void setBackLight(float);
};

#endif /*__DISPLAY_H*/
