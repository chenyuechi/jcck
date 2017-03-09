#include <ESP8266WiFi.h>
//#include <EEPROM.h>
 WiFiClient client;
#define relay1 4 //继电器连接在8266的GPIO2上
const char *ssid     = "House Wireless 1302";//这里是我的wifi，你使用时修改为你要连接的wifi ssid
const char *password = "18601092310";//你要连接的wifi密码
//char *ssid;
//char *password;
const char *host = "jcck.online";//修改为手机的的tcpServer服务端的IP地址，即手机在路由器上的ip
const int httpPort =8266;
String line ="";
int m=0; 
//int ssidAddress = 0; 
//int passwdAddress = 20;
/*void smartConfig();
void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig");
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      strcpy(ssid, WiFi.SSID().c_str());
      strcpy(password, WiFi.psk().c_str());
      //EEPROM.put(ssidAddress, ssid);
      //EEPROM.put(passwdAddress, password);
      break;
    }
  }
} */
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(relay1,OUTPUT);
  // We start by connecting to a WiFi network
  //EEPROM.get(ssidAddress, ssid);
  //EEPROM.get(passwdAddress, password);
  Serial.println(ssid);
  Serial.println(password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  // WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //smartConfig();
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  while (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //return;
    delay(500);
  }
  Serial.print("connecting to ");
  Serial.println(host);
  client.write("mode=bind&apikey=674c0a5616458361&data={ck001000bind}\r\n");
  delay(3000);
  //delay(10);
  client.write("mode=up&apikey=674c0a5616458361&data={ck0040001}\r\n");
}

//int value = 0;

void loop() {
  while(client.available()){
    String line = client.readStringUntil('\n');
     Serial.println(line);  
   
    if(line[5]=='4' & line[8]=='0' & line[9]=='0')
    {
      
      digitalWrite(relay1,LOW);
      Serial.println("led off");
     }
     if(line[5]=='4' & line[8]=='0' & line[9]=='1')
    { 
      digitalWrite(relay1,HIGH);
      Serial.println("led on");
     }
  }
   
String line ="";
  m++; 
  if(m%200==0)
  {
    client.write("mode=up&apikey=674c0a5616458361&data={ck004000update}\r\n");
       m=0;
   }
  delay(600);
}
