#include <SPI.h>
#include <Wire.h>
#include <Keypad.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>

// OLED
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Keypad
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
#define BLUE_LED_PIN 2
#define GREEN_LED_PIN 15
#define RED_LED_PIN 4
#define RELAY 16
#define BUTTON_PIN 17

// RFID
#define SS_PIN 5
#define RST_PIN 0
MFRC522 rfid(SS_PIN, RST_PIN);

// Predefined authorized UID
byte authorizedUID[] = { 0xA4, 0xC6, 0xE7, 0x07 };  // Replace with your own authorized UID

const String correctPassword = "1489";
String enteredPassword = "";

bool passwordModeActive = false;

void setup() {
  Serial.begin(115200);
  // initialize the OLED object
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(RELAY, HIGH);
  SPI.begin();
  rfid.PCD_Init();
  int x = 30;
  display.setFont(&FreeMonoBoldOblique9pt7b);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(7, x - 17);
  display.println("Welcome to");
  display.setCursor(45, x);
  display.println("the");
  display.setCursor(7, x + 17);
  display.print("ASTRO LABS");
  display.display();
  delay(2000);
}

void loop() {
  char key = keypad.getKey();
  int bSta = digitalRead(BUTTON_PIN);
  // Serial.println(bSta);
  if (bSta == 0) {
    digitalWrite(RELAY, LOW);
    delay(2000);
    digitalWrite(RELAY, HIGH);
  } else {
    digitalWrite(RELAY, HIGH);
  }
  if (!passwordModeActive) {
    display.setFont(&FreeMonoBoldOblique9pt7b);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(13, 37);
    display.println("SCAN RFID");
    display.display();
    // RFID mode
    // Look for new cards
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      // RFID card detected
      if (isAuthorized(rfid.uid.uidByte, rfid.uid.size)) {
        Welcome();
        // Authorized access
        Serial.println("Authorized access");
      } else {
        // Unauthorized access
        digitalWrite(BLUE_LED_PIN, LOW);   // Turn off blue LED
        digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green LED
        digitalWrite(RED_LED_PIN, HIGH);   // Turn on red LED
        Serial.println("Unauthorized access");
      }
      delay(1000);
      digitalWrite(BLUE_LED_PIN, LOW);   // Turn off blue LED
      digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green LED
      digitalWrite(RED_LED_PIN, LOW);    // Turn off red LED
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
      int centerX = (SCREEN_WIDTH - 3) / 2;  // 3 accounts for the width of each underscore
      display.setFont();                     // Set default font
      display.clearDisplay();
      display.setCursor(22, 12);
      display.setTextSize(1);
      display.print("Enter Password");
      display.setTextSize(2);
      display.setCursor(centerX - 40, 40);
      display.println("_ ");  // print char at said cursor and show *
      display.setCursor(centerX - 15, 40);
      display.println("_ ");  // print char at said cursor and show *
      display.setCursor(centerX + 15, 40);
      display.println("_ ");  // print char at said cursor and show *
      display.setCursor(centerX + 40, 40);
      display.println("_ ");  // print char at said cursor and show *
      display.display();
      // Blink RGB LED to indicate password mode activation
      blinkRGB(1, 500);  // Blink once
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
        blinkRGB(3, 250);  // Blink 3 times
      } else if (key == 'E') {
        // 'E' key pressed, clear the entered password
        enteredPassword = "";
        Serial.println("Password cleared");
        // Clear the display and show the underscores again
        int centerX = (SCREEN_WIDTH - 3) / 2;
        display.setFont();  // Set default font
        display.clearDisplay();
        display.setCursor(22, 12);
        display.setTextSize(1);
        display.print("Enter Password");
        display.setTextSize(2);
        display.setCursor(centerX - 40, 40);
        display.println("_ ");
        display.setCursor(centerX - 15, 40);
        display.println("_ ");
        display.setCursor(centerX + 15, 40);
        display.println("_ ");
        display.setCursor(centerX + 40, 40);
        display.println("_ ");
        display.display();
      } else {
        // Append pressed key to entered password
        enteredPassword += key;
        // Check if the entered password length matches the correct password length
        if (enteredPassword.length() == 1) {
          OneStar();
        }
        if (enteredPassword.length() == 2) {
          TwoStar();
        }
        if (enteredPassword.length() == 3) {
          ThreeStar();
        }
        if (enteredPassword.length() == correctPassword.length()) {
          FourStar();
          delay(350);
          // Compare entered password with correct password
          if (enteredPassword == correctPassword) {
            CorrectPass();
            Serial.println("Access Granted");
            passwordModeActive = false;
          } else {
            IncorrectPass();
            passwordModeActive = false;
          }
          // Reset entered password after 2 seconds
          delay(2000);
          digitalWrite(BLUE_LED_PIN, LOW);   // Turn off blue LED
          digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green LED
          digitalWrite(RED_LED_PIN, LOW);    // Turn off red LED
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
void blinkRGB(int times, int duration) {
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

void OneStar() {
  int centerX = (SCREEN_WIDTH - 3) / 2;  // 3 accounts for the width of each underscore
  display.setFont();
  display.setTextSize(3);
  display.setCursor(centerX - 42, 40);
  display.println("*");  // print char at said cursor and show *
  display.display();
}

void TwoStar() {
  int centerX = (SCREEN_WIDTH - 3) / 2;  // 3 accounts for the width of each underscore
  display.setTextSize(3);
  display.setFont();
  display.setCursor(centerX - 16, 40);
  display.println("*");  // print char at said cursor and show *
  display.display();
}

void ThreeStar() {
  int centerX = (SCREEN_WIDTH - 3) / 2;  // 3 accounts for the width of each underscore
  display.setTextSize(3);
  display.setFont();
  display.setCursor(centerX + 14, 40);
  display.println("*");  // print char at said cursor and show *
  display.display();
}

void FourStar() {
  int centerX = (SCREEN_WIDTH - 3) / 2;  // 3 accounts for the width of each underscore
  display.setTextSize(3);
  display.setFont();
  display.setCursor(centerX + 38, 40);
  display.println("*");  // print char at said cursor and show *
  display.display();
}

void CorrectPass() {
  display.setFont();  // Set default font
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(25, 1);
  display.println("Correct");
  display.setCursor(20, 22);
  display.println("Password");
  display.setCursor(25, 44);
  display.println("WELCOME");
  display.display();
  // Correct password entered, grant access
  digitalWrite(RELAY, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);    // Turn off blue LED
  digitalWrite(GREEN_LED_PIN, HIGH);  // Turn on green LED
  digitalWrite(RED_LED_PIN, LOW);     // Turn off red LED
  delay(2000 );
  digitalWrite(RELAY, HIGH);
}

void IncorrectPass() {
  display.setFont();  // Set default font
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(14, 1);
  display.println("Incorrect");
  display.setCursor(20, 22);
  display.println("Password");
  display.setCursor(15, 44);
  display.println("TRY AGAIN");
  display.display();
  // Incorrect password entered, deny access
  digitalWrite(BLUE_LED_PIN, LOW);   // Turn off blue LED
  digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green LED
  digitalWrite(RED_LED_PIN, HIGH);   // Turn on red LED
  Serial.println("Access Denied");
}

void Welcome() {
  display.setFont();  // Set default font
  display.clearDisplay();
  display.setTextSize(2);
  String text = "WELCOME";
  int textWidth = text.length() * 8;  // Adjust font width as needed
  int x_ax = (SCREEN_WIDTH - textWidth) / 2;
  display.setCursor(x_ax - 13, 20);
  display.println(text);
  display.display();
  digitalWrite(RELAY, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);    // Turn off blue LED
  digitalWrite(GREEN_LED_PIN, HIGH);  // Turn on green LED
  digitalWrite(RED_LED_PIN, LOW);     // Turn off red LED
  delay(2000);
  digitalWrite(RELAY, HIGH);
}
