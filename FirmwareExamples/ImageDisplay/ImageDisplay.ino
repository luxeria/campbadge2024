#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
AsyncWebServer server = AsyncWebServer(80);
#include <DNSServer.h>
DNSServer dnsServer;

#include <Adafruit_NeoPixel.h>

// Include the jpeg decoder library
#include <FS.h>
#include <TJpg_Decoder.h>


#define LED_PIN 4
#define LED_COUNT 214
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define EYE 1
#define HORN 0
#define SCARFSIZE 7
uint8_t ledsScarfTop[SCARFSIZE] = {207,208,209,210,211,212,213};
uint8_t ledsScarfBottom[SCARFSIZE] = {16,17,18,37,170,189,190};
#define LED_MATRIX_HEIGHT 11
#define LED_MATRIX_WIDTH 19

uint32_t hornHue = 0;

boolean eyeOpen = true;
uint32_t delayEye = 0;

#include "ScarfLinkedList.h"
ScarfLinkedlist scarfBottom;;
ScarfLinkedlist scarfTop = ScarfLinkedlist(SCARFSIZE);
uint32_t delayScarfBottom = 0;
uint32_t delayScarfTop = 0;

String imageFilename = "/image.jpg";
boolean uploading = false;
boolean updatedImage = false;

uint8_t frameRate_Hz = 20;
uint32_t delayFrame = 0;


void setup() {
    Serial.begin(115200);
    Serial.println("");
    Serial.println("");

    strip.begin();
    initLED();

    // Start AP with no password
    WiFi.mode(WIFI_AP);
    WiFi.softAP("MINIME");
    // Start captive portal
    // dnsServer.start(53, "*", WiFi.softAPIP());
    Serial.print("AP 'MINIME' started: ");
    Serial.println(WiFi.softAPIP());

    // Start web server
    server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
        request->send(200, "text/html", "<html><body><form action='/upload' method='POST' enctype='multipart/form-data'><input type='file' name='file' accept='image/*' capture='camera'><p><input type='submit'></form>");
    });
    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->redirect("/");
    }, handleUpload);
    server.onNotFound([&](AsyncWebServerRequest *request) { // Catch all
        Serial.println("Not found, redirecting to /");
        request->redirect("/");
    });
    server.begin();

    // Initialise SPIFFS
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS initialisation failed!");
        while (1) yield(); // Stay here twiddling thumbs waiting
    }
    Serial.println("SPIFFS done.");
    if (SPIFFS.exists(imageFilename)) {
        Serial.println("Image file found.");
        updatedImage = true;
    } else {
        Serial.println("No image file stored.");
    }

    // The decoder must be given the exact name of the rendering function above
    TJpgDec.setCallback(displayJpegMatrix);

    scarfBottom = ScarfLinkedlist(SCARFSIZE);
}


// uint32_t delayPrintHeap = 0;
void loop() {
    // if (millis() > delayPrintHeap) {
    //     delayPrintHeap = millis() + 3000;
    //     Serial.print("Heap: ");
    //     Serial.print(ESP.getFreeHeap());
    //     Serial.print(" / ");
    //     Serial.println(ESP.getHeapFragmentation());
    // }

    // Is just flickering anyway on the ESP8266
    if (uploading) {
        return;
    }
    
    // Captive portal, serve local IP for all DNS requests
    // dnsServer.processNextRequest();
    // Webserver is async, no need to call anything

    if (millis() > delayFrame) {
        // Timer next frame
        delayFrame = millis() + (1000 / frameRate_Hz);
        
        // strip.clear(); // Do not clear, as matrix is not updated every frame

        if (updatedImage) {
            updatedImage = false;
            displayJpgMatrix();
        }

        setHorn();
        setEye();
        setScarf();
        strip.show();
    }
}


void initLED() {
    strip.setBrightness(20);
    for(uint8_t i = 1; i < LED_COUNT; i++){
        strip.setPixelColor(i, 0, 0, 0);
    }
    strip.setPixelColor(0, 255, 0, 0);
    strip.show();
}


uint32_t watchDogBreak = 0;
void displayJpgMatrix() {
    // Get the width and height in pixels of the jpeg if you wish
    uint16_t w = 0, h = 0;
    TJpgDec.getFsJpgSize(&w, &h, imageFilename, SPIFFS);
    // Serial.print(w); Serial.print("x"); Serial.print(h); Serial.print(" -> "); Serial.print(w/8);  Serial.print("x"); Serial.println(h/8);

    w = w / 8;
    h = h / 8;

    // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
    uint8_t scaling = 1;
    if (w > LED_MATRIX_WIDTH * 8 || h > LED_MATRIX_HEIGHT * 8) {
        scaling = 8;
    } else if (w > LED_MATRIX_WIDTH * 4 || h > LED_MATRIX_HEIGHT * 4) {
        scaling = 4;
    } else if (w > LED_MATRIX_WIDTH * 2 || h > LED_MATRIX_HEIGHT * 2) {
        scaling = 2;
    }
    TJpgDec.setJpgScale(scaling);


    // TODO center image !!!


    watchDogBreak = millis() + 1000;
    TJpgDec.drawFsJpg(0, 0, imageFilename, SPIFFS);
}

bool displayJpegMatrix(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    // Function triggers ESP8266 wifi-watchdog if it takes too long
    if (millis() > watchDogBreak) {
        delay(1);
        watchDogBreak = millis() + 1000;
        Serial.println("Display watchdog triggered");
    }

    // The jpeg is a bitmap of 8x8 pixel blocks
    x = x / 8;
    y = y / 8;

    if (y > LED_MATRIX_HEIGHT - 1 || x > LED_MATRIX_WIDTH - 1) {
        // Serial.print("Row or col out of bounds:"); Serial.print(y); Serial.print(", "); Serial.println(x);
        return true;
    }

    // Extract the red, green, blue values from each pixel
    uint8_t b = uint8_t((*bitmap & 0x001F)<<3); // 5 LSB for blue
    uint8_t g = uint8_t((*bitmap & 0x07C0)>>3); // 6 'middle' bits for green
    uint8_t r = uint8_t((*bitmap++ & 0xF800)>>8); // 5 MSB for red

    // strip.setPixelColor(y * LED_MATRIX_WIDTH + x, (r / 1.2) + 20, (g / 1.2) + 20, (b / 1.2) + 20);
    strip.setPixelColor(y * LED_MATRIX_WIDTH + x, r, g, b);

    return true;
}


void setHorn() {
    // Rainbow colors
    hornHue = (hornHue + 20) % 65535;
    strip.setPixelColor(HORN, strip.gamma32(strip.ColorHSV(hornHue)));
}


void setEye() {
    // Blink eye white with random delays
    if (millis() > delayEye) {
        delayEye = millis() + (eyeOpen ? random(50, 250) : random(200, 5000));
        eyeOpen = !eyeOpen;
    }
    if (eyeOpen) {
        strip.setPixelColor(EYE, 255, 255, 255);
    } else {
        strip.setPixelColor(EYE, 0, 0, 0);
    }
}

void setScarf() {
    // Purrple scarf with wind/wave effect
    scarfBottom.resetCurrent();
    scarfTop.resetCurrent();
    for (uint8_t i = 0; i < SCARFSIZE; i++) {
        scarfBottom.nextCurrent();
        scarfTop.nextCurrent();
        strip.setPixelColor(ledsScarfBottom[i], scarfBottom.current->r, 0, scarfBottom.current->b);
        strip.setPixelColor(ledsScarfTop[i], scarfTop.current->r, 0, scarfTop.current->b);
    }
    if (millis() > delayScarfBottom) {
        delayScarfBottom = millis() + random(0, 200); // 0 delay is single frame
        (random(0, 3) ? scarfBottom.shiftAddNodeHead() : scarfBottom.shiftAddNodeTail());   
    }
    if (millis() > delayScarfTop) {
        delayScarfTop = millis() + random(0, 200); // 0 delay is single frame
        (random(0, 2) ? scarfTop.shiftAddNodeHead() : scarfTop.shiftAddNodeTail());   
    }
}


void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
        // open the file on first call and store the file handle in the request object
        request->_tempFile = SPIFFS.open(imageFilename, "w");
        Serial.println("Upload to image.jpg: " + String(filename));

        // Clear matrix
        initLED();
        uploading = true;

        watchDogBreak = millis() + 1000;
    }

    if (len) {
        // stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
        // Serial.println("Writing file: index=" + String(index) + " len=" + String(len));
    }
    
    if (final) {
        // close the file handle as the upload is now done
        request->_tempFile.close();
        Serial.println("Upload Complete: size: " + String(index + len));
        
        updatedImage = true;

        // Restart ESP as 'FIX' for this random memory leak when uploading/connecting/something
        // For some reason multiple uploads of even small files cause a spontaneous memory leak
        // Sometimes this is related with reconnecting to wifi
        delay(50);
        ESP.reset();
    }


    if (millis() > watchDogBreak) {
        delay(1);
        watchDogBreak = millis() + 1000;
        Serial.println("Upload watchdog triggered");
    }
}
