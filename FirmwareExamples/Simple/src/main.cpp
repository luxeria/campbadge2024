#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 4
#define LED_COUNT 214
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define HORN 0
#define EYE 1

void setMatrix();
void setHorn();
void setEye();
void setScarf();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello World!");
  strip.begin();
  strip.setBrightness(15);
  strip.setPixelColor(0, 255, 0, 0);
  strip.show();
}

int pos=0;
void loop() {
  // put your main code here, to run repeatedly:
  pos++;

  setMatrix();  // set all LEDs to rainbow colors
  setHorn();    // set horn color to yellow blinking
  setEye();     // set eye color to red blinking
  setScarf();   // set scarf color to purple blinking
  
  strip.show();
  delay(10);
}

void setMatrix()
{
  for(int i=1;i<LED_COUNT;i++){
    int hue = ((pos+i)*200) % 65535;
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue)));
  }
}

void setHorn()
{
  // set horn color to yellow blinking
  int intensity = (sin(pos/30.0)+1)*117+20.0;
  strip.setPixelColor(HORN,intensity,intensity,0);
}

void setEye()
{
  // set eye color to red blinking
  strip.setPixelColor(EYE,(pos*4)%255,0,0);
}

void setScarf()
{
  // set scarf color to purple blinking
  int ledsScarf[14] = {16,17,18,37,170,189,190,207,208,209,210,211,212,213};
  int intensity = (sin(pos/50.0)+1)*107+40.0;
  for (int i : ledsScarf)
  {
    strip.setPixelColor(i,intensity,0,intensity);
  }
}