#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

static const unsigned int PAYLOAD = 32;

// CE, CSN pins
RF24 radio(9, 10);

//Important: Must be similar on each modules
const byte address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

void setup() {
  Serial.begin(115200);

  delay(200);

  // ─── Very important ───
  // Mix multiple analog pins + time for better initial seed
  randomSeed(
    (analogRead(A0) << 2) ^
    (analogRead(A1) << 1) ^
    (analogRead(A2)) ^
    (millis() & 0xFFFF) ^
    micros()
  );

  delay(200);

  if (!radio.begin()) {
    Serial.println("nRF24 hardware is not responding!!");
    while (1); // halt
  }

  Serial.println("Hardware detected: nRF24L01");
  Serial.println("TRANSMITTER mode");

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(77);
  radio.setPayloadSize(32);
  radio.setAutoAck(true);
  radio.setCRCLength(RF24_CRC_16);
  radio.openWritingPipe(address);
  radio.stopListening();

  printf_begin();
  radio.printDetails();

  Serial.print("Chip connected: ");
  Serial.println(radio.isChipConnected() ? "YES" : "NO");

  Serial.print("Is valid / responding: ");
  Serial.println(radio.isValid() ? "YES" : "NO");

}

void loop() {
  char text[32];
  generateRandomHex32(text);

  bool ok = radio.write(text, PAYLOAD);  // force 32 bytes

  if (ok) {
    Serial.print("Message sent: ");
    Serial.println(text);
  } else {
    Serial.println("ERROR: message delivery");
  }

  delay(5000);
}

/**
   Right-pads the string with spaces to reach exactly targetWidth characters.
   If the original string is longer than targetWidth, it is truncated.
   Always null-terminates the result.

   @param dest       output buffer (must be large enough)
   @param destSize   size of dest buffer (including space for '\0')
   @param src        source string (can be NULL → treated as empty)
   @param targetWidth desired final visible width (spaces added on right)
*/
void rightPadSpaces(char* dest, size_t destSize, const char* src, byte targetWidth)
{
  if (destSize == 0) return;           // safety
  dest[0] = '\0';                      // always start empty

  if (targetWidth == 0) return;

  int charsWritten = 0;

  if (src && src[0] != '\0')
  {
    charsWritten = snprintf(dest, destSize, "%s", src);
    // snprintf returns what *would* have been written (excluding \0)
    // but it never writes more than destSize-1 characters + \0
  }

  // If we have no space left → we're done
  if (charsWritten >= (int)destSize - 1) return;

  int spacesNeeded = targetWidth - charsWritten;

  // No need to pad (already long enough or negative)
  if (spacesNeeded <= 0) return;

  // But we can't write more than remaining buffer space
  size_t spaceAvailable = destSize - charsWritten - 1; // -1 for \0

  if ((int)spaceAvailable < spacesNeeded)
  {
    spacesNeeded = (int)spaceAvailable;
  }

  if (spacesNeeded > 0)
  {
    memset(dest + charsWritten, ' ', spacesNeeded);
    dest[charsWritten + spacesNeeded] = '\0';
  }
}

void generateRandomHex32(char* out32) {
  static const char hexchars[] = "0123456789abcdef";

  for (uint8_t i = 0; i < 32; i += 2) {
    uint8_t value;

    // Try to mix entropy sources
    switch (i % 12) {
      case 0:  value = analogRead(A0) ^ analogRead(A1); break;
      case 2:  value = (uint8_t)(micros() >> 5);        break;
      case 4:  value = (uint8_t)(millis() >> 2);        break;
      case 6:  value = analogRead(A2) ^ i;              break;
      case 8:  value = random(0, 256);                  break;
      case 10: value = (uint8_t)(micros() & 0xFF);      break;
      default: value = random(256);
    }

    out32[i]   = hexchars[value >> 4];
    out32[i + 1] = hexchars[value & 0x0F];
  }

  out32[32] = '\0';
}
