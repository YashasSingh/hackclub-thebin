#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>

#define STEP_PIN 15        // Stepper motor step pin
#define DIR_PIN 14         // Stepper motor direction pin
#define stepsPerRevolution 200 // Number of steps per revolution for the stepper motor
#define ENABLE_PIN 13      // Stepper motor enable pin
#define IR_PIN 16          // Infrared receiver pin

Adafruit_SSD1306 display(128, 64, &Wire, -1); // OLED display object
Adafruit_MPU6050 mpu;                         // MPU6050 sensor object
Servo myservo;                                // Servo motor object
IRrecv irrecv(IR_PIN);                        // IR receiver object
decode_results results;                       // Object to store decoded IR signals

int pos = 0;               // Servo position
int increment = 5;         // Increment value for servo position
bool systemRunning = true; // Flag to track if the system is running or stopped

// Sensor event objects for accelerometer, gyroscope, and temperature data
sensors_event_t accel_event, gyro_event, temp_event;

void setup() {
  Serial1.begin(115200); // Initialize serial communication

  Wire.begin(); // Initialize I2C communication

  // Initialize MPU6050 sensor
  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT, &Wire)) {
    while (1) { delay(10); } // Halt if MPU6050 is not connected
  }

  myservo.attach(20); // Attach servo to pin GP20

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Halt if display allocation failed
  }

  // Display initial message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Hello, world!"));
  display.display();

  // Initialize stepper motor pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  digitalWrite(DIR_PIN, HIGH);  // Set initial direction
  digitalWrite(ENABLE_PIN, LOW); // Enable stepper motor

  irrecv.enableIRIn(); // Start the IR receiver
}

void loop() {
  delay(2); // Small delay for stability

  // Check if an IR signal is received
  if (irrecv.decode(&results)) {
    systemRunning = !systemRunning; // Toggle the systemRunning flag
    irrecv.resume(); // Receive the next value
  }

  if (systemRunning) {
    // Get accelerometer, gyroscope, and temperature data
    mpu.getAccelerometerSensor()->getEvent(&accel_event);
    mpu.getGyroSensor()->getEvent(&gyro_event);
    mpu.getTemperatureSensor()->getEvent(&temp_event);

    // Clear the display buffer
    display.clearDisplay();

    // Display data in table format
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Accel    Gyro    Temp");

    display.setCursor(0, 10);
    display.print("X:");
    display.print(accel_event.acceleration.x, 1);
    display.print("     ");
    display.print(gyro_event.gyro.x, 1);

    display.setCursor(0, 20);
    display.print("Y:");
    display.print(accel_event.acceleration.y, 1);
    display.print("     ");
    display.print(gyro_event.gyro.y, 1);

    display.setCursor(0, 30);
    display.print("Z:");
    display.print(accel_event.acceleration.z, 1);
    display.print("     ");
    display.print(gyro_event.gyro.z, 1);

    display.setCursor(0, 40);
    display.print("Temp:          ");
    display.print(temp_event.temperature, 1);
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
      increment = -increment; // Reverse the direction of the servo
    }
    myservo.write(pos);
  } else {
    // Display system stopped message
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("System Stopped");
    display.display();
  }
}
