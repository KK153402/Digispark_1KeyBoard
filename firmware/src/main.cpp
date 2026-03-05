#include <DigiKeyboard.h>

// PB2 = デジタルピン2, 外部プルアップ（押下時 LOW）
#define BUTTON_PIN 2

bool prevState = HIGH;

void setup() {
  pinMode(BUTTON_PIN, INPUT);  // 外部プルアップのため INPUT_PULLUP は不要
}

void loop() {
  // USB接続を維持するために毎ループ必ず呼ぶ
  DigiKeyboard.update();

  bool currentState = digitalRead(BUTTON_PIN);

  // 立下りエッジ（HIGH→LOW）でEnter送信
  if (prevState == HIGH && currentState == LOW) {
    DigiKeyboard.sendKeyStroke(KEY_ENTER);
  }

  prevState = currentState;

  // チャタリング防止
  DigiKeyboard.delay(10);
}
