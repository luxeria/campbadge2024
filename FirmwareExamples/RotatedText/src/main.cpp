#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 4
#define LED_COUNT 214
// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
#define HORN 0
#define EYE 1

#define LED_MATRIX_WIDTH 19
#define LED_MATRIX_HEIGHT 11
Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(LED_MATRIX_WIDTH, LED_MATRIX_HEIGHT, 
  1, 2, // rows, cols - we use 2 cols to get enough LEDs for the scarf
  LED_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE + 
// progressive vs zigzag makes no difference for a 4 arrays next to one another
    NEO_TILE_TOP + NEO_TILE_LEFT +  NEO_TILE_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800 );

void setScarfTop();
void setScarfBottom();
void setEye();
void setHorn();
void setMatrix();

void setup() {
  Serial.begin(115200);
  Serial.println("Spit Happens!");
  matrix->begin();
  matrix->setBrightness(15);
  matrix->setPixelColor(0, 255, 0, 0);
  matrix->setTextWrap(false);
  matrix->setTextSize(1);
  matrix->show();
}
int pos=0;
void loop() {
    pos++;

    matrix->clear();

    // set color for matrix first
    setMatrix();

    // set eye, horn, scarfs
    setEye();
    setHorn();
    setScarfTop();
    setScarfBottom();

    matrix->show();
    delay(50);
}

void setMatrix()
{
    int fontSize = max(int(LED_MATRIX_WIDTH/8), 1);
    int animation1Frames = 78;
    int animation2Frames = 11*8*fontSize+1;
    int animation1Length = animation1Frames;
    int animation2Length = animation2Frames/2;
    int step = pos%(animation1Length + animation2Length);

    // horizontal text
    if (step<animation1Length) {
        int cursorPos = 7-step;
        matrix->setCursor(cursorPos,2);
        matrix->setTextWrap(false);
        matrix->setTextSize(1);
        matrix->setRotation(0);
        matrix->setTextColor(matrix->Color(255,0,255));
        matrix->print("LuxCamp 2024");
    }

    // vertical text
    else {
        step -= animation1Length;
        int cursorPos = 1*8*fontSize-step;
        matrix->setCursor(cursorPos*2,LED_MATRIX_WIDTH/2-fontSize*4+1);
        matrix->setTextWrap(false);
        matrix->setTextSize(fontSize);
        matrix->setRotation(3);
        matrix->setTextColor(matrix->Color(255,0,255));
        matrix->print("Spit Happens!");
    }
}

void setEye()
{
    matrix->setPixelColor(EYE, 255,0,0);
}

void setHorn()
{
    matrix->setPixelColor(HORN, 50,100,150);   
}

void setScarfBottom()
{
    int ledsScarfBottom[7] = {16,17,18,37,170,189,190};

    for(int i : ledsScarfBottom)
    {
        matrix->setPixelColor(i,255,0,255);
    }
}

void setScarfTop()
{
    int ledsScarfTop[7] = {207,208,209,210,211,212,213};

    for(int i : ledsScarfTop)
    {
        matrix->setPixelColor(i,255,0,255);
    }
}