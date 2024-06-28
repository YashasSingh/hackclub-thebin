#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <IRremote.h>

#define STEP_PIN 15
#define DIR_PIN 14
#define stepsPerRevolution 200
#define ENABLE_PIN 13
#define IR_PIN 16

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Adafruit_MPU6050 mpu;
Servo myservo;
IRrecv irrecv(IR_PIN);
decode_results results;

int pos = 0;
int increment = 5;
bool systemRunning = true;
sensors_event_t accel_event, gyro_event, temp_event;

void setup() {
  Serial1.begin(115200);
  Serial1.println(SDA);
  Serial1.println(SCL);

  Wire.begin();

  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT, &Wire)) {
    Serial1.println("MPU6050 not connected!");
    while (1) { delay(10); }
  }
  Serial1.println("MPU6050 ready!");

  myservo.attach(20);
  Serial1.println("Servo ready");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial1.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  Serial1.println("Oled ready!");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Hello, world!"));
  display.display();

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  digitalWrite(DIR_PIN, HIGH);
  digitalWrite(ENABLE_PIN, LOW);

  irrecv.enableIRIn();
}

void loop() {
  delay(2);

  if (irrecv.decode(&results)) {
    Serial1.print("IR Code: ");
    Serial1.println(results.value, HEX);
    systemRunning = !systemRunning;
    irrecv.resume();
  }

  if (systemRunning) {
    mpu.getAccelerometerSensor()->getEvent(&accel_event);
    mpu.getGyroSensor()->getEvent(&gyro_event);
    mpu.getTemperatureSensor()->getEvent(&temp_event);

    display.clearDisplay();

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

    display.display();

    pos += increment;
    if (pos >= 180 || pos <= 0) {
      for (int i = 0; i < stepsPerRevolution; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(2000);
      }
      increment = -increment;
    }
    myservo.write(pos);
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("System Stopped");
    display.display();
  }
}
