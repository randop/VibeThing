#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// CE, CSN pins
RF24 radio(9, 10);

//Important: Must be similar on each modules
const byte address[6] = "42069";

void setup() {
  Serial.begin(115200);
  if (!radio.begin()) {
    Serial.println("nRF24 hardware is not responding!!");
    while (1);
  }

  Serial.println("Hardware detected: nRF24L01");
  Serial.println("TRANSMITTER mode");

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(115);

  //Initialization
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  const char text[] = "Hello";

  bool ok = radio.write(&text, sizeof(text));

  if (ok) {
    Serial.print("Message sent: ");
    Serial.println(text);
  } else {
    Serial.println("ERROR: message delivery failed");
  }

  delay(1200);
}
