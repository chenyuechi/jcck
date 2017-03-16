/*
  文件名称：smartconfig.ino
  功能：ESP8266快速配置功能,自动保存配置参数
        1.上电时，检测ssid是否有配置，无配置自动进入smartconfig配置，有配置按配置连接网络
        2.进入smartconfig配置后，3分钟没有进行smartconfig配置，自动转换模式为AP 模式，进入httpServer模式配置ssid和psw
        3.进入工作后模式后，短按按键切换控制输出状态，长按恢复出厂设置
        4.每秒钟从时间服务器上读取时间
  作者：张河志
  日期：2016-11-29
  版本：1.0
  
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Ticker.h>
#include "project.h"
#include <time.h>
#include <IRremoteESP8266.h>
IRsend irsend(14); 
#define MAGIC_NUMBER 0xAA

//一下宏定义 使用于nodemcu板
//#ifdef NODEMCU
//#define D0  16
//#define D1  5
//#define D2  4
//#define D3  0
//#define D4  2
//#define D5  14
//#define D6  12
//#define D7  13
//#endif


extern WiFiClient client;

char outsta;
char default_id[32];
char keyid[30];
unsigned long RTPre;
bool ReadTimeEn;

void Set_ID(void)
{
  uint8_t mac[6];
  WiFi.macAddress(mac);       //读取mac地址
  sprintf(default_id, "%s_%02X%02X%02X", DEFAULT_ID, mac[3], mac[4], mac[5]);
  
}
/*
 * 保存参数到EEPROM
*/
void saveConfig()
{
  Serial.println("Save config!");
  Serial.print("stassid:");
  Serial.println(config.stassid);
  Serial.print("stapsw:");
  Serial.println(config.stapsw);
  Serial.print("id:");
  Serial.println(config.id);
  Serial.print("key:");
  Serial.println(config.key);

  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
}

/*
 * 从EEPROM加载参数
*/
void loadConfig()
{
  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    *(p + i) = EEPROM.read(i);
  }
  EEPROM.commit();
  if (config.magic != MAGIC_NUMBER)         //判断参数存储区是否已经格式化
  {
    Set_ID();
    strcpy(config.stassid, DEFAULT_STASSID);//ssid 空
    strcpy(config.stapsw, DEFAULT_STAPSW);  //psw  空
    strcpy(config.id, default_id);//DEFAULT_ID);
    strcpy(config.key, DEFAULT_KEY);
    config.magic = MAGIC_NUMBER;            //参数存储区已经格式化标志
    saveConfig();                           //保存参数
   // system_restore();
    Serial.println("Restore config!");
  }
  Serial.println("-----Read config-----");
  Serial.print("stassid:");
  Serial.println(config.stassid);
  Serial.print("stapsw:");
  Serial.println(config.stapsw);
  Serial.print("id:");
  Serial.println(config.id);
  Serial.print("key:");
  Serial.println(config.key);
  Serial.println("-------------------");
}

/*
 * 开启smartconfig，等待配置完成，若超时，延时重启
*/
void doSmartconfig()
{
  Serial.println("[doSmartconfig]");
  WiFi.mode(WIFI_STA);
  WiFi.stopSmartConfig();
  WiFi.beginSmartConfig();
  int cnt = 0;
  bool flag_ok = false;
  while (true) 
  {
    ESP.wdtFeed();
    delay(300);
    ledState = 1-ledState; 
    digitalWrite(LED, ledState);
    if (flag_ok == true)
      continue;
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      /*Serial.println("SSID: "+WiFi.SSID());
        Serial.println("PSW: "+WiFi.psk());*/
      strcpy(config.stassid, WiFi.SSID().c_str());    //复制ssid到参数
      strcpy(config.stapsw, WiFi.psk().c_str());      //复制psw到参数
      Serial.printf("SSID:%s\r\n", config.stassid);   //打印ssid
      Serial.printf("PSW:%s\r\n", config.stapsw);     //打印PSW
      saveConfig();                                    //保存参数
      delayRestart(10);                                //延时启动
      flag_ok = true;                                  //smartconfig配置完成标志
    }
    cnt++;
    if (cnt >= 600)                                    //判断配置是否超时 3分钟配置不成功，重启
    {
      Serial.println("Time out!");
      //delayRestart(1);
      WiFi.stopSmartConfig();
      return;
    }
  }
}

/* 
 * 恢复出厂设置
*/
void restoreFactory()
{
  Serial.println("[restoreFactory]");
  config.magic = 0x00;
  saveConfig();
  delayRestart(1);
  while (1) 
  {
    ESP.wdtFeed();
    delay(100);
  }
}
/*
void wenshidu()
{
char wsdtemp[64];
int temp;//温度
int humi;//湿度
int tol;//校对码
int j;
unsigned int loopCnt;
int chr[40] = {0};//创建数字数组，用来存放40个bit
unsigned long time1;
  bgn:
  delay(2000);
//设置2号接口模式为：输出
//输出低电平20ms（>18ms）
//输出高电平40μs
  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);
  delay(20);
  digitalWrite(pin,HIGH);
  delayMicroseconds(40);
  digitalWrite(pin,LOW);
//设置2号接口模式：输入
  pinMode(pin,INPUT);
  //高电平响应信号
  loopCnt=10000;
  while(digitalRead(pin) != HIGH)
  {
    if(loopCnt-- == 0)
    {
//如果长时间不返回高电平，输出个提示，重头开始。
      Serial.println("HIGH");
      goto bgn;
    }
  }
  //低电平响应信号
  loopCnt=30000;
  while(digitalRead(pin) != LOW)
  {
    if(loopCnt-- == 0)
    {
//如果长时间不返回低电平，输出个提示，重头开始。
      Serial.println("LOW");
      goto bgn;
    }
  }
//开始读取bit1-40的数值  
    for(int i=0;i<40;i++)
  {
    while(digitalRead(pin) == LOW)
    {}
//当出现高电平时，记下时间“time”
    time1 = micros();
    while(digitalRead(pin) == HIGH)
    {}
//当出现低电平，记下时间，再减去刚才储存的time
//得出的值若大于50μs，则为‘1’，否则为‘0’
//并储存到数组里去
    if (micros() - time1  >50)
    {
      chr[i]=1;
    }else{
      chr[i]=0;
    }
  }
   
//湿度，8位的bit，转换为数值
humi=chr[0]*128+chr[1]*64+chr[2]*32+chr[3]*16+chr[4]*8+chr[5]*4+chr[6]*2+chr[7];
   
//温度，8位的bit，转换为数值
temp=chr[16]*128+chr[17]*64+chr[18]*32+chr[19]*16+chr[20]*8+chr[21]*4+chr[22]*2+chr[23];
  //校对码，8位的bit，转换为数值
//tol=chr[32]*128+chr[33]*64+chr[34]*32+chr[35]*16+chr[36]*8+chr[37]*4+chr[38]*2+chr[39];
//输出：温度、湿度、校对码
  Serial.print("temp:");
  Serial.println(temp);
  sprintf(wsdtemp,"mode=up&apikey=%s&data={ck%03d%03d%d.%d}\r\n",APIKEY,wsSID,wsNID,humi,temp);
  sendTCP(wsdtemp);
  Serial.println(temp);
  Serial.print("humi:");
  Serial.println(humi);
  
}
*/
/*
 * 运行中检查用户是否按键，如果有按键判断是短按或者长按，分别进入smartconfig模式或者恢复出厂
*/
void RunKey()
{
  char keyCnt,shortkey_en;
  unsigned long preTick;
  int num;
  if(digitalRead(PINKEY) == 0)                  //运行中判断按钮是否按下
  {
    digitalWrite(LED, 0);
    Serial.println("Short press key: smartconfig mode\r\nlong press key:factory restore mode");
    preTick = millis();
    keyCnt = 0;
    num = 0;
    shortkey_en=0;
    while(1)
    {
      ESP.wdtFeed();
      if (millis() - preTick < 10 )//10ms检测一次按键
        continue;
      preTick = millis();
      num++;
      if (num % 10 == 0)
      {
        ledState = !ledState;
        digitalWrite(LED, ledState);
        Serial.print(".");
      }
      if (keyCnt >= 200)
      { //长按                
        keyCnt = 0;
        Serial.println("\r\nRLong Press key");
        restoreFactory();                               //长按进入恢复出厂设置
      }
      else if (keyCnt >= 5 && shortkey_en==1)
      { //短按
        keyCnt = 0;
        Serial.println("\r\nShort Press key");
       // doSmartconfig();                               //短按进入smartconfig设置
        if(outsta==1)
        {
          outsta=0;
          digitalWrite(OUT,LOW);
          Serial.println("LOW");
        }
        else
        {
          outsta=1;
          digitalWrite(OUT,HIGH);
          Serial.println("HIGH");
        }
      }
      if (digitalRead(PINKEY) == 0) 
        keyCnt++;
      else
      {
        if(keyCnt>=5)//短按有效
          shortkey_en=1;
        else
        { 
          keyCnt = 0;
          return;
        }
      }
    }
  }
}
/*
*初始化WiFi
*/
void initWiFi()
{
  char temp[32];
  uint8_t mac[6];
  WiFi.macAddress(mac);       //读取mac地址
  sprintf(temp, "%s_%02X%02X%02X", HOST_NAME, mac[3], mac[4], mac[5]);
  Serial.printf("\r\nMAC address:%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  WiFi.hostname(temp);

  sprintf(temp, "%s_%02X%02X%02X", apssid, mac[3], mac[4], mac[5]);

  if (strcmp(config.stassid, DEFAULT_STASSID) != 0)//判断ssid是否为空
  {//SSID不空
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.begin(config.stassid, config.stapsw);
  }
  else 
  {//SSID为空
    Serial.println("\r\nNo sta is set");  //提示STA没有设置
    WiFi.disconnect();                    //WiFi断开连接
    delay(100);                            //延时0.1秒
    WiFi.mode(WIFI_AP);                   //改为AP模式
    Serial.println("AP MODE");
  }
  WiFi.softAP(temp);                      //设置AP名称
}
/*
*WiFiTick
*检查是否需要初始化WiFi
*检查WiFi是否连接上，若连接成功启动TCP Client
*控制指示灯
*/
void wifiTick()
{
  static bool wifiInitFlag=false;  
  static bool taskStarted = false;
  static bool ledTurnon = false;
  if(!wifiInitFlag)                     //判断wifi初始化标志
  {
    wifiInitFlag = true;               
    initWiFi();                         //初始化wifi
    initHttpServer();                   //初始化http服务器
    startUDPServer(udpPort);            //开始udp广播
    Serial.printf("Heap size:%d\r\n",ESP.getFreeHeap());//打印剩余空间
  }
  
  if ( WiFi.status() != WL_CONNECTED )//判断是否连接  
  {
    if (millis() - lastWiFiCheckTick > 1000) //WiFi没有连接就每秒闪一下
    {
      lastWiFiCheckTick = millis();
      ledState = !ledState; 
      digitalWrite(LED, ledState);
      ledTurnon = false;
    }
  }
  else                                           //已经连上路由器
  {
    if (ledTurnon == false)
    {
      ledTurnon = true;
      digitalWrite(LED, 0);                      //wifi连接后小蓝灯常亮
      ReadTimeEn=true;
      RTPre=millis();
    }
    if (taskStarted == false)
    {
      taskStarted = true;
      Serial.print("\r\nGet IP Address: ");
      Serial.println(WiFi.localIP());
      startTCPClient();                          //连接服务器
    }
  }
}

/*
*检查串口是否有数据
*/
void doUartTick()
{
  if(Serial.available())
  {
    size_t len = Serial.available();//获得串口有效数据长度
    uint8_t sbuf[len];                //申请一块内存空间
    Serial.readBytes(sbuf, len);     //读取串口有效数据
    parseUartPackage((char*)sbuf);    //处理串口接收到的数据
  }
}

void Usart_Init(void)
{
  Serial.begin(115200);
  Serial.println("Start module");
}

void GPIO_Init(void)
{
  pinMode(LED, OUTPUT);             //引脚配置为输出
  pinMode(OUT, OUTPUT);            //引脚配置为输出
  pinMode (PINKEY, INPUT_PULLUP);  //配置为上拉输入
  digitalWrite(LED, 0);             //指示灯亮
  outsta=1;
  digitalWrite(OUT, HIGH);          //关闭输出
}
/*
*初始化
*/
void setup()
{
  Usart_Init();                     //串口初始化
  GPIO_Init();                      //端口初始化
 //irsend.begin();
  Serial.println("\r\n\r\nStart ESP8266");
  Serial.print("Firmware Version:");
  Serial.println(VER);
  Serial.print("SDK Version:");
  Serial.println(ESP.getSdkVersion());
  wifi_station_set_auto_connect(0);//关闭自动连接  
  ESP.wdtEnable(5000);
  loadConfig();
  initParseData();                                //初始化参数
  
  if(strcmp(config.stassid,DEFAULT_STASSID)==NULL)             //ssid为空，需要smartconfig配置
  {
    Serial.println("SSID is NULL");
    doSmartconfig();
  }
  else
  {
    Serial.print("SSID:");
    Serial.println(config.stassid);
    Serial.print("PSW:");
    Serial.println(config.stapsw);
    Serial.println("WiFi Connection");
  }
  ReadTimeEn=false;
}
/*
*主循环
*/
void loop()
{
  ESP.wdtFeed();
  wifiTick();                     //连接服务监控，断线自动连接
  doHttpServerTick();
  doTCPClientTick();
  RunKey();
}

