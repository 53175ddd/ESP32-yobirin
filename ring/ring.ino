#include <WiFi.h>
#include <esp_now.h>

#define SPK 14  // スピーカが繋がるピンの番号

void esp_now_callback(const esp_now_recv_info *esp_now_recieve_info, const unsigned char *recieve_data, int length) {
  char recieve_data_buffer[length];  // 受信データ格納用

  sprintf(recieve_data_buffer, "%s", recieve_data);

  if(strncmp(recieve_data_buffer, "HELLO", 5) == 0) {
    ring_famima ();
  }
}

void setup() {
  pinMode(SPK, OUTPUT);
  setToneChannel(SPK); 
}

void loop() {
  WiFi.mode(WIFI_STA); 

  if(esp_now_init() == ESP_OK){
    Serial.println("Success to initialize ESP-NOW");
  }else {
    Serial.println("Error initializing ESP-NOW");

    while(true) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN,  LOW);
      delay(50);
    }
  }

  esp_now_register_recv_cb(esp_now_callback);  
}

void ring_famima(void) {
  tone(SPK, 369, 300);
  delay(300);
  tone(SPK, 293, 300);
  delay(300);
  tone(SPK, 220, 300);
  delay(300);
  tone(SPK, 293, 300);
  delay(300);
  tone(SPK, 329, 300);
  delay(300);
  tone(SPK, 440, 600);
  delay(600);
  tone(SPK, 220, 300);
  delay(300);
  tone(SPK, 329, 300);
  delay(300);
  tone(SPK, 369, 300);
  delay(300);
  tone(SPK, 329, 300);
  delay(300);
  tone(SPK, 220, 300);
  delay(300);
  tone(SPK, 293, 600);
  delay(2000);
}
