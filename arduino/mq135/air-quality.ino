#define SENSOR_PIN    A0

// Connect MQ-135 DOUT on digital pin 7
#define DO_PIN        7

// Typical values – change RZERO after your own calibration!
const float RLOAD   = 10.0;     // Load resistor on board (kΩ) – usually 10 or 22
const float RZERO   = 76.63;    // ← CHANGE THIS after calibration!
// Typical fresh air values: 70–120 range

// PPM curve parameters for CO2 approximation (from common datasheets)
const float PARA    = 116.6020682;
const float PARB    = -2.769034857;

void setup() {
  Serial.begin(115200);
  Serial.println("MQ-135 Air Quality Monitor");
  Serial.println("Preheating...");
  Serial.println("Please wait at least 24-48h to complete sensor preheating.");
  pinMode(DO_PIN, INPUT);
  delay(2000);
}

void loop() {
  // Read average of 10 measurements
  int rawSum = 0;
  for (int i = 0; i < 10; i++) {
    rawSum += analogRead(SENSOR_PIN);
    float sensorValue = analogRead(A0);
    Serial.println("Sensor value:");
    Serial.println(sensorValue);
    delay(50);
  }
  float sensorValue = rawSum / 10.0;

  // Convert to voltage
  float voltage = sensorValue * (5.0 / 1023.0);

  // Calculate RS (sensor resistance)
  float RS = ((5.0 * RLOAD) / voltage) - RLOAD;

  // RS / R0 ratio
  float ratio = RS / RZERO;

  // Approximate PPM (CO2-ish curve – most popular approximation)
  float ppm = PARA * pow(ratio, PARB);

  // Simple Air Quality Index levels (approximate!)
  String quality;
  int alertLevel = 0;

  if (ppm <= 450) {
    quality = "GOOD";
    alertLevel = 0;
  } else if (ppm <= 1000) {
    quality = "Moderate / Acceptable";
    alertLevel = 1;
  } else if (ppm <= 1500) {
    quality = "POOR – ventilate!";
    alertLevel = 2;
  } else {
    quality = "VERY BAD – danger!";
    alertLevel = 3;
  }

  Serial.print("Raw: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | RS/R0: ");
  Serial.print(ratio, 3);
  Serial.print(" | PPM ≈ ");
  Serial.print(ppm, 0);
  Serial.print("  →  ");
  Serial.println(quality);

  int doState = digitalRead(DO_PIN);

  // Most common MQ-135 modules: LOW = polluted / gas detected
  // Some rare modules are inverted — swap HIGH/LOW if behavior is opposite
  if (doState == LOW) {
    Serial.println("Status: POLLUTED / BAD AIR QUALITY (gas detected)");
  } else {
    Serial.println("Status: GOOD / Clean air");
  }

  Serial.print("DOUT raw = ");
  Serial.println(doState);

  delay(2000);
}
