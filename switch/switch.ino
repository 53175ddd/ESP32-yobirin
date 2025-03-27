#include <WiFi.h>
#include <esp_now.h>

#define BAUDRATE 115200

#define LED_BUILTIN 5  // 送信成功すると光るやつ
#define SW          4  // 信号送るトリガースイッチ

uint8_t target[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // 使ってない

char cmd[] = "HELLO";  // この文字列を送ると起動する

esp_now_peer_info_t peer_info;

void esp_now_callback(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(status == ESP_NOW_SEND_SUCCESS) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN,  LOW);
  }
}

void setup() {
  Serial.begin(BAUDRATE);

  WiFi.mode(WIFI_STA);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SW         ,  INPUT_PULLUP);

  if(esp_now_init() == ESP_OK){  // 初期化が正しく通ったかの判別
    Serial.println("Success to initialize ESP-NOW");
  }else {  // 初期化に失敗した時の処理
    Serial.println("Error initializing ESP-NOW");

    while(true) {  // 初期化失敗したときに入る無限ループ
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN,  LOW);
      delay(50);
    }
  }

  memcpy(peer_info.peer_addr, target, 6);
  peer_info.channel = 0;      // Wi-Fi のチャンネル選択  
  peer_info.encrypt = false;  // 暗号化の有無

  if(esp_now_add_peer(&peer_info) == ESP_OK) {
    Serial.println("Successed to add peer");
  }else {
    Serial.println("Faild to add peer");
  }

  esp_now_register_send_cb(esp_now_callback);
}

void loop() {
  static uint8_t pin = 0;

  pin = (pin << 1) + (digitalRead(SW) == HIGH ? 0 : 1);

  if((pin & 0b00000111) == 0b001) {
    send_cmd();
  } 
}

void send_cmd(void) {
  esp_now_send(target, (uint8_t*)&cmd, sizeof(cmd));
}
