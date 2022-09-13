#define BUZZER_PIN 8
#define BUZZER_CHANNEL 0

void setup() {}

void loop() {
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcDetachPin(BUZZER_PIN);
}
