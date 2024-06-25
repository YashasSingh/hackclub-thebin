#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET    -1 // Reset pin (or -1 if sharing Arduino reset pin)





Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPU6050 mpu;
Servo myservo;

int pos = 0;
sensors_event_t event;




void setup() {
  Serial1.begin(115200);
  while (!Serial1) {
    delay(10); // Wait for Serial1 to be ready
  }
  Serial1.println("Hello, Raspberry Pi Pico W!");
  
  // Initialize I2C and MPU6050
  Wire.begin();
  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT, &Wire)) {
    Serial1.println("MPU6050 not connected!");
    while (1) {
      delay(10); // Halt if MPU6050 is not connected
    }
  }
  Serial1.println("MPU6050 ready!");

  // Initialize the servo
  myservo.attach(20);


  // Initialize the I2C interface with specific pins
  Wire.setSDA(12); // Set SDA to GP12
  Wire.setSCL(13); // Set SCL to GP13
  Wire.begin();

  // Initialize the OLED display
  if (!display.begin(SSD1306_PAGEADDR, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  // Clear the buffer
  display.clearDisplay();

  // Display initial text
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("Hello, world!"));
  display.display();
}

void loop() {
  delay(2); // Small delay for stability

  // Get accelerometer data
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
  if (pos > 180) {
    pos = 0; // Reset the position if it exceeds the limit
  }
  myservo.write(pos);


}
