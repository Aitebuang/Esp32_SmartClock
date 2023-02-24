// #include "getWeather.h"


// char location_text[10] = "abcderf";
// // char weather_text_display[10] = {0};  //显示的天气文字
// long weather_tem = 0;
// char* location_display = location_text;
// const char* weather_text = "weather_text";
// const char* location = "NULL";


// /* URL默认为汕尾 */
// String WeatherUrl = "https://api.seniverse.com/v3/weather/now.json?key=SrbNhwG__nGGuQ8Ri&location=shanwei&language=zh-Hans&unit=c" ; // API接口


// // //	获取天气情况
// void getWeather()
// {




// // 	HTTPClient http2;
// // 	http2.begin(WeatherUrl); //HTTP begin
// // 	int httpCode2 = http2.GET(); //进入接口

// // 	DynamicJsonDocument doc(1024);
// // 	if (httpCode2 > 0)
// // 	{
// // 		// httpCode will be negative on error
// // 		// Serial.printf("HTTP Get Code: %d\r\n", httpCode); //查看httpCode返回值

// // 		if (httpCode2 == HTTP_CODE_OK) // 设置正确的httpCode返回值为200
// // 		{
// // 			String resBuff = http2.getString();

// // 			// 使用ArduinoJson_6.x版本，具体请移步：https://github.com/bblanchon/ArduinoJson			
// // 			deserializeJson(doc, resBuff); //开始使用Json解析
			
// // 			location = doc["results"][0]["location"]["name"]; //获取所在地名字
// // 			strcpy(location_text, location);

// // 			weather_text = doc["results"][0]["now"]["text"];  //获取天气现象描述
			

// // 			weather_tem = doc["results"][0]["now"]["temperature"];   //获取所在地温度

// // 			Serial.printf("天气: %s \r\n", weather_text);   
// // 			Serial.printf("位置: %s \r\n", location);   
// // 			Serial.printf("温度: %ld度 \r\n", weather_tem);  					

// // 		}
// // 	}
// // 	else
// // 	{
// // 		Serial.printf("HTTP Get Error: %s\n", http2.errorToString(httpCode2).c_str());
// // 	}

// // 	http2.end();
// }

