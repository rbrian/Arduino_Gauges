#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <spi4teensy3>
#include "Adafruit_ILI9340.h"
#include "Adafruit_ILI9341.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735.h>
#include <Wire.h>
#include <Gauges.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/digital7italic12pt7b.h>
#include <Fonts/digital7italic18pt7b.h>
#include <Fonts/digital7monoitalic12pt7b.h>
#include <Fonts/digital7monoitalic18pt7b.h>

#define _sclk 13
#define _miso 12

#define _mosi 11
#define _cs   10
#define _dc   9
#define _rst  8
#define _led  6
#define _cs1  15
#define _rst1 21
#define _dc1  20

// Common 16-bit colors:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


//Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs1, _dc1, _rst1);
//Adafruit_ST7735  tft  =  Adafruit_ST7735(_cs, _dc, _rst);
Adafruit_ILI9341 tft1 = Adafruit_ILI9341(_cs,  _dc, _rst);

//Adafruit_SSD1306 tft1 = Adafruit_SSD1306();
//Adafruit_ILI9340 tft  = Adafruit_ILI9340(_cs,   _dc,  _mosi, _sclk, _rst,  _miso);
//Adafruit_ILI9340 tft1 = Adafruit_ILI9340(_cs1,  _dc1, _mosi, _sclk, _rst1, _miso);
textGauge Gspeed = textGauge(&tft1, 20, 20, 75, 40);
textGauge Gtemp  = textGauge(&tft1, 0, 0, 128, 32);
textGauge  Glabel = textGauge(&tft1, 95, 20, 65, 40);

void setup() {
  Serial.begin(9600);
  delay(2000);
  //tft.begin();
  Serial.printf("beginning initialization \n");
  tft1.begin();
  pinMode(_led, OUTPUT);
  analogWrite(_led, 255);
  tft1.setRotation(3);
  tft1.clearDisplay();
  tft1.printf("tft1");

  /*
  for (int __i=0; __i<=10;__i++){
    Serial.print(".");
    tft1.print(".");
    delay(100);
  }
  */
//delay(5000);
  Serial.printf("tft1 initialized \n");

  /*
   *
   tft.begin();
  tft.setRotation(3);
  tft.clearDisplay();
  tft.printf("tft");
  Serial.printf("tft initialized \n");
  delay(5000);
  //tft1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  */

//  Serial.printf("starting TFT with buffer at 0x%x\n", &tft);
  Serial.println("start init Gspeed");
  Gspeed.setAutoRedraw(true);

  Gspeed.setFGColor(WHITE);
  Gspeed.setBGColor(BLUE);
  Gspeed.setBorder(2);
  Gspeed.setBorderColor(GREEN);
  Gspeed.setGutter(2,2,2,2);
  Gspeed.setHAlign(RIGHT);
  Gspeed.setVAlign(TOP);
  //Gspeed.setFont(&FreeMonoBold18pt7b);
  Gspeed.setFont(&digital7monoitalic18pt7b);
  Gspeed.redraw();
  Serial.println("end init Gspeed");
  Serial.println("start init Glabel");
  Glabel.setAutoRedraw(true);

  Glabel.setFGColor(WHITE);
  Glabel.setBGColor(BLACK);
  Glabel.setBorder(2);
  Glabel.setBorderColor(RED);
  Glabel.setHAlign(LEFT);
  Glabel.setVAlign(MIDDLE);
  Glabel.setGutter(1,1,1,1);
  Glabel.setFont(&FreeSans12pt7b);
  Glabel.setValue("kmh");
  Glabel.redraw();
  Serial.println("end init Glabel");

  Gtemp.setAutoRedraw(true);
  Gtemp.setGutter(1,1,1,1);
  Gtemp.setFGColor(1);
  Gtemp.setBGColor(0);
  Gtemp.setHAlign(CENTER);
  Gtemp.setVAlign(MIDDLE);
  Gtemp.setFont(&FreeMonoBold18pt7b);
  Gtemp.redraw();
  //oled.display();
  //oled.printf("starting");
  //delay(1000);
}
void loop() {
  // put your main code here, to run repeatedly:
  time_t t0, t1;

  Gtemp.setDisplay(&tft1);
  Gtemp.setPosition(20,80);
  Gtemp.setFGColor(WHITE);
  Gtemp.setBGColor(BLUE);
  Gtemp.setBorder(0);
  Gtemp.setBorderColor(YELLOW);
  //oled.clearDisplay();

  //tft.fillScreen(BLUE);
  tft1.fillScreen(BLACK);
  Gtemp.redraw();
  Glabel.redraw();
  Gspeed.redraw();
  for (uint8_t i = 0; i <= 199; i++) {
    t0 = micros();
    Gspeed.setValue(i);
    Gspeed.redraw();
    t1 = micros();
    Serial.printf("(speed) update took %ius\n", t1 - t0);
    t0 = micros();
    Gtemp.setValue((int)(i / 10));
    Gtemp.redraw();
    t1 = micros();
    //Serial.printf("(temp-tft1) update took %ius\n", t1 - t0);
    //delay(100);
  }
  Serial.println("setting Label");
  Glabel.setValue("mph");
  Glabel.redraw();
  Gtemp.setDisplay(&tft1);
  Gtemp.setPosition(20,80);
  Gtemp.setFGColor(WHITE);
  Gtemp.setBGColor(BLUE);

  //oled.display();
  //tft.fillScreen(BLUE);
  Gtemp.redraw();
  Glabel.redraw();
  Gspeed.redraw();
  for (int i = 200; i >= 1; i--) {
    t0 = micros();
    Gspeed.setValue(i);
    Gspeed.redraw();
    t1 = micros();
    Serial.printf("update took %ius\n", t1 - t0);
    t0 = micros();
    Gtemp.setValue((int)(i / 10));
    Gtemp.redraw();
    t1 = micros();
    //Serial.printf("(temp tft) update took %ius\n", t1 - t0);
    //delay(100);
  }
  Glabel.setValue("kmh");
}
