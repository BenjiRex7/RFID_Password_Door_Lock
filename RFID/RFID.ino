#include <SPI.h>
#include <MFRC522.h>

// RFID
#define SS_PIN 5
#define RST_PIN 0
MFRC522 rfid(SS_PIN, RST_PIN);

// LED pins
#define GREEN_LED_PIN 2
#define RED_LED_PIN 15

// Predefined authorized UID
//A4 C6 E7 07
byte authorizedUID[] = { 0xA4, 0xC6, 0xE7, 0x07 }; // Replace with your own authorized UID

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}

void loop() {

  // Look for new cards
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    Serial.print("UID tag: ");
    String content = "";
    byte letter;
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(rfid.uid.uidByte[i], HEX);
      content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(rfid.uid.uidByte[i], HEX));
    }
    Serial.println();

    // Check if the UID matches the authorized UID
    if (isAuthorized(rfid.uid.uidByte, rfid.uid.size)) {
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
      Serial.println("Authorized access");
      // Add further logic to check password from keypad here
    } else {
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
      Serial.println("Unauthorized access");
    }
    delay(1000);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);

    // Halt PICC
    rfid.PICC_HaltA();
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
  }
}

bool isAuthorized(byte *uid, byte uidSize) {
  if (uidSize != sizeof(authorizedUID)) return false;
  for (byte i = 0; i < uidSize; i++) {
    if (uid[i] != authorizedUID[i]) return false;
  }
  return true;
}
