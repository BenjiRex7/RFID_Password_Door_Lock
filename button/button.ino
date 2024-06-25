// Pin definitions
#define BUTTON_PIN 17
#define GREEN 
#define RED 4

void setup() {
  // Start serial connection
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
}

void loop() {
  // Read the pushbutton value into a variable
  int bSta = digitalRead(BUTTON_PIN);
  Serial.println(bSta);
  if (bSta == 0) {
    digitalWrite(GREEN, HIGH);
    delay(1000);
    digitalWrite(GREEN, LOW);
  } else {
    digitalWrite(GREEN, LOW);
  }
}
