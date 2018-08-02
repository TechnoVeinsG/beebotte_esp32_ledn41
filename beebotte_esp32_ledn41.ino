#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

#define MQTT_MAX_PACKET_SIZE 1024

WiFiClient wifiClient;
PubSubClient MQTTClient(host, mqttPort, wifiClient);
IPAddress ledn41_IP(ip[0],ip[1],ip[2],ip[3]);

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.println("MQTT message arrived");
  
  char buffer[MQTT_MAX_PACKET_SIZE];
  StaticJsonBuffer<MQTT_MAX_PACKET_SIZE> jsonBuffer;

  // Beebotteから受信したペイロードをbufferにコピー
  snprintf(buffer, sizeof(buffer), "%s", payload);
  
  // json形式のデータbufferをパース
  JsonObject& root = jsonBuffer.parseObject(buffer);

  if (!root.success()) {
    Serial.println("parseObject failed");
    return;
  }

  // dataの値を取得
  const char* parsedData = root["data"];

  if (parsedData != NULL) {
    
    // dataの値がdisplay_onの場合、LEDN41に表示コマンドを送信
    if (strcmp(parsedData, "display_on") == 0) {
      
      Serial.println("got display_on from beebotte");

      // LEDN41に接続
      if (wifiClient.connect(ledn41_IP, port)) {
        
        Serial.println("connected to ledn41");
        
        // LEDN41にコマンド送信
        wifiClient.write(ledn41_cmd, sizeof(ledn41_cmd));

      }
      
    }
    
  }

}

void setup() {
  
  Serial.begin(115200);
  Serial.println();
  
  WiFi.mode(WIFI_STA);
  
}

void loop() {
  
  // Wi-Fi接続されていない場合
  if (WiFi.status() != WL_CONNECTED) {
    
    Serial.print("connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    
    WiFi.begin(ssid, password);
    
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      
      Serial.println("Failed to connect");
      delay(5000);
      return;
      
    } else {
      
      Serial.print("Wi-Fi connected: ");
      Serial.println(WiFi.localIP());
      
    }
    
  }

  // Beebotteに接続されていない場合
  if (!MQTTClient.connected()) {
    
    String tokenname = "token:";
    tokenname += channelToken;
    
    // Beebotteに接続
    MQTTClient.connect(clientID, tokenname.c_str(), NULL);
    
    if (MQTTClient.connected()) {
      
      Serial.println("MQTT connected");
      
      // トピック名を指定してsubscriberに設定
      MQTTClient.subscribe(topic);
      
      // Beebotteからのメッセージ受信時に呼ばれる関数を設定
      MQTTClient.setCallback(callback);
      
    } else {
      
      Serial.print("MQTT connection failed: ");
      Serial.println(MQTTClient.state());
      delay(5000);
      
    }
    
  } else {
    
    // Beebotteとの接続をキープ
    MQTTClient.loop();
    
  }

}
