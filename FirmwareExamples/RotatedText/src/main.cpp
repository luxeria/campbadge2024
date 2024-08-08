#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>



#define LED_BLACK		0

#define LED_RED_VERYLOW 	(3 <<  11)
#define LED_RED_LOW 		(7 <<  11)
#define LED_RED_MEDIUM 		(15 << 11)
#define LED_RED_HIGH 		(31 << 11)

#define LED_GREEN_VERYLOW	(1 <<  5)   
#define LED_GREEN_LOW 		(15 << 5)  
#define LED_GREEN_MEDIUM 	(31 << 5)  
#define LED_GREEN_HIGH 		(63 << 5)  

#define LED_BLUE_VERYLOW	3
#define LED_BLUE_LOW 		7
#define LED_BLUE_MEDIUM 	15
#define LED_BLUE_HIGH 		31

#define LED_ORANGE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW		(LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW	(LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW		(LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM	(LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH		(LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW	(LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW		(LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM		(LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH		(LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW		(LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)






#define LED_PIN 4
#define LED_COUNT 214
// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
#define HORN 0
#define EYE 1

Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(19, 11, 
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
int offset=0;
void loop() {
    // matrix->clear();

    offset+=200;

    // set color for matrix first
    setMatrix();

    // set eye, horn, scarfs
    setEye();
    setHorn();
    setScarfTop();
    setScarfBottom();

    matrix->show();
    delay(10);
}

void setMatrix()
{
    int mw = 19;
    int mh = 11;
    // for(int i=1;i<LED_COUNT;i++){
        // int hue = (offset+i*200) % 65535;
        // matrix->setPixelColor(i, matrix§->gamma32(matrix->ColorHSV(hue)));
    // }
    uint8_t size = max(int(mw/8), 1);
    matrix->clear();
    matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
    matrix->setTextSize(1);
    matrix->setRotation(0);
    for (int8_t x=7; x>=-70; x--) {
        matrix->clear();
        matrix->setCursor(x,2);
        matrix->setTextColor(LED_PURPLE_HIGH);
        matrix->print("LuxCamp 2024");
        setEye();
        setHorn();
        setScarfBottom();
        setScarfTop();
        matrix->show();
        delay(50);
    }

    matrix->setRotation(3);
    matrix->setTextSize(size);
    matrix->setTextColor(LED_PURPLE_HIGH);
    for (int16_t x=8*size; x>=-9*8*size; x--) {
        matrix->clear();
        matrix->setCursor(x,mw/2-size*4+1);
        matrix->print("Spit Happens!");
        setEye();
        setHorn();
        setScarfBottom();
        setScarfTop();
        matrix->show();
	// note that on a big array the refresh rate from show() will be slow enough that
	// the delay become irrelevant. This is already true on a 32x32 array.
        delay(50/size);
    }
    matrix->setRotation(0);
    matrix->setCursor(0,0);
    matrix->show();
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