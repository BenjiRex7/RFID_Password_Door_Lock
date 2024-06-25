#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>

#define ROW_NUM 4     // four rows
#define COLUMN_NUM 4  // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { 'S', '0', 'E', 'D' }
};

byte pin_rows[ROW_NUM] = { 13, 12, 14, 27 };       // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = { 26, 25, 33, 32 };  // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// RGB LED pins
#define BLUE_LED_PIN 15
#define GREEN_LED_PIN 2
#define RED_LED_PIN 4

// RFID
#define SS_PIN 5
#define RST_PIN 0
MFRC522 rfid(SS_PIN, RST_PIN);

// Predefined authorized UID
byte authorizedUID[] = { 0xA4, 0xC6, 0xE7, 0x07 };  // Replace with your own authorized UID

const String correctPassword = "1234";
String enteredPassword = "";

bool passwordModeActive = false;

void setup() {
  Serial.begin(115200);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  char key = keypad.getKey();

  if (!passwordModeActive) {
    // RFID mode
    // Look for new cards
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      // RFID card detected
      if (isAuthorized(rfid.uid.uidByte, rfid.uid.size)) {
        // Authorized access
        digitalWrite(BLUE_LED_PIN, LOW);  // Turn off blue LED
        digitalWrite(GREEN_LED_PIN, HIGH); // Turn on green LED
        digitalWrite(RED_LED_PIN, LOW);    // Turn off red LED
        Serial.println("Authorized access");
      } else {
        // Unauthorized access
        digitalWrite(BLUE_LED_PIN, LOW);   // Turn off blue LED
        digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green LED
        digitalWrite(RED_LED_PIN, HIGH);   // Turn on red LED
        Serial.println("Unauthorized access");
      }
      delay(1000);
      digitalWrite(BLUE_LED_PIN, LOW);    // Turn off blue LED
      digitalWrite(GREEN_LED_PIN, LOW);   // Turn off green LED
      digitalWrite(RED_LED_PIN, LOW);     // Turn off red LED
      // Halt PICC
      rfid.PICC_HaltA();
      // Stop encryption on PCD
      rfid.PCD_StopCrypto1();
    }

    // Check if 'S' key is pressed to activate password mode
    if (key == 'S') {
      passwordModeActive = true;
      enteredPassword = "";
      Serial.println("Password mode activated");
      // Blink RGB LED to indicate password mode activation
      blinkRGB(1,500); // Blink 5 times
    }
  } else {
    // Password mode
    // Handle keypad input for password entry
    if (key) {
      // A key is pressed
      Serial.println(key);
      if (key == 'S') {
        // 'S' key pressed again, deactivate password mode
        passwordModeActive = false;
        enteredPassword = "";
        Serial.println("Password mode deactivated");
        // Blink RGB LED to indicate password mode deactivation
        blinkRGB(3,250); // Blink 5 times
      } else {
        // Append pressed key to entered password
        enteredPassword += key;
        // Check if the entered password length matches the correct password length
        if (enteredPassword.length() == correctPassword.length()) {
          // Compare entered password with correct password
          if (enteredPassword == correctPassword) {
            // Correct password entered, grant access
            digitalWrite(BLUE_LED_PIN, LOW);  // Turn off blue LED
            digitalWrite(GREEN_LED_PIN, HIGH); // Turn on green LED
            digitalWrite(RED_LED_PIN, LOW);    // Turn off red LED
            Serial.println("Access Granted");
            passwordModeActive = false;
          } else {
            // Incorrect password entered, deny access
            digitalWrite(BLUE_LED_PIN, LOW);   // Turn off blue LED
            digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green LED
            digitalWrite(RED_LED_PIN, HIGH);   // Turn on red LED
            Serial.println("Access Denied");
            passwordModeActive = false;
          }
          // Reset entered password after 2 seconds
          delay(2000);
          digitalWrite(BLUE_LED_PIN, LOW);    // Turn off blue LED
          digitalWrite(GREEN_LED_PIN, LOW);   // Turn off green LED
          digitalWrite(RED_LED_PIN, LOW);     // Turn off red LED
          enteredPassword = "";
        }
      }
    }
  }
}

bool isAuthorized(byte *uid, byte uidSize) {
  if (uidSize != sizeof(authorizedUID)) return false;
  for (byte i = 0; i < uidSize; i++) {
    if (uid[i] != authorizedUID[i]) return false;
  }
  return true;
}

// Function to blink RGB LED multiple times
void blinkRGB(int times,int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(duration);
    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    delay(duration);
  }
}
