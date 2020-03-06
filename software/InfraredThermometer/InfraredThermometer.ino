#include <SPI.h>
#include <Wire.h>

// Third-party libraries:
#include <Adafruit_GFX.h>       // Adafruit GFX Library by Adafruit.
#include <Adafruit_SSD1306.h>   // Adafruit SSD1306 Wemos Mini OLED by Adafruit + mcauser
#include <Adafruit_MLX90614.h>  // Adafruit MLX90614 by Adafruit.

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  initSerial();
  initDisplay();
  initSensor();

  displayBootMessage();
  delay(1500);
}

void loop() {
  double ambientTempC = mlx.readAmbientTempC();
  double targetTempC = mlx.readObjectTempC();
  float voltage = readBatteryVoltage();
  
  Serial.print("Ambient = "); 
  Serial.print(ambientTempC); 
  Serial.print("°C\tTarget = "); 
  Serial.print(targetTempC); 
  Serial.println("°C");

  displayMeasuredData(ambientTempC, targetTempC, voltage);

  delay(1000);
}


void initSerial() {
  Serial.begin(74880); // 74880 is the speed used to print boot messages.
  Serial.println();
  Serial.println(F("Initializing serial connection DONE."));
}

void initDisplay() {
  Serial.print(F("Initializing the display..."));
  // By default, we'll generate the high voltage from the 3.3v line internally.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48).
  Serial.println(F("DONE."));
}

void initSensor() {
  Serial.print(F("Initializing the sensor..."));
  mlx.begin();
  Serial.println(F("DONE."));
}


void displayBootMessage() {
  // The display buffer is intialized with an Adafruit logo
  // internally, so first let's delete that.
  display.clearDisplay();
  
  display.setTextSize(1);  // Size 1 means each character is 6 width and 8 high.
  display.setTextColor(WHITE);
  display.setCursor(10,25);

  display.println("Welcome!");
  display.display();  
}

void displayMeasuredData(double ambientTempC, double targetTempC, float voltage) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.println("Ambient:");
  display.print("  ");
  display.print(ambientTempC);
  display.println(" C");
  display.println("Target:");  
  display.print("  ");
  display.print(targetTempC);
  display.println(" C");
  display.println("Battery:");  
  display.print("  ");
  display.print(voltage);
  display.println(" V");

  display.display();
  delay(1);  
}

float readBatteryVoltage() {
  // Voltage divider R1 = 220k+100k+220k =540k and R2=100k
  float calib_factor = 5.28; // change this value to calibrate the battery voltage
  unsigned long raw = analogRead(A0);
  float volt = raw * calib_factor/1024; 
  return volt;
}
