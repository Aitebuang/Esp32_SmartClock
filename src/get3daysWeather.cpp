#include "get3daysWeather.h"

int Url_choose = 1;
const char* weather_text_day1 = NULL;
const char* sunrise = NULL;
const char* sunset = NULL;
char weather_text_display1[20] = {0};  //显示的天气文字
char sunrise_time[10] = {0};  //日出时间
char sunset_time[10] = {0};  //日落时间
long weather_tem_max_day1 = 0;
long weather_tem_min_day1 = 0;
long weather_tem_day1 = 0;
long humidity = 0;

// 和风天气3天api,默认为东莞松山湖
// String WeatherUrl2 = "https://devapi.qweather.com/v7/weather/3d?location=113.88,22.92&key=48359829a4c24d72bbd258e33dcbc862&gzip=n"; // API接口
String WeatherUrl2 = "https://devapi.qweather.com/v7/weather/3d?location=113.88,22.89&key=48359829a4c24d72bbd258e33dcbc862&gzip=n"; // API接口

//获取未来三天天气情况
void get3daysWeather()
{
	switch(Url_choose){
        case 1:
            /* 广州 */
            WeatherUrl2 = "https://devapi.qweather.com/v7/weather/3d?location=113.27,23.15&key=48359829a4c24d72bbd258e33dcbc862&gzip=n";
            break;
        case 2:
            /* 深圳 */
            WeatherUrl2 = "https://devapi.qweather.com/v7/weather/3d?location=113.88,22.55&key=48359829a4c24d72bbd258e33dcbc862&gzip=n";			
            break;
        case 3:
            /* DG松山湖 */
			WeatherUrl2 = "https://devapi.qweather.com/v7/weather/3d?location=113.88,22.89&key=48359829a4c24d72bbd258e33dcbc862&gzip=n"; // API接口
            break;
        case 4:
            /* 佛山 */
            WeatherUrl2 = "https://devapi.qweather.com/v7/weather/3d?location=112.89,22.90&key=48359829a4c24d72bbd258e33dcbc862&gzip=n";			
            break;
        default:
            break;        
    }

	int httpCode_weather = 0;
	HTTPClient http3;
	http3.begin(WeatherUrl2); //HTTP begin
	httpCode_weather = http3.GET(); //进入接口

    // DynamicJsonDocument doc_weather(2048);
	StaticJsonDocument<2048> doc_weather;
	if (httpCode_weather > 0)
	{
		// httpCode_weather will be negative on error
		// Serial.printf("HTTP Get Code: %d\r\n", httpCode_weather); //httpCode_weather

		if (httpCode_weather == HTTP_CODE_OK) // 设置正确的httpCode返回值为200
		{
			String resBuff3 = http3.getString();
			// Serial.println(resBuff3);

				// 使用ArduinoJson_6.x版本，具体请移步：https://github.com/bblanchon/ArduinoJson			
			deserializeJson(doc_weather, resBuff3); //开始使用Json解析	

			/*------------------------Day1------------------------------*/ 	
			weather_text_day1 = doc_weather["daily"][0]["textDay"];  //获取天气现象描述
			strcpy(weather_text_display1, weather_text_day1); 
			sunrise = doc_weather["daily"][0]["sunrise"];  //获取日出时间
			strcpy(sunrise_time, sunrise); 
			sunset  = doc_weather["daily"][0]["sunset"];  //获取日落时间
			strcpy(sunset_time, sunset); 
			weather_tem_max_day1 = doc_weather["daily"][0]["tempMax"];   //获取最高温度
			weather_tem_min_day1 = doc_weather["daily"][0]["tempMin"];   //获取最低温度
			humidity = doc_weather["daily"][0]["humidity"]; 
			weather_tem_day1 = (weather_tem_max_day1+weather_tem_min_day1)/2; //平均温度



			// Serial.printf("东莞今天天气：\r\n");
			// Serial.printf("天气概况: %s \r\n", weather_text_day1);  
			// Serial.printf("平均温度: %ld\r\n", weather_tem_day1);  
			// Serial.printf("日出时间: %s \r\n", sunrise);  		
			// Serial.printf("日落时间: %s \r\n", sunset);  	
								
		}
	}
	else
	{
		Serial.printf("HTTP Get Error: %s\n", http3.errorToString(httpCode_weather).c_str());
	}

	http3.end();  
}



			/*------------------------Day2------------------------------*/
            // Serial.printf("东莞明天天气：\r\n");

			// const char* weather_text_day2 = doc["daily"][1]["textDay"];  //获取天气现象描述
			// Serial.printf("天气概况: %s \r\n", weather_text_day2);   

			// long weather_tem_max_day2 = doc["daily"][1]["tempMax"];   //获取最高温度
			// Serial.printf("最高温度: %ld度 \r\n", weather_tem_max_day2);  

			// long weather_tem_min_day2 = doc["daily"][1]["tempMin"];   //获取最低温度
			// Serial.printf("最低温度: %ld度 \r\n", weather_tem_min_day2);  	

			/*------------------------Day3------------------------------*/
            // Serial.printf("东莞后天天气：\r\n");

			// const char* weather_text_day3 = doc["daily"][2]["textDay"];  //获取天气现象描述
			// Serial.printf("天气概况: %s \r\n", weather_text_day3);   

			// long weather_tem_max_day3 = doc["daily"][2]["tempMax"];   //获取最高温度
			// Serial.printf("最高温度: %ld度 \r\n", weather_tem_max_day3);  

			// long weather_tem_min_day3 = doc["daily"][2]["tempMin"];   //获取最低温度
			// Serial.printf("最低温度: %ld度 \r\n", weather_tem_min_day3);  	