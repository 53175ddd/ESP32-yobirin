#include <sound.h>

#include <WiFi.h>
#include <esp_now.h>

#define BAUDRATE 115200

#define SPK 14  // スピーカが繋がるピンの番号

#define CONCERT_PITCH 440.0f

sound music(CONCERT_PITCH);

uint8_t target[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

esp_now_peer_info_t peer_info;

score_t skype[] = {{Ds4, DOTTED_EIGHTH_NOTE, 0}, {As4, DOTTED_EIGHTH_NOTE, 0}, {Ds4, QUARTER_NOTE, QUARTER_NOTE + EIGHTH_NOTE}, {As4, QUARTER_NOTE, 0}, {D4, EIGHTH_NOTE, 0}, {As4, QUARTER_NOTE, DOTTED_EIGHTH_NOTE}, {Ds4, DOTTED_EIGHTH_NOTE, 0}, {As4, DOTTED_EIGHTH_NOTE, 0}, {Ds4, QUARTER_NOTE, QUARTER_NOTE + EIGHTH_NOTE}, {As4, QUARTER_NOTE, 0}, {D4, EIGHTH_NOTE, 0}, {As4, QUARTER_NOTE, EIGHTH_NOTE}};
score_t famima[] = {{Fs4, EIGHTH_NOTE, 0}, {D4, EIGHTH_NOTE, 0}, {A3, EIGHTH_NOTE, 0}, {D4, EIGHTH_NOTE, 0}, {E4, EIGHTH_NOTE, 0}, {A4, QUARTER_NOTE, 0}, {A3, EIGHTH_NOTE, 0}, {E4, EIGHTH_NOTE, 0}, {Fs4, EIGHTH_NOTE, 0}, {E4, EIGHTH_NOTE, 0}, {A3, EIGHTH_NOTE, 0}, {D4, QUARTER_NOTE, DOTTED_HALF_NOTE}};
score_t machinami_haruka[] = {{Cs5, QUARTER_NOTE, 0}, {A4, QUARTER_NOTE, 0}, {Cs5, HALF_NOTE, 0}, {Gs5, HALF_NOTE, 0}, {Fs5, DOTTED_HALF_NOTE, 0}, {B5, QUARTER_NOTE - 10, 10}, {B5, DOTTED_HALF_NOTE, 0}, {A5, QUARTER_NOTE, 0}, {Gs5, QUARTER_NOTE, 0}, {Fs5, QUARTER_NOTE, 0}, {E5, QUARTER_NOTE, 0}, {Ds5, QUARTER_NOTE, 0}, {A5, HALF_NOTE, 0}, {Gs5, HALF_NOTE, 0}, {Fs5, QUARTER_NOTE, 0}, {E5, QUARTER_NOTE, 0}, {Ds5, QUARTER_NOTE, 0}, {Fs5, QUARTER_NOTE, 0}, {E5, WHOLE_NOTE, WHOLE_NOTE}};
score_t yumewokakeru[] = {{Df5, EIGHTH_NOTE - 10, 10}, {Df5, EIGHTH_NOTE - 10, 10}, {Df5, EIGHTH_NOTE, 0}, {Gf5, EIGHTH_NOTE - 10, 10}, {Gf5, EIGHTH_NOTE, 0}, {Af5, EIGHTH_NOTE, 0}, {Gf5, EIGHTH_NOTE, 0}, {Df6, DOTTED_QUARTER_NOTE, 0}, {Gf5, EIGHTH_NOTE - 10, 10}, {Gf5, QUARTER_NOTE, EIGHTH_NOTE}, {Df5, EIGHTH_NOTE, 0}, {Df5, EIGHTH_NOTE, 0}, {Gf5, QUARTER_NOTE, 0}, {Af5, EIGHTH_NOTE, 0}, {Af5, EIGHTH_NOTE, 0}, {Bf5, QUARTER_NOTE - 10, 10}, {Bf5, EIGHTH_NOTE, EIGHTH_NOTE}, {Bf5, QUARTER_NOTE, 0}, {Af5, EIGHTH_NOTE, 0}, {G5, EIGHTH_NOTE, 0}, {G5, EIGHTH_NOTE, 0}, {Af5, QUARTER_NOTE, 0}, {B5, EIGHTH_NOTE, EIGHTH_NOTE}, {D5, EIGHTH_NOTE - 10, 10}, {D5, EIGHTH_NOTE - 10, 10}, {D5, EIGHTH_NOTE, 0}, {Gf5, EIGHTH_NOTE - 10, 10}, {Gf5, EIGHTH_NOTE, 0}, {Af5, EIGHTH_NOTE, 0}, {Gf5, EIGHTH_NOTE, 0}, {Df6, DOTTED_QUARTER_NOTE, 0}, {Gf5, EIGHTH_NOTE - 10, 10}, {Gf5, QUARTER_NOTE, EIGHTH_NOTE}, {Ef5, EIGHTH_NOTE, 0}, {B5, EIGHTH_NOTE, 0}, {Bf5, QUARTER_NOTE, 0}, {Af5, EIGHTH_NOTE, 0}, {Af5, EIGHTH_NOTE, 0}, {Bf5, QUARTER_NOTE - 10, 10}, {Af5, DOTTED_QUARTER_NOTE, 0}, {Gf5, EIGHTH_NOTE - 10, 10}, {Gf5, QUARTER_NOTE, QUARTER_NOTE}};

void esp_now_callback(const esp_now_recv_info *esp_now_recieve_info, const unsigned char *recieve_data, int length) {
  char recieve_data_buffer[length];  // 受信データ格納用

  sprintf(recieve_data_buffer, "%s", recieve_data);

  if(strncmp(recieve_data_buffer, "HELLO", 5) == 0) {
    // music.play(skype, (sizeof(skype) / sizeof(score_t)), 120);
    // music.play(famima, (sizeof(famima) / sizeof(score_t)), 120);
    music.play(machinami_haruka, (sizeof(machinami_haruka) / sizeof(score_t)), 320);
    // music.play(yumewokakeru, (sizeof(yumewokakeru) / sizeof(score_t)), 195);

    send_message("END", 4);
  }
}

void setup() {
  Serial.begin(BAUDRATE);

  delay(100);

  pinMode(SPK, OUTPUT);
  setToneChannel(SPK);

  music.set_speaker_out(SPK);
  music.generate();
  music.debug_table();

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

  memcpy(peer_info.peer_addr, target, 6);
  peer_info.channel = 0;      // Wi-Fi のチャンネル選択  
  peer_info.encrypt = false;  // 暗号化の有無

  if(esp_now_add_peer(&peer_info) == ESP_OK) {
    Serial.println("Successed to add peer");
  }else {
    Serial.println("Faild to add peer");
  }

  esp_now_register_recv_cb(esp_now_callback);
}

void loop() {
}

void send_message(char msg[], int8_t count) {
  esp_now_send(target, (uint8_t *)msg, count);
}
