#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>

const char* ssid     = "SUZAKU_PC";
const char* password = "12122001";
const int LED=5;
Servo myservo;
WebServer server(80);
String html;
bool locked = false;
String  key = "NEKOhakeny";

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
  String changed = form;
  Serial.println(changed);
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
void setup(void) 
{
  pinMode(LED,OUTPUT);
  myservo.attach(2); 
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
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
