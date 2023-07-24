// ekran
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
//Bosch
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor* bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor* bmp_pressure = bmp.getPressureSensor();
//Pulse sensor
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
const int PulseWire = 14; // analogni pin
int Threshold = 550; //osjetljivost
//ekran reset pin
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);
PulseSensorPlayground pulseSensor;

void setup() {
    Serial.begin(9600);
    if (!bmp.begin()) {
        Serial.println(F("Nema BMP280!!!!")); //Postoji li BMP280
        while (1);
    }
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
        Adafruit_BMP280::SAMPLING_X2,     /* Temp. preuzrokovanje */
        Adafruit_BMP280::SAMPLING_X16,    /* Pressure preuzrokovanje */
        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
        Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    bmp_temp->printSensorDetails();
    pulseSensor.analogInput(PulseWire);
    pulseSensor.setThreshold(Threshold);
    pulseSensor.begin();
}


void loop() {
    info();
    bpm();
}

void bpm() {
    int otkucaji = pulseSensor.getBeatsPerMinute(); 
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("BPM:");
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(50, 25);
    display.println(otkucaji);
    display.display();
    delay(2000);
}

void info() {
    sensors_event_t temp_event, pressure_event;
    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Temperature = ");
    display.setTextSize(2);
    display.setCursor(28, 16);
    display.print(temp_event.temperature);
    display.println("*C");
    display.display();
    delay(2500);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Nadmorska visina: ");
    display.setTextSize(2);
    display.setCursor(28, 16);
    display.print(bmp.readAltitude(1013.25));
    display.print("m");
    display.display();
    delay(2000);
}
