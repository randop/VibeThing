#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {8, 7, 6};


Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  Serial.println("Keypad - Press any key");

  // start safe
  // Set all possible row pins as OUTPUT one by one
  // Set all column pins as INPUT_PULLUP
  for (int p = 2; p <= 9; p++) {
    pinMode(p, INPUT_PULLUP);
  }
}

void loop() {
  for (int row = 2; row <= 9; row++) {
    pinMode(row, OUTPUT);
    digitalWrite(row, LOW);

    // Check all column candidates
    for (int col = 2; col <= 9; col++) {
      if (col == row) continue;
      pinMode(col, INPUT_PULLUP);
      if (digitalRead(col) == LOW) {
        if ((row == 5 && col == 2) || (row == 2 && col == 5)) {
          Serial.println("Pressed: #");
        } else if ((row == 6 && col == 2) || (row == 2 && col == 6)) {
          Serial.println("Pressed: 9");
        } else if ((row == 8 && col == 2) || (row == 2 && col == 8)) {
          Serial.println("Pressed: 3");
        } else if ((row == 7 && col == 2) || (row == 2 && col == 7)) {
          Serial.println("Pressed: 6");
        } else if ((row == 3 && col == 5) || (row == 5 && col == 3)) {
          Serial.println("Pressed: 0");
        } else if ((row == 3 && col == 6) || (row == 6 && col == 3)) {
          Serial.println("Pressed: 8");
        } else if ((row == 3 && col == 7) || (row == 7 && col == 3)) {
          Serial.println("Pressed: 5");
        } else if ((row == 3 && col == 8) || (row == 8 && col == 3)) {
          Serial.println("Pressed: 2");
        } else if ((row == 4 && col == 5) || (row == 5 && col == 4)) {
          Serial.println("Pressed: *");
        } else if ((row == 4 && col == 6) || (row == 6 && col == 4)) {
          Serial.println("Pressed: 7");
        } else if ((row == 4 && col == 7) || (row == 7 && col == 4)) {
          Serial.println("Pressed: 4");
        } else if ((row == 4 && col == 8) || (row == 8 && col == 4)) {
          Serial.println("Pressed: 1");
        } else {
          Serial.print("Row pin ");
          Serial.print(row);
          Serial.print(" + Col pin ");
          Serial.print(col);
          Serial.println(" pressed");
        }
        delay(300);
      }
    }

    pinMode(row, INPUT_PULLUP);  // back to safe
    delay(30);
  }
}
