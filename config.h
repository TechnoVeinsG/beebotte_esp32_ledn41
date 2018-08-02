// LEDN41に1234と表示させるコマンド
const char ledn41_cmd[8] = {0x1B, 0x30, 0x44, 0x31, 0x32, 0x33, 0x34, 0x0D};
// LEDN41のIPアドレス
const uint8_t ip[4] = {192,168,1,199};
// LEDN41のポート番号
const uint16_t port = 10001;
// MQTTホスト名
const char* host = "mqtt.beebotte.com";
// MQTTポート番号
const uint16_t mqttPort = 1883;
// クライアントID
const char *clientID = "TVC_ESP32";
// トピック名
const char* topic = "control_ledn41/display";
// Channel Token
const char* channelToken = "token_xxxxxxxxxxxxxxxx";
// Wi-Fi SSID
const char *ssid = "YOUR SSID";
// Wi-Fi Password
const char *password = "YOUR PASSWORD";
