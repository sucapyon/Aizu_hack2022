#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "rx600m-bf5de4-1";
const char* password = "2fc8ecf29924b";

const int LED=5;
bool flg=HIGH;

WebServer server(80);
char *html;

void create_HTML()
{
  String s = "";
  s += "<!DOCTYPE html>\n<html lang \"ja\">\n<head>\n<meta charset = \"utf-8\"><meta charset=\"utf-8\">\n  <title>オートロック管理システム</title>\n</head>\n\n<body>\n  <form action=\"#\" method=\"post\">\n    パスワード：<br>\n    <input type=\"text\" name=\"password\" id=\"Pass\">\n    <input type=\"button\" value=\"確認する\" id=\"Conf\" oncick=Make_body(this.id);></p>\n    <script>\n      const post = (path, body) => {\n        const request = new \nMLHttpRequest();\n        request.open(\'POST\', path, false);\n        request.setRequestHeader(\"Content-Type\", \"text/plain\");\n        request.send(body);\n        if (request.status === 200) {\n          alert(\'リクエストに成功しました\');\n          return JSON.parse(request.responseText);\n        } else {\n          alert(\'リクエストに失敗しました\');\n          return 0;\n        }\n      }\n      const pass = document.getElementId(\"Pass\");\n      const path = \"192.168.1.3:80\";\n      const Make_body = () => {\n        const body = pass.value;\n        post(path, pass.value);\n        if (post == 0) console.log(pass.value);\n      }\n    </script>\n</body>\n</form>\n</html>";
  s.toCharArray(html,s.length());
}

void mainPage() 
{
  Serial.println("mainPage on");
  create_HTML();
  \\server.send(200,"text/html",html);
  server.send(200,"text/plain","This is main page");
}

void sendState() 
{
  if(server.method()!=HTTP_GET)
  {
    server.send(404,"text/plain","ng");
  }
  Serial.println("state start");
  //server.send(200, "application/json", "{\"state\":\""+String(flg?"ON":"OFF")+"\"}");
}
void getChange()
{
  String mes;
  if(server.method()!=HTTP_POST)
  {
    Serial.println("cant this method");
    server.send(404,"text/plain","ng");
  }
   Serial.println("getChange start");
  /*mes=server.arg("plain");
  if(mes=="on"){
    flg=HIGH;
    Serial.println("ON!");
  }else if(mes=="off"){
    flg=LOW;
    Serial.println("OFF!");
  }else{
    Serial.print(mes);
    Serial.println("is not found");
    server.send(404,"text/plain","ng");
  }
  digitalWrite(LED,flg);
  server.send(200, "text/plain", "ok");
  */
}

void setup(void) 
{
  pinMode(LED,OUTPUT);
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
  server.on("/state",sendState);
  server.on("/change",getChange);
  server.begin();
}

void loop(void) 
{
  server.handleClient();
}
