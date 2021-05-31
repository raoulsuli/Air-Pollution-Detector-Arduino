#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

#define ledPIN 9
#define analogInput A0 // sensor pin
#define THRESHOLD 100
#define BUZZER 13

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // led initialization
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, ledPIN, NEO_GRB); // lcd initialization
uint32_t color_red = strip.Color(255, 0, 0); // warning color

void setup() 
{
  pinMode(analogInput, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  strip.begin();
  strip.show();
  strip.setBrightness(20);
  pinMode(BUZZER, OUTPUT);
}
  
void loop() {
  lcd.setCursor(0, 0);
  int gasPPM = analogRead(analogInput); // read sensor value
  lcd.clear();
  lcd.print("Gas -> ");
  lcd.print(gasPPM);
  lcd.print(" PPM");

  if (gasPPM < THRESHOLD) rainbow(gasPPM); // show rainbow if value is below threshold
  strip.clear();
  strip.show();

  if (gasPPM > THRESHOLD) { // ring buzzer and show lcd warning if above threshold
    tone(BUZZER, 1000);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color_red);
    }
    strip.show();
    delay(2000);
    noTone(BUZZER);
    strip.clear();
    strip.show();
  }
  delay(500);
}

void rainbow(int gasPPM) {
  int limit = gasPPM * strip.numPixels() / THRESHOLD;
  for(int i = 0; i < limit; i++) { // calculate each rainbow value
    int pixelHue = (i * 65536L / limit);
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    strip.show();
    delay(250);
  }
}
