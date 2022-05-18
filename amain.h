#ifndef __MAIN_H__
#define __MAIN_H__

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <string.h>
#include <WiFiManager.h>


WiFiManager wm;                       //配网申明
String cityCode = "101190201";    

struct Config_Set                     //配置信息
{
  uint16_t store;                     //数据有效标志位，0x55AA说明写入数据了
  char ssid[32];                      //路由器的账号和密码
  char pass[64];
  char city[15];                      //城市代码
};
Config_Set Set_Pra;
bool Read_Pra_Flag = false;           //参数读取状态

int Led = 2;                          //LED输出，高电平亮
int KEY1 = 27;                        //GPIO为KEY输入，低电平有效

TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;        //定义缓存区
static lv_color_t buf[LV_HOR_RES_MAX * 10];

static lv_style_t style_cn;           //定义新的样式,请注意使用 static 或者全局变量
static lv_style_t style_led;          //LED字体样式
static lv_style_t style1; 
static lv_style_t style2; 
static lv_style_t style3; 

LV_FONT_DECLARE(myFont1)              //汉字的字体申明，微软雅黑
LV_FONT_DECLARE(myFontLed)            //数码管字体
LV_IMG_DECLARE(bing)                  //冰墩墩启动界面
LV_IMG_DECLARE(bing01)                //冰墩墩动画
LV_IMG_DECLARE(bing02) 
LV_IMG_DECLARE(bing03) 
LV_IMG_DECLARE(bing04) 
LV_IMG_DECLARE(bing05) 
LV_IMG_DECLARE(cwatch_bg);            //表盘背景
LV_IMG_DECLARE(chour);                //表盘指针
LV_IMG_DECLARE(cminute);
LV_IMG_DECLARE(csecond);
LV_IMG_DECLARE(d00)                   //天气
LV_IMG_DECLARE(d01)
LV_IMG_DECLARE(d02)
LV_IMG_DECLARE(d03)
LV_IMG_DECLARE(d04)
LV_IMG_DECLARE(d05)
LV_IMG_DECLARE(d06)
LV_IMG_DECLARE(d07)
LV_IMG_DECLARE(d09)
LV_IMG_DECLARE(d11)
LV_IMG_DECLARE(d13)
LV_IMG_DECLARE(d14)
LV_IMG_DECLARE(d15)
LV_IMG_DECLARE(d16)
LV_IMG_DECLARE(d17)
LV_IMG_DECLARE(d18)
LV_IMG_DECLARE(d19)
LV_IMG_DECLARE(d20)
LV_IMG_DECLARE(d29)
LV_IMG_DECLARE(d30)
LV_IMG_DECLARE(d31)
LV_IMG_DECLARE(d53)

lv_obj_t * img1;                      //图像对象，启动界面
lv_obj_t * img2;                      //图像对象，动画
uint8_t img_num = 0;                  //显示图像号，动画用
lv_obj_t * wimg1;                     //图像对象，天气
lv_obj_t * Watch_Img;                 //图像，表盘
lv_obj_t * Hour_Img;                  //图像，表针
lv_obj_t * Minute_Img;
lv_obj_t * Second_Img;

const char * day_names[7] = {"日", "一",  "二", "三", "四", "五", "六"};//日历的周和月汉字显示
const char * month_names[12] = {"一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"};
lv_obj_t *scr;                        //顶级父对象
lv_obj_t *label1;                     //联网文本显示
lv_obj_t *label2;                     //联网文本显示
lv_obj_t *labelTime;                  //时间显示
lv_obj_t *labelDay;                   //日期显示
lv_obj_t *bar1;                       //进度条
uint8_t bar1_value = 0;
lv_obj_t * calendar1;                 //日历
lv_calendar_date_t today_date;        //日历日期结构体申明

#define LV_COLOR_LONON LV_COLOR_MAKE(28, 152, 186)
#define COLOR_BAR lv_color_make(86, 94, 102)
#define COLOR_THEME lv_color_make(252, 199, 0)
#define COLOR_DEEP lv_color_make(246, 174, 61)
#define COLOR_TEXT lv_color_make(56, 56, 56)
#define COLOR_BG lv_color_make(238, 241, 245)
#define COLOR_GREEN lv_color_make(60, 179, 113)

String Data_Dis = "";
String Weather_Text[7];               //天气信息存储
bool Key_State = false;               //按键状态，true为按键按下
uint8_t Key_Buff[2];                  //按键值
uint16_t Key_Down_Time = 0;           //按键次数
uint8_t Page_Id = 0;                  //页面号

//---------------------NTP相关参数---------------------
static const char ntpServerName[] = "ntp1.aliyun.com";      //NTP服务器
const int timeZone = 8;               //时区，东八区为北京时间
WiFiUDP Udp;
unsigned int localPort = 8888;        //连接时间服务器的本地端口号
time_t prevDisplay = 0;               //上一次获取到的时间
const int NTP_PACKET_SIZE = 48;       //NTP发送数据包长度
byte packetBuffer[NTP_PACKET_SIZE];   //NTP数据包缓冲区

//---------------------Time 相关参数---------------------
bool Led_State = false;               //LED状态，1亮0暗
int Led_Flag = HIGH;                  //默认当前灭灯
unsigned long now0;                   //用于定时闪灯
unsigned long LastTime0 = 0;
unsigned long now1;                   //用于定时切换图片
unsigned long LastTime1 = 0;
unsigned long now2;                   //用于定时获取天气
unsigned long LastTime2 = 0;
unsigned long now3;                   //用于按键扫描
unsigned long LastTime3 = 0;

typedef struct
{
  uint8_t Month;                      //RTC Date Month (in BCD format).0x01-0x12
  uint8_t Date;                       //RTC Date.1-31
  uint16_t Year;                      //RTC Date Year.2000-2099    
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;
  uint8_t Week;
}RTC_DateTypeDef;                     //时间结构体

uint8_t PowerOn_Flag = 0;             //上电标志位
RTC_DateTypeDef Time;                 //本次读到的时间
RTC_DateTypeDef Last_Time;            //上次读到的时间

//---------------------WEB 相关参数---------------------
const char* host1 = "esp32";
WebServer server(80);                   //WEB服务器使用80端口


#endif
