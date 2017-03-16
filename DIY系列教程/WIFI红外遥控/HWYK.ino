/*
 * IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to ESP8266 pin 0.
 * Version 0.1 June, 2015
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 */

#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
 WiFiClient client;
//#define relay1 2 //LED连接在8266的GPIO2上
//#define pin 4
const char *ssid     = "Netcore";//这里是我的wifi，你使用时修改为你要连接的wifi ssid
const char *password = "01234567890";//你要连接的wifi密码
//char *ssid="";
//char *password="";
const char *host = "jcck.online";
const int httpPort =8266;
int m=0;
IRsend irsend(14); //an IR led is connected to GPIO pin 0
void setup()
{
  irsend.begin();
  Serial.begin(9600);
  //pinMode(relay1,OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     //smartConfig();
    Serial.print(".");
  }
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  while(!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //return;
  }
  Serial.print("connecting to ");
  Serial.println(host);
  client.write("mode=bind&apikey=674c0a5616458361&data={ck001000bind}\r\n");//绑定apikey
  delay(10);
}

void loop() {
       while(client.available()){
      String line = client.readStringUntil('\r');
     Serial.print(line);
    if(line[5]=='5' & line[8]=='1' & line[9]=='0')//修改sid和nid,line[5]为你的sid值，line[8]为你的nid值，line[9]为我发送的data，因为我的空调设备sid为005，nid为001，所以line[5]=5,line[8]=1,下面也是一样。
    {
      irsend.sendMIDEA(0x7B84E01F, 32); 
      irsend.sendMIDEA(0x7B84E01F, 32); 
      Serial.println("kongtiao off");
     }
     if(line[5]=='5' & line[8]=='1' & line[9]=='1' & line[10]=='7')
    { 
      irsend.sendMIDEA(0x1FE008F7, 32); 
      irsend.sendMIDEA(0x1FE008F7, 32);
      Serial.println("17度");
     }
     if(line[5]=='5' & line[8]=='1' & line[9]=='1' & line[10]=='8')
    { 
      irsend.sendMIDEA(0x1FE018E7, 32); 
      irsend.sendMIDEA(0x1FE018E7, 32);
      Serial.println("18度");
     }
     if(line[5]=='5' & line[8]=='1' & line[9]=='1' & line[10]=='9')
    { 
      irsend.sendMIDEA(0x1FE038C7, 32); 
      irsend.sendMIDEA(0x1FE038C7, 32);
      Serial.println("19度");
     }
     if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='0')
    { 
      irsend.sendMIDEA(0x1FE028D7, 32); 
      irsend.sendMIDEA(0x1FE028D7, 32);
      Serial.println("20度");
     }
      if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='1')
    { 
      irsend.sendMIDEA(0x1FE06897, 32); 
      irsend.sendMIDEA(0x1FE06897, 32);
      Serial.println("21度");
     }
     if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='2')
    { 
      irsend.sendMIDEA(0x1FE07887, 32); 
      irsend.sendMIDEA(0x1FE07887, 32);
      Serial.println("22度");
     }
     if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='3')
    { 
      irsend.sendMIDEA(0x1FE058A7, 32); 
      irsend.sendMIDEA(0x1FE058A7, 32);
      Serial.println("23度");
     }
     if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='4')
    { 
      irsend.sendMIDEA(0x1FE048B7, 32); 
      irsend.sendMIDEA(0x1FE048B7, 32);
      Serial.println("24度");
     }
     if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='5')
    { 
      irsend.sendMIDEA(0x1FE0C837, 32); 
      irsend.sendMIDEA(0x1FE0C837, 32);
      Serial.println("25度");
       }
       if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='6')
    { 
      irsend.sendMIDEA(0x1FE0D827, 32); 
      irsend.sendMIDEA(0x1FE0D827, 32);
      Serial.println("26度");
      }
      if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='7')
    { 
      irsend.sendMIDEA(0x1FE09867, 32); 
      irsend.sendMIDEA(0x1FE09867, 32);
      Serial.println("27度");
      }
      if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='8')
    { 
      irsend.sendMIDEA(0x1FE08877, 32); 
      irsend.sendMIDEA(0x1FE08877, 32);
      Serial.println("28度");
      }
      if(line[5]=='5' & line[8]=='1' & line[9]=='2' & line[10]=='9')
    { 
      irsend.sendMIDEA(0x1FE0A857, 32); 
      irsend.sendMIDEA(0x1FE0A857, 32);
      Serial.println("29度");
      }
       if(line[5]=='5' & line[8]=='1' & line[9]=='3' & line[10]=='0')
    { 
      irsend.sendMIDEA(0x1FE0B847, 32); 
      irsend.sendMIDEA(0x1FE0B847, 32);
      Serial.println("30度");
      }
  }
   
String line ="";
  m++; 
  if(m%200==0)
  {
    client.write("mode=up&apikey=674c0a5616458361&data={ck005001life}\r\n");
           m=0;
   }
  delay(60);
}
