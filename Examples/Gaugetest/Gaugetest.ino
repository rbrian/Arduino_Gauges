#include "SPI.h"
//#include <spi4teensy3>
#include "Adafruit_ILI9340.h"
#include "Adafruit_GFX.h"
#include <Gauges.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>

#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8
#define _led 6

// Common 16-bit colors:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
    
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

integerGauge Gspeed = integerGauge(&tft);
integerGauge Gtemp = integerGauge(&tft);
textGauge Glabel = textGauge(&tft);

void setup() {
//test->setDisplay(tft);
  Serial.begin(9600);
  delay(200);
  tft.begin();
  Serial.printf("starting TFT with buffer at 0x%x\n",&tft);
  Serial.print("setting Rotation\n");
  tft.setRotation(3);
  Serial.print("enabling Backlight\n");
  tft.fillScreen(127);
  pinMode(_led, OUTPUT);
  analogWrite(_led, 255);
}

void loop() {
  // put your main code here, to run repeatedly:
  time_t t0,t1;
  Gspeed.setPosition(20,20);
  Gspeed.setSize(75,40);
  Gspeed.setAutoRedraw(false);
  Gspeed.setFormatString(String("%3i"));
  Gspeed.setFGColor(WHITE);
  Gspeed.setBGColor(BLACK);
  Gspeed.setCursor(10,26);
  Gspeed.setFont(&FreeMonoBold18pt7b);
  Gspeed.redraw();

  Glabel.setPosition(95,20);
  Glabel.setSize(65,40);
  Glabel.setAutoRedraw(false);
  //Glabel.setFormatString(String("%2iC"));
  Glabel.setValue("km/h");
  Glabel.setFGColor(WHITE);
  Glabel.setBGColor(BLACK);
  Glabel.setCursor(0,26);
  Glabel.setFont(&FreeMonoBold12pt7b);
  Glabel.redraw();


  Gtemp.setPosition(20,80);
  Gtemp.setSize(51,27);
  Gtemp.setAutoRedraw(true);
  Gtemp.setFormatString(String("%2iC"));
  Gtemp.setFGColor(0);
  Gtemp.setBGColor(65535);
  Gtemp.setCursor(5,15);
  Gtemp.setFont(&FreeMonoBold12pt7b);
  Gtemp.redraw();
  

  for(int i=0;i<=199;i++){
    //Serial.printf("setting value to %i\n", i);
    t0=micros();
    Gspeed.setValue(i);
    t1=micros();
    Serial.printf("update took %ius\n",t1-t0);
    //Serial.printf("reading value as %i\n", Gspeed.getValue());
    t0=micros();
    Gspeed.redraw();
    t1=micros();
    Serial.printf("redraw took %ius\n",t1-t0);
    t0=micros();
    Gtemp.setValue((int)i/10);
    t1=micros();
    Serial.printf("temp redraw took %ius\n",t1-t0);
    //Serial.println("finished redraw");
    //delay(100);
  }
}
