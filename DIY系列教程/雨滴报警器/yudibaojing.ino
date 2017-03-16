#include <ESP8266WiFi.h>
 WiFiClient client;
#define relay1 2 //雨滴模块的DO脚连接在8266的GPIO2上
const char *ssid     = "****";//这里是我的wifi，你使用时修改为你要连接的wifi ssid
const char *password = "********";//你要连接的wifi密码
const char *host = "jcck.online";//酱菜创客服务器ip
const int httpPort =8266;
int m=0;
void setup() {

  Serial.begin(115200);
  delay(10);
  pinMode(relay1,INPUT);
  // We start by connecting to a WiFi network
   
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  //Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // smartConfig();
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  while (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //return;
  }
  Serial.print("connecting to ");
  Serial.println(host);
  client.write("mode=bind&apikey=修改成你自己的apikey&data={ck001000bind}\r\n");//绑定apikey,修改成你自己的apikey
  delay(1000);
}
void loop() 
{
int val=digitalRead(relay1);
if(val==HIGH)
{
  client.write("mode=up&apikey=你自己的apikey&data={ck0010060}\r\n");//修改成你自己的apikey
  Serial.println("没有雨");
 }
 if(val==LOW)
{
  client.write("mode=up&apikey=修改成你自己的apikey&data={ck0010061}\r\n");//修改成你自己的apikey
  Serial.println("下雨了");
 }
while(client.available()){
    String line = client.readStringUntil('\r');
     Serial.print(line);}
     m++;
    if(m%10==0){
      client.write("mode=up&apikey=修改成你自己的apikey&data={ck001006life}\r\n");//修改成你自己的apikey
      m=0;
      }
  delay(3000);
}