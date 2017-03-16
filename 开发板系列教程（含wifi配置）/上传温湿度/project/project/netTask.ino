#include "project.h"

#define MAX_PACKETSIZE 512
WiFiClient client;
WiFiUDP udp;
extern config_type config;
extern const char serverAddr[];
extern const uint16_t serverPort;
char buffTCP[MAX_PACKETSIZE];
unsigned int buffTCPIndex=0;
unsigned long preTCPTick=0;
unsigned long preTCPStartTick=0;
bool preTCPConnected=false;         //跟服务器连接成功标志
char buffUDP[MAX_PACKETSIZE];
unsigned long preUDPTick=0;
bool TickEn=false;
/*
 * 开启tcp client
*/
void startTCPClient()
{
  char buff1[128];
  Serial.println("\r\nstartTCPClient");              //开始连接服务器
  if(client.connect(serverAddr, serverPort))          //连接服务器，返回1连接成功 返回0连接失败
  {
      Serial.print("Connected to server:");          //连接成功
      Serial.println(serverAddr);
      preTCPConnected = true;                         //建立跟服务器连接成功标志
      device_login();  //登录设备  
      delay(1000);
      sprintf(buff1,"mode=up&apikey=%s&data={ck%03d%03d1}\r\n",APIKEY,kgSID,kgNID);//心跳包内容在此
      Serial.println(buff1);                               //串口发送
      client.print(buff1);
      delay(500);
      wenshidu();
  }
  else
  {
      Serial.print("Failed connected to server:");   //连接失败
      Serial.println(serverAddr);
      client.stop();
      preTCPConnected = false;                        //取消跟服务器连接成功标志
      TickEn=false;
  }
  preTCPStartTick = millis();
}

/*
 * 向服务器发送数据
*/
void sendTCP(char *p)
{
  if (!client.connected())                      //判断是否已经连接到服务器
  {
    Serial.println("Client is not readly");    //还未连接到服务器
    return;
  }
  client.print(p);                            //向服务器发送数据
  Serial.print("[TCP Client Send]:");          //向串口发送数据
  Serial.println(p);
}

/*
 * tcp client tick
*/
void doTCPClientTick()                         //客户端心跳
{
  char buf[64];
  //检查数据，发送心跳
  //检查是否断开，断开后重连
  if(WiFi.status() != WL_CONNECTED)           //未连接WiFi直接退出
    return;
   
  if (!client.connected()) //判断与服务器是否连接
  {//断开重连
    if(preTCPConnected==true)
    {
      TickEn=false;
      preTCPConnected = false;
      preTCPStartTick = millis();
      Serial.println();
      Serial.println("TCP Client disconnected.");
      client.stop();
    }
    else if(millis() - preTCPStartTick > 10*1000)      //断线10秒后重连
    {
      startTCPClient();                                //断开重连
    }
  }
  else                                                 //在线
  {
    if (client.available())                            //判断是否收到数据
    {//收数据
      buffTCP[buffTCPIndex++] = client.read();         //读取数据到缓冲
      preTCPTick = millis();                           //更新心跳时间
      if(buffTCPIndex>=MAX_PACKETSIZE - 1)             //判断数据接收指针是否到最大
      {
        buffTCPIndex = MAX_PACKETSIZE-2;               //超出512 覆盖最后一个字节
        preTCPTick = preTCPTick - 200;
      }
    }
    if(millis() - preTCPTick>=30*1000)// && TickEn==true)  //判断是否应该发心跳数据了
    {//保持心跳 1分中发送一次心跳包
      char buff[128];
      char temp1[128];
      char temp2[128];
      preTCPTick = millis();                               //更新心跳包数据时间
      //sprintf(buff,"mode=up&apikey=%s&data={ck%03d%03dlife}\r\n",APIKEY,hwSID,hwNID);//心跳包内容在此
      Serial.println(buff);                               //串口发送
      client.print(buff);                                //网络发送
      delay(500);
      sprintf(temp1,"mode=up&apikey=%s&data={ck%03d%03dlife}\r\n",APIKEY,kgSID,kgNID);
     client.print(temp1); 
      delay(500);
      sprintf(temp2,"mode=up&apikey=%s&data={ck%03d%03dlife}\r\n",APIKEY,wsSID,wsNID);
      client.print(temp2); 
      delay(1000);
      wenshidu();
    }
  }
  if(buffTCPIndex>0 && (millis() - preTCPTick>=100))      //收到过数据，而且上一个数据是在100ms之前发的
  {//data ready
    buffTCP[buffTCPIndex]=0x00; //收到的数据包最后一个字符填0
    client.flush();             //等待缓冲区数据发送完成
    parseTCPPackage(buffTCP);   //处理TCP信息，解析执行TCP指令
    buffTCPIndex = 0;           //数据接收指针指向行首
  }   
}
/*
 * 开启udp server监听
*/
void startUDPServer(int port)
{
  Serial.print("\r\nStartUDPServer at port:");
  Serial.println(port);
  udp.begin(port);
}
/*
 * udp服务器tick
*/
void doUdpServerTick()
{
  if(udp.parsePacket())
  {
    memset(buffUDP,0x00,sizeof(buffUDP));
    udp.read(buffUDP, MAX_PACKETSIZE-1);
    udp.flush();
    parseUDPPackage(buffUDP);
  }
}
/*
 * 向udp客户端发送数据
*/
void sendUDP(char *p)
{
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write(p);udp.endPacket();
}

