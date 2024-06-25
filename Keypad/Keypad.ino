#include <Keypad.h>

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

#define GREEN_LED_PIN 2
#define RED_LED_PIN 15

const String correctPassword = "1234";
String enteredPassword = "";

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == 'S') {  // 'S' is used to start over
      enteredPassword = "";
      Serial.println("Password Reset");
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
    } else {
      enteredPassword += key;

      if (enteredPassword.length() == correctPassword.length()) {
        if (enteredPassword == correctPassword) {
          digitalWrite(GREEN_LED_PIN, HIGH);
          digitalWrite(RED_LED_PIN, LOW);
          Serial.println("Access Granted");
        } else {
          digitalWrite(GREEN_LED_PIN, LOW);
          digitalWrite(RED_LED_PIN, HIGH);
          Serial.println("Access Denied");
        }
        delay(2000);  // Wait for 2 seconds before clearing the LEDs and the entered password
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        enteredPassword = "";
      }
    }
  }
}
