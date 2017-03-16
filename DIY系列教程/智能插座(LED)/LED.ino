#include <ESP8266WiFi.h>
 WiFiClient client;
#define relay1 4 //继电器连接在8266的GPIO4上
const char *ssid     = "xxxx";//这里是我的wifi，你使用时修改为你要连接的wifi ssid
const char *password = "xxxxxxxxxxx";//你要连接的wifi密码
const char *host = "jcck.online";//酱菜创客服务器IP
const int httpPort =8266;//端口号
String line ="";
int m=0; 
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(relay1,OUTPUT);
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
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
  client.write("mode=bind&apikey=你的apikey&data={ck001000bind}\r\n");//绑定apikey,修改成你自己的apikey
  delay(10);
}

//int value = 0;

void loop() {
  while(client.available()){
    String line = client.readStringUntil('\r');
     Serial.println(line);  
    //valueread=line[47];
    //Serial.println(valueread);
    
    if(line[5]=='3' & line[8]=='0' & line[9]=='1')   //这里的line其实是收到的控制指令，这其实是解析控制指令的程序，
                                                    //因为你填的sid和nid是003和000，所以line[5]=3，line[8]=0，如果你填的sid和nid是004和000，那line[5]=4，line[8]=0，line[9]就是开关命令0/1
    {
      digitalWrite(relay1,HIGH);
      Serial.println("led on");
     }
     if(line[5]=='3' & line[8]=='0' & line[9]=='0')
    { 
      digitalWrite(relay1,LOW);
      Serial.println("led off");
     }
  }
   
String line ="";
  m++; 
  if(m%40==0)
  {
    client.write("mode=up&apikey=你的apikey&data={ck003000life}\r\n");//发送心跳消息，修改成你自己的apikey
    m=0;
   }
  delay(500);
}