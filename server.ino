#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>
#include "BluetoothSerial.h"

const char* ID     = "rx600m-bf5de4-1";
const char* PASS = "2fc8ecf29924";
BluetoothSerial SerialBT;
const int LED=5;
Servo myservo;
WebServer server(80);
String html;
bool locked = false;


void mainPage() 
{
  Serial.println("mainPage on");
  html = "<!DOCTYPE html>\n<html lang \"ja\">\n<head>\n<meta charset = \"utf-8\"><meta charset=\"utf-8\">\n  <title>オートロック管理システム</title>\n</head>\n\n<body>\n  <form action=\"/confirm\" method=\"post\" enctype = \"text/plain\">\n    パスワード：<br>\n    <input type=\"text\" name=\"password\">\n    <input type=\"submit\" value = \"確認する\">\n  </form>\n</body>\n</html>";
  server.send(200,"text/html",html);
  
  
}
void mainPage_2()
{
  String form = server.arg("plain");
  Serial.println(form);
  form.replace("password=","");
  Serial.println(form);
  if(changed.indexOf("NEKOhakeny")==-1)
  {
    server.send(200,"text/plain","ThIs password is wrong.");
    return;
  }
  html = "\n<html lang \"ja\">\n<head>\n<meta charset=\"utf-8\">\n  <title>ロック管理</title>\n</head>\n<body>\n  <p>\n";
  html += sendState();
  html += "</p>\n  <form action = \"/lock\" method = \"post\" enctype = 'text/plain'>\n";
  html += "<input type=\"hidden\" name='password' value=\"NEKOhakeny\">";
  html += "<input type=\"submit\" value=\"鍵をかける\" id=\"Lock\" oncick=lock(this.id);><br>\n  </form>\n    <form action = \"/unlock\" method = \"post\" enctype = 'text/plain'>\n <input type = \"hidden\" value = \"NEKOhakeny\" name = \"password\"><input type=\"submit\" value=\"鍵を開ける\" id=\"unLock\" oncick=unlock(this.id);>\n  </form>\n    <script>\n\n    </script>\n</body>\n</html>\n";
  server.send(200,"text/html",html);
}
String sendState()
{
  if(locked)
  {
    return "鍵はロックされています";
  }
  else
  {
    return "鍵はロックされていません";
  }
}

void lock()
{
  Serial.println("lockpage accessed");
  myservo.write(90);
  locked = true;
   mainPage_2();
  
}
void unlock()
{
  Serial.println("unLockpage accessed");
  myservo.write(0);
  locked = false;
  mainPage_2();
}
bool new_connect()
{ 
  SerialBT.begin("esp32-demo");
  int challenge = 0;
  while(1)
  {
    char *new_ssid;
    char *new_pass;
    if(SerialBT.available())
    {
      if(challenge > 3)return false;
      String read_ssid = SerialBT.readStringUntil('\n');
      String read_pass = SerialBT.readStringUntil('\n');
      read_ssid.toCharArray(new_ssid,read_ssid.length());
      read_pass.toCharArray(new_pass,read_pass.length());
      if(connect(new_ssid,new_pass))return true;
      challenge++;
    }
  }
  return false;
}
bool connect(char *ssid,char *password)
{
  
  WiFi.begin(ssid, password);
  int waiting = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    waiting++;
    Serial.println(waiting);
    if(waiting > 10)
    {
      return new_connect();
    } 
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}
void super_unlock()
{
  delay(1000*60*60*5UL);
  unlock();
}
void setup() 
{
  pinMode(LED,OUTPUT);
  myservo.attach(2); 
  Serial.begin(115200);
  if(!connect((char *)ID,(char *)PASS))super_unlock();
  
  server.on("/", mainPage);
  server.on("/confirm",mainPage_2);
  server.on("/lock",lock);
  server.on("/unlock",unlock);
  server.begin();
}

void loop(void) 
{
  server.handleClient();
}
