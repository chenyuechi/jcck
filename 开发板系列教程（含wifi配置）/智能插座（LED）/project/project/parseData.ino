#include "project.h"
char Phone_jm_en,Esp_jm_en;
extern char keyid[];
/*
 * 初始化
*/
void initParseData()
{//初始化在这里
  Serial.println("[initParseData]");
}

/*
 * 设备登陆
*/
void device_login(void)
{
  char temp[64];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  sprintf(temp,"mode=bind&apikey=%s&data={ck%03d%03dbind}\r\n",APIKEY,wsSID,wsNID);
  sendTCP(temp);
  Serial.println(temp);
}
/*
 * 处理tcp信息
*/
void parseTCPPackage(char *p)
{
  char i;
  char sbuf[128];
  int sid,nid;
  char data1;
  char data2;
  char data3;
  Serial.print("[TCP parseData]:");
  Serial.println(p);

  sbuf[0]=*(p+3)-0x30;
  sbuf[1]=*(p+4)-0x30;
  sbuf[2]=*(p+5)-0x30;
  sid=sbuf[0]*100+sbuf[1]*10+sbuf[2];
  sbuf[0]=*(p+6)-0x30;
  sbuf[1]=*(p+7)-0x30;
  sbuf[2]=*(p+8)-0x30;
  nid=sbuf[0]*100+sbuf[1]*10+sbuf[2];
  data1=*(p+9)-0x30;
  data2=*(p+10)-0x30;
  data3=*(p+11)-0x60;
  
  if(sid==kgSID && nid==kgNID)
  {
    digitalWrite(OUT,data1);
  }
//  if(sid==hwSID && nid==hwNID && data1==0 )
//  {
//      irsend.sendMIDEA(0x7B84E01F, 32); 
//      irsend.sendMIDEA(0x7B84E01F, 32); 
//      Serial.println("kongtiao off");
//  }
//  if(sid==hwSID && nid==hwNID && data1==1 && data2==7 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE008F7, 32); 
//      irsend.sendMIDEA(0x1FE008F7, 32);
//      Serial.println("17度");
//  }
//   if(sid==hwSID && nid==hwNID && data1==1 && data2==8 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE018E7, 32); 
//      irsend.sendMIDEA(0x1FE018E7, 32);
//      Serial.println("18度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==1 && data2==9 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE038C7, 32); 
//      irsend.sendMIDEA(0x1FE038C7, 32);
//      Serial.println("19度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==0 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE028D7, 32); 
//      irsend.sendMIDEA(0x1FE028D7, 32);
//      Serial.println("20度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==1 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE06897, 32); 
//      irsend.sendMIDEA(0x1FE06897, 32);
//      Serial.println("21度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==2 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE07887, 32); 
//      irsend.sendMIDEA(0x1FE07887, 32);
//      Serial.println("22度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==3 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE058A7, 32); 
//      irsend.sendMIDEA(0x1FE058A7, 32);
//      Serial.println("23度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==4 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE048B7, 32); 
//      irsend.sendMIDEA(0x1FE048B7, 32);
//      Serial.println("24度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==5 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE0C837, 32); 
//      irsend.sendMIDEA(0x1FE0C837, 32);
//      Serial.println("25度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==6 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE0D827, 32); 
//      irsend.sendMIDEA(0x1FE0D827, 32);
//      Serial.println("26度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==7 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE09867, 32); 
//      irsend.sendMIDEA(0x1FE09867, 32);
//      Serial.println("27度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==8 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE08877, 32); 
//      irsend.sendMIDEA(0x1FE08877, 32);
//      Serial.println("28度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==9 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE0A857, 32); 
//      irsend.sendMIDEA(0x1FE0A857, 32);
//      Serial.println("29度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==3 && data2==0 && data3==4)
//  {
//      irsend.sendMIDEA(0x1FE0B847, 32); 
//      irsend.sendMIDEA(0x1FE0B847, 32);
//      Serial.println("30度");
//  }
//  if(sid==hwSID && nid==hwNID && data1==1 && data2==7 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC000FF, 32); 
//      irsend.sendMIDEA(0x3FC000FF, 32);
//      Serial.println("17度 冷");
//  }
//   if(sid==hwSID && nid==hwNID && data1==1 && data2==8 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC010EF, 32); 
//      irsend.sendMIDEA(0x3FC010EF, 32);
//      Serial.println("18度 冷");
//  }
//  if(sid==hwSID && nid==hwNID && data1==1 && data2==9 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC030CF, 32); 
//      irsend.sendMIDEA(0x3FC030CF, 32);
//      Serial.println("19度 冷");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==0 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC020DF, 32); 
//      irsend.sendMIDEA(0x3FC020DF, 32);
//      Serial.println("20度 冷");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==1 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC0609F, 32); 
//      irsend.sendMIDEA(0x3FC0609F, 32);
//      Serial.println("21度 冷");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==2 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC0708F, 32); 
//      irsend.sendMIDEA(0x3FC0708F, 32);
//      Serial.println("22度 冷");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==3 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC050AF, 32); 
//      irsend.sendMIDEA(0x3FC050AF, 32);
//      Serial.println("23度 冷");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==4 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC040BF, 32); 
//      irsend.sendMIDEA(0x3FC040BF, 32);
//      Serial.println("24度 冷");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==5 && data3==3)
//  {
//      irsend.sendMIDEA(0x3FC0C03F, 32); 
//      irsend.sendMIDEA(0x3FC0C03F, 32);
//      Serial.println("25度 冷");
//  }
//  if(sid==hwSID && nid==hwNID && data1==1 && data2==7 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC00CF3, 32); 
//      irsend.sendMIDEA(0x3FC00CF3, 32);
//      Serial.println("17度 热");
//  }
//   if(sid==hwSID && nid==hwNID && data1==1 && data2==8 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC01CE3, 32); 
//      irsend.sendMIDEA(0x3FC01CE3, 32);
//      Serial.println("18度 热");
//  }
//  if(sid==hwSID && nid==hwNID && data1==1 && data2==9 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC03CC3, 32); 
//      irsend.sendMIDEA(0x3FC03CC3, 32);
//      Serial.println("19度 热");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==0 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC02CD3, 32); 
//      irsend.sendMIDEA(0x3FC02CD3, 32);
//      Serial.println("20度 热");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==1 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC06C93, 32); 
//      irsend.sendMIDEA(0x3FC06C93, 32);
//      Serial.println("21度 热");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==2 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC07C83, 32); 
//      irsend.sendMIDEA(0x3FC07C83, 32);
//      Serial.println("22度 热");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==3 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC05CA3, 32); 
//      irsend.sendMIDEA(0x3FC05CA3, 32);
//      Serial.println("23度 热");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==4 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC04CB3, 32); 
//      irsend.sendMIDEA(0x3FC04CB3, 32);
//      Serial.println("24度 热");
//  }
//  if(sid==hwSID && nid==hwNID && data1==2 && data2==5 && data3==8)
//  {
//      irsend.sendMIDEA(0x3FC0CC33, 32); 
//      irsend.sendMIDEA(0x3FC0CC33, 32);
//      Serial.println("25度 热");
//  }
}


