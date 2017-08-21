#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <spi4teensy3>
//#include "Adafruit_ILI9340.h"
#include "Adafruit_ILI9341.h"
#include <Adafruit_SSD1306.h>
//#include <Adafruit_ST7735.h>
#include "/home/pjakobs/Arduino/Gaugetest/lib/TFTLCD-Library/Adafruit_TFTLCD.h"
#include <Wire.h>
#include <Gauges.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/digital7italic12pt7b.h>
#include <Fonts/digital7italic18pt7b.h>
#include <Fonts/digital7monoitalic12pt7b.h>
#include <Fonts/digital7monoitalic18pt7b.h>
#include <time.h>

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
//Adafruit_ILI9341 tft1 = Adafruit_ILI9341(_cs,  _dc, _rst);

// optional

//Adafruit_SSD1306 tft1 = Adafruit_SSD1306();
//Adafruit_ILI9340 tft  = Adafruit_ILI9340(_cs,   _dc,  _mosi, _sclk, _rst,  _miso);
//Adafruit_ILI9340 tft1 = Adafruit_ILI9340(_cs1,  _dc1, _mosi, _sclk, _rst1, _miso);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
Adafruit_TFTLCD tft1 = Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

textGauge Gspeed = textGauge(&tft1, 20, 20, 75, 40);
textGauge Gtemp  = textGauge(&tft1, 0, 0, 40, 32);
textGauge Glabel = textGauge(&tft1, 95, 20, 65, 40);
tapeGauge GspeedGauge = tapeGauge(&tft1, 20,120,100,32,TAPE_RIGHTLEFT);
tapeGauge GspeedGauge1 = tapeGauge(&tft1, 20,160,100,32,TAPE_LEFTRIGHT);
tapeGauge GspeedGauge2 = tapeGauge(&tft1, 170,20,40,200,TAPE_BOTTOMUP);
tapeGauge GspeedGauge3 = tapeGauge(&tft1, 220,20,40,200,TAPE_TOPDOWN);
void setup() {
  Serial.begin(9600);
  delay(2000);
  //tft.begin();

  Serial.println("beginning initialization");
  //tft1.begin();
  pinMode(_led, OUTPUT);
  analogWrite(_led, 255);
  //tft1.setRotation(3);
  //tft1.clearDisplay();
  //tft1.printf("tft1");

  /*
  for (int __i=0; __i<=10;__i++){
    Serial.print(".");
    tft1.print(".");
    delay(100);
  }
  */
//delay(5000);
  Serial.println("tft1 initialized");

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

  /*Serial.printf("starting TFT with buffer at 0x%x\n", &tft);*/
  Serial.println("start init Gspeed");
  Gspeed.setAutoRedraw(true);
  Gspeed.setFGColor(WHITE);
  Gspeed.setBGColor(BLUE);
  Gspeed.setBorder(2);
  Gspeed.setBorderColor(GREEN);
  Gspeed.setGutter(2,2,2,2);
  Gspeed.setHAlign(TEXT_RIGHT);
  Gspeed.setVAlign(TEXT_CENTER);
  Gspeed.setFont(&FreeMonoBold18pt7b);
  //Gspeed.setFont(&digital7monoitalic18pt7b);
  Gspeed.redraw();
  Serial.println("end init Gspeed");
  Serial.println("start init Glabel");
  
  Glabel.setFGColor(WHITE);
  Glabel.setBGColor(BLACK);
  Glabel.setBorder(2);
  Glabel.setBorderColor(RED);
  Glabel.setHAlign(TEXT_LEFT);
  Glabel.setVAlign(TEXT_MIDDLE);
  Glabel.setGutter(1,1,1,1);
  Glabel.setFont(&FreeSans12pt7b);
  Glabel.setAutoRedraw(true);
  Glabel.setValue("kmh");
  
  Gtemp.setAutoRedraw(true);
  Gtemp.setGutter(1);
  Gtemp.setFGColor(1);
  Gtemp.setBGColor(0);
  Gtemp.setHAlign(TEXT_CENTER);
  Gtemp.setVAlign(TEXT_MIDDLE);
  Gtemp.setFont(&FreeMonoBold18pt7b);
  Gtemp.redraw();

  GspeedGauge.setMinMax(0,200);
  GspeedGauge.setColors(GREEN,120,YELLOW,180,RED);
  GspeedGauge.setAutoRedraw(true);
  GspeedGauge.setMargins(1);
  GspeedGauge.setBorder(2);
  GspeedGauge.setBorderColor(BLUE);
  
  GspeedGauge1.setMinMax(0,200);
  GspeedGauge1.setColors(GREEN,120,YELLOW,180,RED);
  GspeedGauge1.setAutoRedraw(true);
  GspeedGauge1.setMargins(1);
  GspeedGauge1.setBorder(2);
  GspeedGauge1.setBorderColor(WHITE);
  
  GspeedGauge2.setMinMax(0,200);
  GspeedGauge2.setColors(GREEN,60,YELLOW,140,RED);
  GspeedGauge2.setAutoRedraw(true);
  GspeedGauge2.setMargins(2);
  GspeedGauge2.setBorder(2);
  GspeedGauge2.setBorderColor(WHITE);

  GspeedGauge3.setMinMax(0,200);
  GspeedGauge3.setColors(BLUE,100,RED);
  GspeedGauge3.setAutoRedraw(true);
  GspeedGauge3.setMargins(2);
  GspeedGauge3.setBorder(2);
  GspeedGauge3.setBorderColor(WHITE);
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
  for (uint8_t i = 0; i <= 200; i++) {
    t0 = micros();
    Gspeed.setValue(i);
    //t1 = micros();
    //Serial.printf("(speed) update took %ius\n", t1 - t0);
    Gtemp.setValue((int)i / 10);
    //t0 = micros();
    GspeedGauge.setValue(i);
    GspeedGauge1.setValue(i);
    GspeedGauge2.setValue(i);
    GspeedGauge3.setValue(i);
    t1 = micros();
    //Serial.printf("(temp-tft1) update took %ius\n", t1 - t0);
    
  }
  Serial.println("setting Label");
  Glabel.setValue("mph");
  Glabel.redraw();
  Gtemp.setDisplay(&tft1);
  Gtemp.setPosition(20,80);
  Gtemp.setFGColor(WHITE);
  Gtemp.setBGColor(BLUE);

  Gtemp.redraw();
  Glabel.redraw();
  Gspeed.redraw();
  delay(1000);
  
  for (int i = 199; i >= 1; i--) {
    t0 = micros();
    Gspeed.setValue(i);
    //t1 = micros();
    //Serial.printf("update took %ius\n", t1 - t0);
    
    Gtemp.setValue((int)i / 10);
    //t0 = micros();
    GspeedGauge.setValue(i);
    GspeedGauge1.setValue(i);
    GspeedGauge2.setValue(i);
    GspeedGauge3.setValue(i);
    t1 = micros();
    //Serial.printf("(temp tft) update took %ius\n", t1 - t0);
    
  }
  Glabel.setValue("kmh");
}
