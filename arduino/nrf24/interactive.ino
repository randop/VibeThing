#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

static const unsigned int PAYLOAD = 32;

/********************************************************
  IMPORTANT: Must be set identical with receivers
*********************************************************/
const byte address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
static const unsigned int RF_CHANNEL = 95;
/********************************************************/

static const unsigned int PIN_NRF_CE = 9;
static const unsigned int PIN_NRF_CNS = 10;
RF24 radio(PIN_NRF_CE, PIN_NRF_CNS);

static const unsigned int TRANSMIT_INTERVAL = 3000;

#define DEBUG 0

#if DEBUG == 1
#define DBG_PRINT(...)    Serial.print(__VA_ARGS__)
#define DBG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#else
#define DBG_PRINT(...)    do {} while (0)
#define DBG_PRINTLN(...)  do {} while (0)
#endif

void setup() {
#if DEBUG == 1
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
#endif

  delay(200);

  randomSeed(
    (analogRead(A0) << 2) ^
    (analogRead(A1) << 1) ^
    (analogRead(A2)) ^
    (millis() & 0xFFFF) ^
    micros()
  );

  delay(200);

  if (!radio.begin()) {
    DBG_PRINTLN("nRF24 hardware is not responding!!");
    while (1); // halt
  }

  DBG_PRINTLN("Hardware detected: nRF24L01");
  DBG_PRINTLN("TRANSMITTER mode");

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(RF_CHANNEL);
  radio.setPayloadSize(32);
  radio.setAutoAck(true);
  radio.setCRCLength(RF24_CRC_16);
  radio.openWritingPipe(address);
  radio.stopListening();

#if DEBUG == 1
  printf_begin();
  radio.printDetails();
#endif

  DBG_PRINT("Chip connected: ");
  DBG_PRINTLN(radio.isChipConnected() ? "YES" : "NO");

  DBG_PRINT("Is valid / responding: ");
  DBG_PRINTLN(radio.isValid() ? "YES" : "NO");

}

void loop() {
  char text[32];
  generateRandomHex32(text);

  bool ok = radio.write(text, PAYLOAD);

  if (ok) {
    DBG_PRINT("Message sent: ");
    DBG_PRINTLN(text);
  } else {
    DBG_PRINTLN("ERROR: message delivery");
  }

  delay(TRANSMIT_INTERVAL);
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
  if (destSize == 0) return;
  dest[0] = '\0';

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
