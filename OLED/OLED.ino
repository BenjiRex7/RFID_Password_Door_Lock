
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(115200);

  // initialize the OLED object
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  // int x = 30;
  // display.setFont(&FreeMonoBoldOblique9pt7b);
  // display.clearDisplay();
  // display.setTextSize(1);
  display.setTextColor(WHITE);
  // display.setCursor(7, x - 17);
  // display.println("Welcome to");
  // display.setCursor(45, x);
  // display.println("the");
  // display.setCursor(7, x + 17);
  // display.print("ASTRO LABS");
  // display.display();
  // delay(1000);


  display.clearDisplay();
  display.setTextSize(2);
  String text = "WELCOME";
  int textWidth = text.length() * 8;  // Adjust font width as needed
  int x_ax = (SCREEN_WIDTH - textWidth) / 2;
  display.setCursor(x_ax - 13, 20);
  display.println(text);
  display.display();

}

void loop() {
  // int centerX = (SCREEN_WIDTH - 3) / 2;  // 3 accounts for the width of each underscore
  // display.setFont();                     // Set default font
  // display.clearDisplay();
  // display.setCursor(22, 12);
  // display.setTextSize(1);
  // display.print("Enter Password");
  // display.setTextSize(2);
  // display.setCursor(centerX - 40, 40);
  // display.println("_ ");  // print char at said cursor and show *
  // display.setCursor(centerX - 15, 40);
  // display.println("_ ");  // print char at said cursor and show *
  // display.setCursor(centerX + 15, 40);
  // display.println("_ ");  // print char at said cursor and show *
  // display.setCursor(centerX + 40, 40);
  // display.println("_ ");  // print char at said cursor and show *
  // // display.setCursor(89, 40);
  // // display.println("_ ");  // print char at said cursor and show *
  // display.display();
  // delay(500);

  // display.setTextSize(3);
  // display.setCursor(centerX - 42, 40);
  // display.println("*");  // print char at said cursor and show *
  // display.display();
  // delay(500);
  // display.setCursor(centerX - 16, 40);
  // display.println("*");  // print char at said cursor and show *
  // display.display();
  // delay(500);
  // display.setCursor(centerX + 14, 40);
  // display.println("*");  // print char at said cursor and show *
  // display.display();
  // delay(500);
  // display.setCursor(centerX + 38 , 40);
  // display.println("*");  // print char at said cursor and show *
  // display.display();
  // delay(500);

  // //Incorrect Password 2
  // display.clearDisplay();
  // display.setTextSize(2);
  // display.setCursor(14, 1);
  // display.println("Incorrect");
  // display.setCursor(20, 22);
  // display.println("Password");
  // display.setCursor(15, 44);
  // display.println("TRY AGAIN");
  // display.display();
  // delay(1000);

  // //Correct Password 2
  // display.clearDisplay();
  // display.setTextSize(2);
  // display.setCursor(25, 1);
  // display.println("Correct");
  // display.setCursor(20, 22);
  // display.println("Password");
  // display.setCursor(25, 44);
  // display.println("WELCOME");
  // display.display();
  // delay(1000);
}
