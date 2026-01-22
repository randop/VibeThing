const int strobe = 8;   // STB
const int clock  = 9;   // CLK
const int data   = 10;  // DIO

void setup() {
  pinMode(strobe, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(data,   OUTPUT);

  sendCommand(0x8f);  // Activate and set brightness (0x88–0x8f, higher = brighter)
  resetDisplay();
}

void loop() {
  // Simple counter on display + LEDs chasing
  for (int i = 0; i < 100000000; i++) {  // long loop for demo
    displayNumber(i);
    setLEDs(i % 256);           // chase LEDs with low 8 bits
    delay(100);

    byte keys = readButtons();
    if (keys != 0) {
      displayHex(keys);         // show pressed buttons as hex
      delay(500);
    }
  }
}

void sendCommand(byte value) {
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

void resetDisplay() {
  sendCommand(0x40);           // set auto-increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);  // start address
  for (int i = 0; i < 16; i++) {
    shiftOut(data, clock, LSBFIRST, 0x00);  // clear all
  }
  digitalWrite(strobe, HIGH);
}

// Display an 8-digit number (right-aligned, leading zeros)
void displayNumber(unsigned long num) {
  const byte digits[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
  sendCommand(0x44);  // fixed address mode
  for (int pos = 0; pos < 8; pos++) {
    digitalWrite(strobe, LOW);
    shiftOut(data, clock, LSBFIRST, 0xc0 + (pos * 2));  // even addresses = segments
    byte val = (num > 0) ? digits[num % 10] : 0;
    shiftOut(data, clock, LSBFIRST, val);
    digitalWrite(strobe, HIGH);
    num /= 10;
  }
}

// Turn on/off the 8 LEDs (bit 0 = first LED, bit 7 = last LED)
void setLEDs(byte value) {
  sendCommand(0x44);
  for (int i = 0; i < 8; i++) {
    digitalWrite(strobe, LOW);
    shiftOut(data, clock, LSBFIRST, 0xc1 + (i * 2));  // odd addresses = LEDs
    shiftOut(data, clock, LSBFIRST, (value & (1 << i)) ? 1 : 0);
    digitalWrite(strobe, HIGH);
  }
}

// Read button states (returns byte, bit 0 = button S1, etc.)
byte readButtons() {
  byte buttons = 0;
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0x42);  // read buttons command
  pinMode(data, INPUT);
  for (byte i = 0; i < 4; i++) {
    buttons |= (shiftIn(data, clock, LSBFIRST) << i);
  }
  pinMode(data, OUTPUT);
  digitalWrite(strobe, HIGH);
  return buttons;
}

void displayHex(byte val) {
  const byte hexDigits[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
  sendCommand(0x44);
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);
  shiftOut(data, clock, LSBFIRST, hexDigits[val >> 4]);
  shiftOut(data, clock, LSBFIRST, 0xc2);
  shiftOut(data, clock, LSBFIRST, hexDigits[val & 0x0F]);
  digitalWrite(strobe, HIGH);
}
