#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>

Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();
// changes the speed of the text
const int START_X = ledmatrix.width();
const long DISPLAY_UPDATE_INTERVAL = 100; // milliseconds //1(display):100(message)
const long MESSAGE_UPDATE_INTERVAL = 10000;
const unsigned long BRIGHTNESS = 100; // 0-1000 effective range

String message;  // Can change while running!

void setup() {

  Serial.begin(250000);
  Serial.println("Message scrolling test");

  if (! ledmatrix.begin()) {
   Serial.println("IS31 not found");  
   while (1);  
  }  
  Serial.println("IS31 found!");  

  ledmatrix.setTextWrap(false);

}  

void loop() {

  changeMessage();
  updateDisplay();
}

void changeMessage() {

  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= MESSAGE_UPDATE_INTERVAL) {

      previousMillis = currentMillis;

      message = ""; // enter message here

      Serial.print("New message: ");
      Serial.println(message);
    }
}

void updateDisplay() {

  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  static int x = 15; // Starting point off right edge of display
  static String currentMessage = String(message);
  static int length = getTextWidth((char *)currentMessage.c_str());

  if (currentMillis - previousMillis >= DISPLAY_UPDATE_INTERVAL) {

    previousMillis = currentMillis;

    Serial.print("Displaying at x= ");
    Serial.print(x);
    Serial.print(": ");

    char* messageCharacter = (char *)currentMessage.c_str();
    int cursor = x;

    while (*messageCharacter) {

      Serial.print(*messageCharacter);

      ledmatrix.drawChar(cursor, 0, *messageCharacter++, BRIGHTNESS, 0, 1);
      cursor += 6;      
    }
    Serial.println();

    if (--x < -length) {
      x = START_X;
      currentMessage = String(message);
      length = getTextWidth((char *)currentMessage.c_str());
    }
  }
}

int getTextWidth(char *message) {

  int16_t x1, y1;
  uint16_t w, h;

  ledmatrix.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  return w;
}
