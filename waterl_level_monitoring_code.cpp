#define BLYNK_TEMPLATE_ID "TMPL3b8qUr_qD"
#define BLYNK_TEMPLATE_NAME "Water Level Monitoring System"
#define BLYNK_AUTH_TOKEN "9aaJa1zCutf_hLvYJA4_oteYgLS93L6i"  // Your Blynk Auth Token

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <BlynkSimpleEsp8266.h>

// Define I2C LCD and its address
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, change if needed

// Ultrasonic sensor pins
#define TRIG_PIN D6   // GPIO5
#define ECHO_PIN D5   // GPIO4

// Max distance (height of tank) in cm
#define MAX_DISTANCE 20  // Adjust based on your tank height

// NewPing library setup for ultrasonic sensor
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// Wi-Fi credentials
char auth[] = BLYNK_AUTH_TOKEN;   // Blynk Auth Token
char ssid[] = "Oppo78";   // Replace with your Wi-Fi SSID
char pass[] = "dharun321";  // Replace with your Wi-Fi password

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);
  
  // Initialize Blynk connection
  Blynk.begin(auth, ssid, pass);

  // Initialize the I2C LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Water Level:");

  // Ultrasonic sensor setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Measure distance (water level)
  unsigned int distance = sonar.ping_cm();
  
  // Calculate water level as a percentage
  int waterLevel = map(distance, MAX_DISTANCE, 0, 0, 100);

  // Display the raw cm value and the water level percentage on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println("%");

  // Update values in Blynk app
  Blynk.virtualWrite(V0, distance);  // Send cm value to V0 in Blynk
  Blynk.virtualWrite(V1, waterLevel);  // Send percentage to V1 in Blynk

  // Update LCD display
  lcd.setCursor(0, 1);  // Move to the second line of the LCD
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm");

  // Clear the first line and display water level percentage
  lcd.setCursor(0, 0);  // Move to the first line of the LCD
  lcd.print("Level: ");
  lcd.print(waterLevel);
  lcd.print("%   ");  // Extra spaces to clear old values

  // Run Blynk
  Blynk.run();

  delay(1000);  // Update every 1 second
}