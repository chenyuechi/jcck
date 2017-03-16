#include <ESP8266WiFi.h>
WiFiClient client;
const char *ssid     = "****";//这里是我的wifi，你使用时修改为你要连接的wifi ssid
const char *password = "*********";//你要连接的wifi密码
const char *host = "jcck.online";
const int httpPort =8266;
#define pin 2
//void  keeponline();
//void wificonnect();
void wenshidu();
void wenshidu1();
int n=0;
void setup() {
  Serial.begin(115200);
  delay(10);
  //pinMode(relay1,INPUT);
  // We start by connecting to a WiFi network
   
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  //Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
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
  }
  Serial.print("connecting to ");
  Serial.println(host);
  client.write("mode=bind&apikey=你自己的apikey&data={ck001000bind}\r\n"); //修改成自己的apikey
  delay(100);
}

void loop() {
  while(client.available()){
    String line = client.readStringUntil('\n');
     Serial.println(line);  }
  n++;
   if(n%1==0)
  {
    wenshidu();
    n=0;
   }
  
   delay(30000);
   client.write("mode=up&apikey=你的apikey&data={ck001002life\r\n");  //修改成自己的apikey
   delay(3000);
}

void wenshidu()
{
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
    if (micros() - time1  >50)
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
  String str1="mode=up&apikey=674c0a5616458361&data={ck001002";
  str1+=humi;
  str1+=".";
  str1+=temp;
  str1+="}\r\n";
  client.print(str1);
   Serial.print(str1);
  Serial.print("humi:");
  Serial.println(humi);
 
}