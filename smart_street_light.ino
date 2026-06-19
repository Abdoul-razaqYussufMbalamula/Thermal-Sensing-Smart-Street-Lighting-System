/*
 Project Name: Thermal-Sensing Smart Street Lighting System (Hackathon Reconstruction)
 System Input: HC-SR501 PIR Sensor (Digital Pin 2)
 System Output: LED Array (Digital Pins 4, 5, 6, 7)
 Description: Uses non-blocking logic to illuminate street lights upon human thermal detection.
*/

// Hardware Pin Definitions
const int pirSensorPin = 2;    
const int lightPins[] = {4, 5, 6, 7}; 
const int numLights = 4;

// Timing Variables (Non-blocking)
unsigned long lightActivationTime = 0;
const unsigned long keepLightsOnDuration = 5000; // Keep lights on for 5 seconds after motion stops
bool lightsAreOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(pirSensorPin, INPUT);
  
  // Initialize all LED pins as outputs and ensure they are OFF
  for (int i = 0; i < numLights; i++) {
    pinMode(lightPins[i], OUTPUT);
    digitalWrite(lightPins[i], LOW);
  }
  
  Serial.println("System Initialized. Calibrating PIR...");
  delay(2000); // Give PIR sensor time to stabilize
}

void loop() {
  int pirState = digitalRead(pirSensorPin);
  unsigned long currentMillis = millis();

  // Condition 1: Motion is detected
  if (pirState == HIGH) {
    lightActivationTime = currentMillis; // Reset the timer
    
    if (!lightsAreOn) {
      turnLights(HIGH);
      lightsAreOn = true;
      Serial.println("Pedestrian Detected: Lights ON");
    }
  }

  // Condition 2: No motion, and the timer has expired
  if (lightsAreOn && (currentMillis - lightActivationTime >= keepLightsOnDuration)) {
    turnLights(LOW);
    lightsAreOn = false;
    Serial.println("Area Clear: Lights OFF (Power Saving Mode)");
  }
}

// Helping function to control the entire LED array
void turnLights(int state) {
  for (int i = 0; i < numLights; i++) {
    digitalWrite(lightPins[i], state);
  }
}
