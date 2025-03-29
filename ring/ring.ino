#include <WiFi.h>
#include <esp_now.h>

#define BAUDRATE 115200

#define SPK 14  // スピーカが繋がるピンの番号

typedef struct {
  uint16_t note;
  uint16_t tone_period;
  uint16_t mute_period;
} score_t;

score_t skype[] = {{311, 400, 0}, {466, 400, 0}, {311, 800, 400}, {466, 450, 0}, {294, 250, 0}, {466, 800, 400}};
score_t famima[] = {{369, 300, 0}, {293, 300, 0}, {220, 300, 0}, {293, 300, 0}, {329, 300, 0}, {440, 600, 0}, {220, 300, 0}, {329, 300, 0}, {369, 300, 0}, {329, 300, 0}, {220, 300, 0}, {293, 600, 1400}};
score_t machinami_haruka[] = {{1109, 200, 0}, {880, 200, 0}, {1109, 400, 0}, {1661, 400, 0}, {1480, 600, 0}, {1976, 180, 20}, {1976, 600, 0}, {1760, 200, 0}, {1661, 200, 0}, {1480, 200, 0}, {1319, 200, 0}, {1245, 200, 0}, {1760, 400, 0}, {1661, 400, 0}, {1480, 200, 0}, {1319, 200, 0}, {1245, 200, 0}, {1480, 200, 0}, {1319, 1200, 800}};

void ring(score_t score[], int32_t count) {
  for(size_t i = 0; i < count; i++) {
    tone(SPK, score[i].note, score[i].tone_period);
    delay(score[i].tone_period);
    delay(score[i].mute_period);
  }
}

void esp_now_callback(const esp_now_recv_info *esp_now_recieve_info, const unsigned char *recieve_data, int length) {
  char recieve_data_buffer[length];  // 受信データ格納用

  sprintf(recieve_data_buffer, "%s", recieve_data);

  if(strncmp(recieve_data_buffer, "HELLO", 5) == 0) {
    ring(skype, (sizeof(skype) / sizeof(score_t)));
  }
}

void setup() {
  Serial.begin(BAUDRATE);

  delay(100);

  pinMode(SPK, OUTPUT);
  setToneChannel(SPK);

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

void loop() {
}
