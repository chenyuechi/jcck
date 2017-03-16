#ifndef __MAIN_H_
#define __MAIN_H_


extern "C" {
#include "user_interface.h"
#include "smartconfig.h"
}

//#include "jieXi.h"

struct config_type
{
  char stassid[32];	 //
  char stapsw[64];	 //
  char id[64];		   //
  char key[64];		   //
  uint8_t magic; 	   //��������ʽ����־
};

struct Rconfig_type
{
  char Phone_jm;
  char Esp_jm;
  char cmd[12];     //首命令  
  char user[12];    //用户名
  char mac[6];   //mac地址
  char zilei[3];    //子编类
  char zcmd[12];    //子命令
  char str[64];     //子命令的内容
  char keyid[32];   //密钥
};

void startTCPClient();
void doTCPClientTick();
void startUDPServer(int);
void doUdpServerTick();
void sendTCP(char *);
void sendUDP(char *);
void initParseData();
void parseTCPPackage(char*);
void parseUDPPackage(char*);
void parseUartPackage(char*);

void initHttpServer();
void doHttpServerTick();
void delayRestart(float);
void printHelp();
void device_logout(void);

void device_dataTrans(void);
void device_login(void);

//����
#define VER             "V 1.0"
#define DEFAULT_APSSID  "Esp8266"
#define DEFAULT_STASSID ""
#define DEFAULT_STAPSW  ""
#define pin             2
#define OUT             4
#define PINKEY          5
#define HOST_NAME       "Esp_Device"
#define LEIBIE          "11001"
#define ZILEI           "1"

#define kgSID   3         //修改为你自己的设备sid
#define kgNID   0          //修改为你自己的设备nid
#define wsSID   1           //温湿度设备sid
#define wsNID   2          //温湿度设备nid
//#define hwSID   5        //修改为你自己红外设备sid 
//#define hwNID   1         //修改为你自己红外设备nid
#define APIKEY "674c0a5616458361"      //修改为你自己的apikey
//��Ʒ���
#define DEFAULT_ID    "WiFi_Drv"
#define DEFAULT_KEY    "123456"

const char apssid[]=DEFAULT_APSSID;
const char serverAddr[] = "jcck.online";
const uint16_t serverPort = 8266;

const uint16_t udpPort = 8089;

unsigned long lastWiFiCheckTick = 0;
bool ledState = 0;

config_type config;
Rconfig_type revdata;
#endif
