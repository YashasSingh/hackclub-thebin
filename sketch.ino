#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <IRremote.h> // Include the IRremote library

#define STEP_PIN 15
#define DIR_PIN 14
#define stepsPerRevolution 200
#define ENABLE_PIN 13
#define IR_PIN 16 // Define the IR receiver pin

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Adafruit_MPU6050 mpu;
Servo myservo;
IRrecv irrecv(IR_PIN); // Create an IR receiver object
decode_results results; // Create a results object to store the decoded IR signals

int pos = 0;
sensors_event_t event;

void setup() {
  Serial1.begin(115200);
  Serial1.println(SDA);
  Serial1.println(SCL);

  Wire.begin();

  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT, &Wire)) {
    Serial1.println("MPU6050 not connected!");
    while (1) { delay(10); } // Halt if MPU6050 is not connected
  }
  Serial1.println("MPU6050 ready!");

  myservo.attach(20); // Attach servo to pin GP20
  Serial1.println("Servo ready");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial1.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  Serial1.println("Oled ready!");

  display.clearDisplay(); // Clear the display buffer
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0); 
  display.println(F("Hello, world!"));
  display.display();

  // Initialize stepper motor pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  // Set initial direction
  digitalWrite(DIR_PIN, HIGH);
  
  digitalWrite(ENABLE_PIN, LOW);

  irrecv.enableIRIn(); // Start the IR receiver
}

void loop() {
  delay(2); // Small delay for stability

  // Get and print accelerometer data
  mpu.getAccelerometerSensor()->getEvent(&event);
  Serial1.print("[");
  Serial1.print(millis());
  Serial1.print("] X: ");
  Serial1.print(event.acceleration.x);
  Serial1.print(", Y: ");
  Serial1.print(event.acceleration.y);
  Serial1.print(", Z: ");
  Serial1.print(event.acceleration.z);
  Serial1.println(" m/s^2");

  // Update servo position
  pos += 5;
  if (pos > 180) { pos = 0; }
  myservo.write(pos);

  // Step the motor
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(2000);
    display.println(i + ("Hello, world!"));
  }

  // Check if an IR signal is received
  if (irrecv.decode(&results)) {
    Serial1.print("IR Code: ");
    Serial1.println(results.value, HEX); // Print the received IR code
    irrecv.resume(); // Receive the next value
  }
}
