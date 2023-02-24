#include "getTime.h"

//提醒事项
extern String timeRemind;
extern String birthdayRemind;      //记录生日提醒的时间
extern String timeHandle;   //格式处理后的时间，可以与提醒事项时间匹配
//显示
char timeHour_display[10];
char timeMinute_display[10];
char timeDate_display[20];
//指令响应
extern int commandNumber; 


String TimeUrl = "http://quan.suning.com/getSysTime.do"; //苏宁提供的API接口

/* 获取北京时间 */
void getTime() 
{
	int httpCode1 = 0;
	HTTPClient http1;
	http1.begin(TimeUrl); //HTTP begin
	httpCode1 = http1.GET(); //进入接口

	DynamicJsonDocument doc_time(512);

	if (httpCode1 > 0)
	{
		// httpCode will be negative on error
		// Serial.printf("HTTP Get Code: %d\r\n", httpCode); //查看httpCode返回值

		if (httpCode1 == HTTP_CODE_OK) // 设置正确的httpCode返回值为200
		{
			String resBuff1 = http1.getString(); //获取返回来的json数据，该数据为字符串 
			String timeHour = resBuff1.substring(24,26); //截取字符串：截取resBuff1第24到26个字符
			String timeMinute = resBuff1.substring(26,29); //截取字符串：截取resBuff1第26到29个字符
			String timeDate = resBuff1.substring(13,24); //
			/* 截取小时的字符 */
			for(int i=0;i<timeHour.length();i++)
			{
				timeHour_display[i]=timeHour[i];
			}
			/* 截取分钟的字符 */
			for(int i=0;i<timeMinute.length();i++)
			{
				timeMinute_display[i]=timeMinute[i];
			}			
			/* 截取日期的字符 */
			for(int i=0;i<timeDate.length();i++)
			{
				timeDate_display[i]=timeDate[i];
			}

			String testMonth1 = resBuff1.substring(19,20);
			String testDay1 = resBuff1.substring(21,23);			
			timeHandle = testMonth1 + "月" + testDay1 + "号。";
			// Serial.println("timeHandle:"+timeHandle); 
			// Serial.printf("timeDate_display:%s",timeDate_display);

			/* 进行日期判断，到期则设置相应的commandSelect数字。 */
			if(timeHandle.compareTo(timeRemind) == 0)  
			{
				commandNumber = 3;  //执行提醒事项
				timeRemind = "清空";

			}
			else if(timeHandle.compareTo(birthdayRemind) == 0)	
			{
				commandNumber = 9;	//生日快乐界面
				birthdayRemind = "清空";
			}		

		}
	}
	else
	{
		Serial.printf("HTTP Get Error: %s\n", http1.errorToString(httpCode1).c_str());
	}

	http1.end();
}

