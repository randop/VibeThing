// TM1638 interactive project
// Uses maxint-rd/TM16xx library (https://github.com/maxint-rd/TM16xx)

#include <TM1638.h>
#include <TM16xxButtons.h>

const int PIN_STB = 8;
const int PIN_CLK = 9;
const int PIN_DIO = 10;

TM1638 module(PIN_DIO, PIN_CLK, PIN_STB);
TM16xxButtons buttons(&module);

unsigned long startMillis = 0;
bool running = false;
uint8_t lastButtons = 0;

unsigned long theNumber = 0;

unsigned long previousMillis = 0;
const unsigned long interval = 1000;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_STB, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_DIO, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);

  digitalWrite(PIN_STB, HIGH);
  digitalWrite(PIN_CLK, HIGH);

  // Initialize display: on + medium brightness (0-7)
  module.setupDisplay(true, 4);

  // Optional: clear everything at start
  module.clearDisplay();

  // Turn all LEDs off
  module.setLEDs(0);

  // Attach the button callback functions
  buttons.attachRelease(fnRelease);
  buttons.attachClick(fnClick);
  buttons.attachDoubleClick(fnDoubleclick);
  buttons.attachLongPressStart(fnLongPressStart);
  buttons.attachLongPressStop(fnLongPressStop);
  buttons.attachDuringLongPress(fnLongPress);

  Serial.println(F("Setup done"));

  delay(1000);

  module.clearDisplay();
  module.setDisplayToString("READY", 0, 3);
  delay(2000);

  module.clearDisplay();
  module.setDisplayToString("00000000", 0, 0);
  delay(1000);

  unsigned long displayNum = 12345;

  module.clearDisplay();
  module.setDisplayToDecNumber(displayNum, 0, false);

  delay(2000);

  module.clearDisplay();
  uint8_t dots = 0b11111111;
  module.setDisplayToDecNumber(displayNum, dots, false);

  delay(2000);

  module.clearDisplay();
  module.setDisplayToString("    8888", dots, 0);

  delay(2000);

  module.clearDisplay();
  module.setDisplayToString("8888    ", dots, 0);

  delay(2000);

  module.clearDisplay();
  module.setDisplayToString("88888888", dots, 0);

  delay(2000);
}

void loop() {
  static unsigned long ulTime = millis();
  uint32_t dwButtons = buttons.tick();
  if (dwButtons) {
    Serial.println(dwButtons);
    Serial.println(dwButtons, HEX);
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    uint8_t dots = randomDots();
    module.clearDisplay();

    /**
      //setDisplayToDecNumber() is broken, AVOID
      //module.setDisplayToDecNumber(theNumber, dots, false);
    **/

    String s = padLeft(String(theNumber), 8);
    const char* str = s.c_str();
    module.setDisplayToString(str, dots, 0);

    theNumber++;
    if (theNumber >= 100000000) {
      theNumber = 0;
    }
  }

}

//
// Button callback functions
//

// The Release function will be called when a button was released.
// It can be used for fast actions when no click or double click needs to be detected.
void fnRelease(byte nButton)
{
  // using isPressed or is LongPressed a shift-key can be implemented
  if (buttons.isLongPressed(0)) {
    Serial.print(F("Button 0 still longpressed. "));
  } else if (buttons.isPressed(0)) {
    Serial.print(F("Button 0 still pressed. "));
  }

  Serial.print(F("Button "));
  Serial.print(nButton);

  Serial.println(F(" release."));
} // release


// This function will be called when a button was pressed 1 time (without a second press).
void fnClick(byte nButton)
{
  Serial.print(F("Button "));
  Serial.print(nButton);
  Serial.println(F(" click."));

  String s = String(nButton);
  String s1 = "b" + s;
  const char* str = s1.c_str();
  module.clearDisplay();
  module.setDisplayToString(str, 0, 0);

} // click


// This function will be called when a button was pressed 2 times in a short timeframe.
void fnDoubleclick(byte nButton)
{
  Serial.print(F("Button "));
  Serial.print(nButton);
  Serial.println(F(" doubleclick."));
} // doubleclick


// This function will be called once, when a button is pressed for a long time.
void fnLongPressStart(byte nButton)
{
  Serial.print(F("Button "));
  Serial.print(nButton);
  Serial.println(F(" longPress start"));
} // longPressStart


// This function will be called often, while a button is pressed for a long time.
void fnLongPress(byte nButton)
{
  Serial.print(F("Button "));
  Serial.print(nButton);
  Serial.println(F(" longPress..."));
} // longPress


// This function will be called once, when a button is released after beeing pressed for a long time.
void fnLongPressStop(byte nButton)
{
  Serial.print(F("Button "));
  Serial.print(nButton);
  Serial.println(F(" longPress stop"));
} // longPressStop


void sendCommand(byte value) {
  digitalWrite(PIN_STB, LOW);
  shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, value);
  digitalWrite(PIN_STB, HIGH);
}

// Turn on/off the 8 LEDs (bit 0 = first LED, bit 7 = last LED)
void setLEDs(byte value) {
  sendCommand(0x44);
  for (int i = 0; i < 8; i++) {
    digitalWrite(PIN_STB, LOW);
    shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, 0xc1 + (i * 2));  // odd addresses = LEDs
    shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, (value & (1 << i)) ? 1 : 0);
    digitalWrite(PIN_STB, HIGH);
  }
}

void setAllLEDs(bool on) {
  sendCommand(0x40);  // auto-increment
  digitalWrite(PIN_STB, LOW);
  shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, 0xC1);  // start at LED1 address
  for (int i = 0; i < 8; i++) {
    shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, on ? 0x01 : 0x00);
    shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, 0x00);  // dummy byte for segment
  }
  digitalWrite(PIN_STB, HIGH);
}

void setLEDsAutoIncrement(uint8_t ledPattern) {   // bit 0 = LED1 (leftmost?), bit 7 = LED8
  sendCommand(0x40);                      // auto-increment mode
  digitalWrite(PIN_STB, LOW);
  shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, 0xC1);   // start at first LED address

  for (uint8_t i = 0; i < 8; i++) {
    uint8_t value = (ledPattern & (1 << i)) ? 0x01 : 0x00;
    shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, value);
    // skip dummy byte for segments (TM1638 expects pairs)
    shiftOut(PIN_DIO, PIN_CLK, LSBFIRST, 0x00);   // or whatever was there before
  }
  digitalWrite(PIN_STB, HIGH);
}

// Returns a random pattern with 1 to 8 dots lit
uint8_t randomDots() {
  int count = random(1, 9);           // 1 to 8 dots
  uint8_t pattern = 0;

  // Fisher-Yates like shuffle – pick unique positions
  uint8_t positions[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  for (int i = 7; i > 0; i--) {
    int j = random(0, i + 1);
    uint8_t temp = positions[i];
    positions[i] = positions[j];
    positions[j] = temp;
  }

  // Set the first 'count' positions
  for (int i = 0; i < count; i++) {
    pattern |= (1 << positions[i]);
  }

  return pattern;
}

/**
   Pads the given string with spaces on the LEFT until it reaches the desired length.
   If the string is already longer than target length, returns it unchanged.

   @param s       the input string to pad
   @param length  the target total length
   @return        new String padded with spaces on the left
*/
String padLeft(String s, int length) {
  int currentLength = s.length();

  if (currentLength >= length) {
    return s;
  }

  int spaces = length - currentLength;

  // Most memory-friendly way on Arduino
  String padding = "";
  padding.reserve(spaces);

  for (int i = 0; i < spaces; i++) {
    padding += ' ';
  }

  return padding + s;
}
