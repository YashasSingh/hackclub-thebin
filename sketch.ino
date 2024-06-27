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
int increment = 5; // Variable to track servo position change direction
sensors_event_t accel_event, gyro_event, temp_event;

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

  // Get accelerometer, gyroscope, and temperature data
  mpu.getAccelerometerSensor()->getEvent(&accel_event);
  mpu.getGyroSensor()->getEvent(&gyro_event);
  mpu.getTemperatureSensor()->getEvent(&temp_event);

  // Clear the display buffer
  display.clearDisplay();

  // Display accelerometer data
  display.setCursor(0, 0);
  display.print("Accel:");
  display.setCursor(0, 10);
  display.print("X: ");
  display.print(accel_event.acceleration.x);
  display.print(" m/s^2");
  display.setCursor(0, 20);
  display.print("Y: ");
  display.print(accel_event.acceleration.y);
  display.print(" m/s^2");
  display.setCursor(0, 30);
  display.print("Z: ");
  display.print(accel_event.acceleration.z);
  display.print(" m/s^2");

  // Display gyroscope data
  display.setCursor(0, 40);
  display.print("Gyro:");
  display.setCursor(0, 50);
  display.print("X: ");
  display.print(gyro_event.gyro.x);
  display.print(" rad/s");
  display.setCursor(0, 60);
  display.print("Y: ");
  display.print(gyro_event.gyro.y);
  display.print(" rad/s");
  display.setCursor(0, 70);
  display.print("Z: ");
  display.print(gyro_event.gyro.z);
  display.print(" rad/s");

  // Display temperature data
  display.setCursor(0, 80);
  display.print("Temp:");
  display.print(temp_event.temperature);
  display.print(" C");

  display.display(); // Update the display with the new data

  // Update servo position
  pos += increment;
  if (pos >= 180 || pos <= 0) {
    // Trigger the motor for a full revolution
    for (int i = 0; i < stepsPerRevolution; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(2000);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(2000);
    }

    // Reverse the direction of the servo
    increment = -increment;
  }
  myservo.write(pos);

  // Check if an IR signal is received
  if (irrecv.decode(&results)) {
    Serial1.print("IR Code: ");
    Serial1.println(results.value, HEX); // Print the received IR code
    irrecv.resume(); // Receive the next value
  }
}
