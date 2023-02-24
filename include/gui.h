#ifndef __GUI_H
#define __GUI_H

#include "lvgl.h"
#include "lv_font.h"
#include "include.h"

void gui_setup();

void label_Minute_event_cb(lv_event_t * e); //label_Minute回调函数
// void label_remind_event_cb(lv_event_t * a);

// #define LV_FONT_MONTSERRAT_30 1


#endif/*__GUI_H*/