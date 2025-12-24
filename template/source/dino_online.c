#include <Arduino.h>
#include <WiFi.h>  
#include <WiFiClient.h>  
#include <WiFiServer.h>  
  
const char* ssid = "your_SSID";  
const char* password = "your_PASSWORD";  
  
WiFiServer server(8080); // 在8080端口上创建服务器  
  
void setup() {  
  Serial.begin(115200); // 初始化串口通信  
  WiFi.begin(ssid, password); // 连接到Wi-Fi网络  
  
  while (WiFi.status() != WL_CONNECTED) {  
    delay(1000); // 等待连接  
    Serial.println("Connecting to WiFi...");  
  }  
  
  Serial.println("Connected to WiFi!");  
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP()); // 打印ESP32的IP地址  
  
  server.begin(); // 启动服务器  
}  
  
void loop() {  
  WiFiClient client = server.available(); // 检查是否有新的客户端连接  
  if (client) {  
    Serial.println("New client connected");  
    String currentLine = "";  
    while (client.connected() && !client.available()) {  
      delay(1); // 空循环等待数据  
    }  
  
    while (client.connected() && client.available()) {  
      char c = client.read(); // 读取客户端发送的数据  
      Serial.write(c);        // 将数据打印到串口监视器  
      if (c == '\n') {  
        // 当接收到换行符时，处理接收到的数据  
        Serial.println(currentLine);  
        // 根据需要添加处理数据的代码  
        currentLine = ""; // 清空当前行，以便接收下一行数据  
      } else if (c != '\r') {  
        currentLine += c; // 将字符添加到当前行  
      }  
    }  
  
    client.stop(); // 关闭客户端连接  
    Serial.println("Client disconnected");  
  }  
}